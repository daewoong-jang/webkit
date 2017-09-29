/*
 * Copyright (C) 2013 Samsung Electronics. All rights reserved.
 * Copyright (C) 2015 NAVER Corp. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "DispatchQueue.h"

#include "DispatchQueueWorkItem.h"
#include <safeint/safeint.h>
#include <unistd.h>
#include <wtf/Assertions.h>
#include <wtf/CurrentTime.h>
#include <wtf/Threading.h>

namespace WTF {

static const int microSecondsPerSecond = 1000000;
static const int nanoSecondsPerSecond = 1000000000;
static const int invalidSocketDescriptor = -1;
static const char wakeUpThreadMessage = 'W';

class DispatchQueue::ThreadContext {
public:
    static void start(const char* name, Ref<DispatchQueue> dispatchQueue)
    {
        ThreadContext* threadContext = new ThreadContext(WTFMove(dispatchQueue));
        threadContext->m_thread = Thread::create(name, [threadContext] { ThreadContext::function(threadContext); });
        threadContext->m_thread->detach();
    }

private:
    ThreadContext(Ref<DispatchQueue> dispatchQueue)
        : m_dispatchQueue(WTFMove(dispatchQueue))
    {
    }

    static void* function(ThreadContext* threadContext)
    {
        std::unique_ptr<ThreadContext>(threadContext)->m_dispatchQueue->dispatchQueueThread();
        return 0;
    }

    Ref<DispatchQueue> m_dispatchQueue;
    RefPtr<Thread> m_thread;
};

Ref<DispatchQueue> DispatchQueue::create(const char* name, bool dispatchOnWorkerThread)
{
    Ref<DispatchQueue> dispatchQueue = adoptRef<DispatchQueue>(*new DispatchQueue());

    if (dispatchOnWorkerThread) {
        LockHolder locker(dispatchQueue->m_initializeRunLoopConditionMutex);
        ThreadContext::start(name, dispatchQueue.copyRef());
        dispatchQueue->m_initializeRunLoopCondition.wait(dispatchQueue->m_initializeRunLoopConditionMutex);
    }

    return dispatchQueue;
}

DispatchQueue::DispatchQueue()
    : m_dispatchThread(0)
    , m_runLoop(nullptr)
    , m_isThreadRunning(true)
    , m_preventThreadBlock(false)
{
    int fds[2];
    if (pipe(fds))
        ASSERT_NOT_REACHED();

    m_readFromPipeDescriptor = fds[0];
    m_writeToPipeDescriptor = fds[1];
    FD_ZERO(&m_fileDescriptorSet);
    FD_SET(m_readFromPipeDescriptor, &m_fileDescriptorSet);
    m_maxFileDescriptor = m_readFromPipeDescriptor;

    m_socketDescriptor = invalidSocketDescriptor;
}

DispatchQueue::~DispatchQueue()
{
    close(m_readFromPipeDescriptor);
    close(m_writeToPipeDescriptor);
}

void DispatchQueue::dispatch(std::unique_ptr<WorkItem> item)
{
    {
        MutexLocker locker(m_workItemsLock);
        m_workItems.append(WTFMove(item));
    }

    wakeUpThread();
}

void DispatchQueue::dispatch(std::unique_ptr<TimerWorkItem> item)
{
    {
        MutexLocker locker(m_timerWorkItemsLock);
        if (item->timerID())
            removeTimerWorkItem(item->timerID());

        insertTimerWorkItem(WTFMove(item));
    }

    wakeUpThread();
}

void DispatchQueue::removeTimer(uint64_t timerID)
{
    MutexLocker locker(m_timerWorkItemsLock);
    removeTimerWorkItem(timerID);
}

void DispatchQueue::run()
{
    dispatchQueueThread();
}

void DispatchQueue::step()
{
    performWork();
}

void DispatchQueue::stopThread()
{
    ASSERT(m_socketDescriptor == invalidSocketDescriptor);

    m_isThreadRunning = false;
    wakeUpThread();
}

void DispatchQueue::setSocketEventHandler(int fileDescriptor, std::function<void ()> function)
{
    ASSERT(m_socketDescriptor == invalidSocketDescriptor);

    m_socketDescriptor = fileDescriptor;
    m_socketEventHandler = WTFMove(function);

    if (fileDescriptor > m_maxFileDescriptor)
        m_maxFileDescriptor = fileDescriptor;
    FD_SET(fileDescriptor, &m_fileDescriptorSet);
}

void DispatchQueue::clearSocketEventHandler()
{
    ASSERT(m_socketDescriptor != invalidSocketDescriptor);

    if (m_socketDescriptor == m_maxFileDescriptor)
        m_maxFileDescriptor = m_readFromPipeDescriptor;

    FD_CLR(m_socketDescriptor, &m_fileDescriptorSet);

    m_socketDescriptor = invalidSocketDescriptor;
    // Reset the handler function object to drop reference counts
    // its parameters might be holding.
    m_socketEventHandler = nullptr;
}

void DispatchQueue::performWork()
{
    while (true) {
        Vector<std::unique_ptr<WorkItem>> workItems;

        {
            MutexLocker locker(m_workItemsLock);
            if (m_workItems.isEmpty())
                return;

            m_workItems.swap(workItems);
        }

        for (size_t i = 0; i < workItems.size(); ++i)
            workItems[i]->dispatch();
    }
}

void DispatchQueue::performTimerWork()
{
    Vector<std::unique_ptr<TimerWorkItem>> timerWorkItems;

    {
        MutexLocker locker(m_timerWorkItemsLock);
        if (m_timerWorkItems.isEmpty())
            return;

        double currentTimeNanoSeconds = monotonicallyIncreasingTime() * nanoSecondsPerSecond;
        Vector<std::unique_ptr<TimerWorkItem>> activeTimerWorkItems;

        for (size_t i = 0; i < m_timerWorkItems.size(); ++i) {
            if (m_timerWorkItems[i]->hasExpired(currentTimeNanoSeconds)) {
                m_activeTimers.remove(m_timerWorkItems[i]->timerID());
                timerWorkItems.append(WTFMove(m_timerWorkItems[i]));
            } else
                activeTimerWorkItems.append(WTFMove(m_timerWorkItems[i]));
        }

        m_timerWorkItems.swap(activeTimerWorkItems);
    }

    for (size_t i = 0; i < timerWorkItems.size(); ++i) {
        // If a timer work item has expired, dispatch the function of the work item.
        timerWorkItems[i]->dispatch();
    }
}

void DispatchQueue::performFileDescriptorWork()
{
    fd_set readFileDescriptorSet = m_fileDescriptorSet;

    if (select(m_maxFileDescriptor + 1, &readFileDescriptorSet, 0, 0, getNextTimeOut()) >= 0) {
        if (FD_ISSET(m_readFromPipeDescriptor, &readFileDescriptorSet)) {
            char message;
            if (read(m_readFromPipeDescriptor, &message, 1) == -1)
                LOG_ERROR("Failed to read from DispatchQueue Thread pipe");

            ASSERT(message == wakeUpThreadMessage);
        }

        if (m_socketDescriptor != invalidSocketDescriptor && FD_ISSET(m_socketDescriptor, &readFileDescriptorSet))
            m_socketEventHandler();
    }
}

void DispatchQueue::insertTimerWorkItem(std::unique_ptr<TimerWorkItem> item)
{
    ASSERT(item);

    size_t position = 0;

    // The items should be ordered by expire time.
    for (; position < m_timerWorkItems.size(); ++position)
        if (item->expirationTimeNanoSeconds() < m_timerWorkItems[position]->expirationTimeNanoSeconds())
            break;

    if (item->timerID())
        m_activeTimers.add(item->timerID());
    m_timerWorkItems.insert(position, WTFMove(item));
}

void DispatchQueue::removeTimerWorkItem(uint64_t timerID)
{
    ASSERT(timerID > 0);

    size_t position = 0;

    if (m_timerWorkItems.isEmpty() || !m_activeTimers.contains(timerID))
        return;

    for (; position < m_timerWorkItems.size(); ++position)
        if (timerID == m_timerWorkItems[position]->timerID())
            break;

    m_activeTimers.remove(timerID);
    m_timerWorkItems.remove(position);
}

void DispatchQueue::dispatchQueueThread()
{
    m_dispatchThread = currentThread();

    {
        LockHolder locker(m_initializeRunLoopConditionMutex);
        m_runLoop = &RunLoop::current();
        m_initializeRunLoopCondition.notifyOne();
    }

    while (m_isThreadRunning) {
        performWork();
        performTimerWork();
        performFileDescriptorWork();
    }
}

void DispatchQueue::wakeUpThread()
{
    if (currentThread() == m_dispatchThread) {
        m_preventThreadBlock = true;
        return;
    }

    MutexLocker locker(m_writeToPipeDescriptorLock);
    if (write(m_writeToPipeDescriptor, &wakeUpThreadMessage, sizeof(char)) == -1)
        LOG_ERROR("Failed to wake up DispatchQueue Thread");
}

timeval* DispatchQueue::getNextTimeOut() const
{
    static timeval timeValue;
    timeValue.tv_sec = 0;
    timeValue.tv_usec = 0;

    if (m_preventThreadBlock) {
        m_preventThreadBlock = false;
        return &timeValue;
    }

    MutexLocker locker(m_timerWorkItemsLock);
    if (m_timerWorkItems.isEmpty())
        return 0;

    double timeOutSeconds = (m_timerWorkItems[0]->expirationTimeNanoSeconds() - monotonicallyIncreasingTime() * nanoSecondsPerSecond) / nanoSecondsPerSecond;
    if (timeOutSeconds > 0) {
        timeValue.tv_sec = safeint::integral_cast<long>(timeOutSeconds);
        timeValue.tv_usec = safeint::integral_cast<long>((timeOutSeconds - timeValue.tv_sec) * microSecondsPerSecond);
    }

    return &timeValue;
}

}
