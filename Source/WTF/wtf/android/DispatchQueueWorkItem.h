/*
 * Copyright (C) 2013 Samsung Electronics. All rights reserved.
 * Copyright (C) 2013 NAVER Corp.
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

#ifndef DispatchQueueWorkItem_h
#define DispatchQueueWorkItem_h

#include <wtf/Assertions.h>
#include <wtf/CurrentTime.h>
#include <wtf/FunctionDispatcher.h>
#include <wtf/RefCounted.h>

namespace WTF {

class WorkItemContext {
public:
    virtual ~WorkItemContext() { }
};

class WorkItem {
public:
    static std::unique_ptr<WorkItem> create(WorkItemContext context, Function<void ()>&& function)
    {
        return std::unique_ptr<WorkItem>(new WorkItem(context, WTFMove(function)));
    }
    void dispatch() { m_function(); }

protected:
    WorkItem(WorkItemContext context, Function<void ()>&& function)
        : m_context(WTFMove(context))
        , m_function(WTFMove(function))
    {
    }

private:
    WorkItemContext m_context;
    Function<void ()> m_function;
};

class TimerWorkItem : public WorkItem {
public:
    static std::unique_ptr<TimerWorkItem> create(WorkItemContext context, Function<void ()>&& function, Seconds delaySeconds, uint64_t timerID = 0)
    {
        ASSERT(delaySeconds.value() >= 0);
        return std::unique_ptr<TimerWorkItem>(new TimerWorkItem(context, WTFMove(function), monotonicallyIncreasingTime() * 1000000000.0 + delaySeconds.nanoseconds(), timerID));
    }
    double expirationTimeNanoSeconds() const { return m_expirationTimeNanoSeconds; }
    bool hasExpired(double currentTimeNanoSeconds) const { return currentTimeNanoSeconds >= m_expirationTimeNanoSeconds; }
    uint64_t timerID() const { return m_timerID; }

protected:
    TimerWorkItem(WorkItemContext context, Function<void ()>&& function, double expirationTimeNanoSeconds, uint64_t timerID)
        : WorkItem(WTFMove(context), WTFMove(function))
        , m_expirationTimeNanoSeconds(expirationTimeNanoSeconds)
        , m_timerID(timerID)
    {
    }

private:
    double m_expirationTimeNanoSeconds;
    uint64_t m_timerID;
};

}

using WTF::WorkItemContext;
using WTF::WorkItem;
using WTF::TimerWorkItem;

#endif // DispatchQueueWorkItem_h
