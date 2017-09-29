/*
 * Copyright (C) 2013 NAVER Corp. All rights reserved.
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
#include "MainThreadSharedTimer.h"

#include <wtf/CurrentTime.h>
#include <wtf/MainThread.h>
#include <wtf/NeverDestroyed.h>
#include <wtf/android/ThreadLoop.h>

#include <mutex>

using namespace WTF;

namespace WebCore {

static int32_t kSharedThreadLoopTimer;

static ThreadLoop& mainThreadLoop()
{
    static ThreadLoop* threadLoop;
    ASSERT(isMainThread());
    static std::once_flag onceFlag;
    std::call_once(onceFlag, []{
        threadLoop = &ThreadLoop::current();
        kSharedThreadLoopTimer = (*threadLoop).registerTimer();
    });

    return *threadLoop;
}

// The fire time is relative to the classic POSIX epoch of January 1, 1970,
// as the result of currentTime() is.
void MainThreadSharedTimer::setFireInterval(Seconds fireTime)
{
    mainThreadLoop().dispatchTimer([=] { fired(); }, kSharedThreadLoopTimer, fireTime.seconds() * 1000.0);
}

void MainThreadSharedTimer::stop()
{
    mainThreadLoop().removeTimer(kSharedThreadLoopTimer);
}

void MainThreadSharedTimer::invalidate()
{
}

}  // namespace WebCore
