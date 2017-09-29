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

#ifndef DispatchQueue_h
#define DispatchQueue_h

#include <functional>
#include <time.h>
#include <sys/select.h>
#include <wtf/HashSet.h>
#include <wtf/RefCounted.h>
#include <wtf/RunLoop.h>
#include <wtf/ThreadSafeRefCounted.h>
#include <wtf/Threading.h>
#include <wtf/Vector.h>

namespace WTF {

class TimerWorkItem;
class WorkItem;

class DispatchQueue : public ThreadSafeRefCounted<DispatchQueue> {
public:
    static Ref<DispatchQueue> create(const char* name, bool dispatchOnWorkerThread = true);
    ~DispatchQueue();

    void dispatch(std::unique_ptr<WorkItem>);
    void dispatch(std::unique_ptr<TimerWorkItem>);
    void removeTimer(uint64_t);
    void run();
    void step();
    void stopThread();
    void setSocketEventHandler(int, std::function<void ()>);
    void clearSocketEventHandler();

    RunLoop& runLoop() { return *m_runLoop; }

private:
    class ThreadContext;

    DispatchQueue();

    void performWork();
    void performTimerWork();
    void performFileDescriptorWork();
    void insertTimerWorkItem(std::unique_ptr<TimerWorkItem>);
    void removeTimerWorkItem(uint64_t);
    void dispatchQueueThread();
    void wakeUpThread();
    timeval* getNextTimeOut() const;

    fd_set m_fileDescriptorSet;
    int m_maxFileDescriptor;
    int m_readFromPipeDescriptor;
    int m_writeToPipeDescriptor;
    Mutex m_writeToPipeDescriptorLock;

    ThreadIdentifier m_dispatchThread;
    Lock m_initializeRunLoopConditionMutex;
    Condition m_initializeRunLoopCondition;
    RunLoop* m_runLoop;
    bool m_isThreadRunning;
    mutable bool m_preventThreadBlock;

    int m_socketDescriptor;
    std::function<void ()> m_socketEventHandler;

    Vector<std::unique_ptr<WorkItem>> m_workItems;
    Mutex m_workItemsLock;

    HashSet<uint64_t> m_activeTimers;
    Vector<std::unique_ptr<TimerWorkItem>> m_timerWorkItems;
    mutable Mutex m_timerWorkItemsLock;
};

}

using WTF::DispatchQueue;

#endif // DispatchQueue_h
