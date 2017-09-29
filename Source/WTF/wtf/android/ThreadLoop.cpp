/*
 * Copyright (C) 2015 NAVER Corp. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "ThreadLoop.h"

#include "DispatchQueue.h"
#include "DispatchQueueWorkItem.h"
#include "MainThread.h"
#include "RunLoop.h"
#include "ThreadSpecific.h"

#include <org/webkit/wtf/Natives/ThreadLoopBackend.h>

#include <androidjni/GlobalRef.h>
#include <safeint/safeint.h>

namespace WTF {

class ThreadLoopHolder;

static int32_t generateTimerID()
{
    static int32_t uniqueTimerID = 1;
    return uniqueTimerID++;
}

class WorkerThreadLoop : public ThreadLoop {
    friend class ThreadLoopHolder;
public:
    static WorkerThreadLoop& current();
    ~WorkerThreadLoop();

    void dispatch(std::function<void ()> function) override;
    int32_t registerTimer() override;
    void dispatchTimer(std::function<void ()> function, int32_t timerID, double nextFireIntervalInMillis) override;
    void removeTimer(int32_t timerID) override;

    void run() override;
    void step() override;
    void stop() override;

private:
    WorkerThreadLoop();

    RefPtr<DispatchQueue> m_dispatchQueue;
};

WorkerThreadLoop& WorkerThreadLoop::current()
{
    ASSERT(!isApplicationUIThread());
    return static_cast<WorkerThreadLoop&>(ThreadLoop::current());
}

WorkerThreadLoop::WorkerThreadLoop()
    : m_dispatchQueue(DispatchQueue::create("org.WebKit.WorkerThreadLoop", false))
{
}

WorkerThreadLoop::~WorkerThreadLoop()
{
}

void WorkerThreadLoop::dispatch(std::function<void ()> function)
{
    m_dispatchQueue->dispatch(WorkItem::create(WorkItemContext(), [=] {
        function();
    }));
}

int32_t WorkerThreadLoop::registerTimer()
{
    return generateTimerID();
}

void WorkerThreadLoop::dispatchTimer(std::function<void ()> function, int32_t timerID, double nextFireIntervalInMillis)
{
    m_dispatchQueue->dispatch(TimerWorkItem::create(WorkItemContext(), [=] {
        function();
    }, Seconds::fromMilliseconds(nextFireIntervalInMillis), timerID));
}

void WorkerThreadLoop::removeTimer(int32_t timerID)
{
    m_dispatchQueue->removeTimer(timerID);
}

void WorkerThreadLoop::run()
{
    m_dispatchQueue->run();
}

void WorkerThreadLoop::step()
{
    m_dispatchQueue->step();
}

void WorkerThreadLoop::stop()
{
    m_dispatchQueue->stopThread();
}

class UIThreadLoop : public ThreadLoop {
    friend class ThreadLoopHolder;
    friend class WTF::Natives::ThreadLoopBackend;
public:
    static UIThreadLoop& current();
    ~UIThreadLoop();

    void dispatch(std::function<void ()> function) override;
    int32_t registerTimer() override;
    void dispatchTimer(std::function<void ()> function, int32_t timerID, double nextFireIntervalInMillis) override;
    void removeTimer(int32_t timerID) override;

    void run() override;
    void step() override;
    void stop() override;

private:
    UIThreadLoop();

    struct Timer {
        std::function<void ()> function;
        int32_t timerID;
        double intervalInMillis;

        Timer(std::function<void ()> function, int32_t timerID, double intervalInMillis)
            : function(function)
            , timerID(timerID)
            , intervalInMillis(intervalInMillis)
        {
        }
    };

    typedef Vector<std::function<void ()>> FunctionQueue;
    FunctionQueue m_functionQueue;
    Mutex m_functionQueueLock;

    typedef HashMap<int32_t, std::unique_ptr<Timer>> TimerMap;
    TimerMap m_activeTimers;
    Mutex m_activeTimersLock;

    JNI::GlobalRef<Natives::ThreadLoopBackend> m_loop;

    void performMessage();
    void performTimer(int32_t timerID);
};

UIThreadLoop& UIThreadLoop::current()
{
    ASSERT(isMainThread());
    return static_cast<UIThreadLoop&>(ThreadLoop::current());
}

UIThreadLoop::UIThreadLoop()
    : m_loop(Natives::ThreadLoopBackend::create())
{
}

UIThreadLoop::~UIThreadLoop()
{
}

void UIThreadLoop::dispatch(std::function<void ()> function)
{
    {
        MutexLocker lock(m_functionQueueLock);
        m_functionQueue.append(function);
    }

    m_loop->dispatch();
}

int32_t UIThreadLoop::registerTimer()
{
    return generateTimerID();
}

void UIThreadLoop::dispatchTimer(std::function<void ()> function, int32_t timerID, double nextFireIntervalInMillis)
{
    {
        MutexLocker lock(m_activeTimersLock);
        m_activeTimers.set(timerID, std::make_unique<Timer>(function, timerID, nextFireIntervalInMillis));
    }

    m_loop->dispatchTimer(timerID, nextFireIntervalInMillis);
}

void UIThreadLoop::removeTimer(int32_t timerID)
{
    {
        MutexLocker lock(m_activeTimersLock);
        TimerMap::iterator it = m_activeTimers.find(timerID);
        if (it == m_activeTimers.end())
            return;

        m_activeTimers.remove(it);
    }

    m_loop->removeTimer(timerID);
}

void UIThreadLoop::run()
{
}

void UIThreadLoop::step()
{
}

void UIThreadLoop::stop()
{
    m_loop->stop();
}

void UIThreadLoop::performMessage()
{
    FunctionQueue firedFunctions;

    {
        MutexLocker lock(m_functionQueueLock);
        m_functionQueue.swap(firedFunctions);
    }

    for (auto& function : firedFunctions)
        function();
}

void UIThreadLoop::performTimer(int32_t timerID)
{
    std::function<void()> firedFunction;

    {
        MutexLocker lock(m_activeTimersLock);
        TimerMap::iterator it = m_activeTimers.find(timerID);
        if (it == m_activeTimers.end()) {
            // The timer must have been stopped after the timer message was posted to the message queue.
            return;
        }

        firedFunction = std::move(it->value->function);

        m_activeTimers.remove(it);
    }

    firedFunction();
}

namespace Natives {

void ThreadLoopBackend::nativePerformMessage()
{
    UIThreadLoop::current().performMessage();
}

void ThreadLoopBackend::nativePerformTimer(int32_t timerID)
{
    UIThreadLoop::current().performTimer(timerID);
}

ThreadLoopBackend* ThreadLoopBackend::nativeCreate()
{
    return new ThreadLoopBackend;
}

}

class ThreadLoopHolder {
public:
    ThreadLoopHolder()
        : m_threadLoop((isApplicationUIThread()) ? static_cast<ThreadLoop*>(new UIThreadLoop) : static_cast<ThreadLoop*>(new WorkerThreadLoop))
    {
    }

    ThreadLoop& threadLoop() { return *m_threadLoop; }

private:
    std::unique_ptr<ThreadLoop> m_threadLoop;
};

inline ThreadLoopHolder& threadLoopHolder()
{
    static std::once_flag onceFlag;
    static ThreadSpecific<ThreadLoopHolder>* holder;
    std::call_once(onceFlag, []{
        holder = new ThreadSpecific<ThreadLoopHolder>;
    });
    return **holder;
}

ThreadLoop& ThreadLoop::current()
{
    return threadLoopHolder().threadLoop();
}

}
