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

#if !OS(ANDROID)

#include "ThreadLoopBackend.h"
#include "ThreadSpecific.h"

#include <android/os/Handler.h>
#include <android/os/Looper.h>
#include <android/os/Message.h>

#include <atomic>

using namespace android;

namespace WTF {
namespace Managed {

class ThreadLoopBackendPrivate;

class ThreadLoopBackendHandler : public os::Handler {
public:
    ThreadLoopBackendHandler(ThreadLoopBackendPrivate& loop)
        : m_loop(loop)
    {
    }

    void handleMessage(os::Message& msg) override;

private:
    ThreadLoopBackendPrivate& m_loop;
};

class ThreadLoopBackendPrivate : public ThreadLoopBackend::Private {
public:
    ThreadLoopBackend& m_loop;
    std::atomic<bool> m_messageFired;
    std::unique_ptr<os::Handler> m_handler;

    ThreadLoopBackendPrivate(ThreadLoopBackend& loop)
        : m_loop(loop)
        , m_messageFired(true)
        , m_handler(new ThreadLoopBackendHandler(*this))
    {
    }
    ~ThreadLoopBackendPrivate()
    {
    }
};

static inline ThreadLoopBackendPrivate& d(const std::unique_ptr<ThreadLoopBackend::Private>& p)
{
    return static_cast<ThreadLoopBackendPrivate&>(*p);
}

void ThreadLoopBackend::INIT()
{
    nativeCreate();

    m_private.reset(new ThreadLoopBackendPrivate(*this));
}

void ThreadLoopBackend::dispatch()
{
    if (!d(m_private).m_messageFired.exchange(false)) {
        // mMessageFired was already false.
        return;
    }
    d(m_private).m_handler->sendEmptyMessage(0);
}

void ThreadLoopBackend::dispatchTimer(int32_t timerID, double nextFireIntervalInMillis)
{
    removeTimer(timerID);
    int64_t nextFireInterval = static_cast<int64_t>(nextFireIntervalInMillis);
    d(m_private).m_handler->sendEmptyMessageDelayed(timerID, std::chrono::milliseconds(nextFireInterval));
}

void ThreadLoopBackend::removeTimer(int32_t timerID)
{
    d(m_private).m_handler->removeMessages(timerID);
}

void ThreadLoopBackend::stop()
{
    d(m_private).m_handler->getLooper()->quitSafely();
}

void ThreadLoopBackendHandler::handleMessage(os::Message& msg)
{
    if (msg.what == 0) {
        m_loop.m_messageFired.store(true);
        m_loop.m_loop.nativePerformMessage();
    } else {
        m_loop.m_loop.nativePerformTimer(msg.what);
    }
}

}
}

#endif
