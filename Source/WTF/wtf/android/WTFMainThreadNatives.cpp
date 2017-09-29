/*
 * Copyright (C) 2013-2015 NAVER Corp. All rights reserved.
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
#include "MainThread.h"

#include "RunLoop.h"
#include "Threading.h"
#include <mutex>
#include <org/webkit/wtf/Natives/WTFMainThread.h>

namespace WTF {

static ThreadIdentifier applicationUIThreadIdentifer;

struct ThreadFlags {
    enum {
        UI = 1 << 0,
        Web = 1 << 1,
        Main = 1 << 2,
        Worker = 1 << 3,
    };
};
static thread_local uint32_t threadFlags = 0;

void initializeMainThreadPlatform()
{
    if (isUIThread())
        return;

    ASSERT(!(threadFlags & ThreadFlags::Worker));

    threadFlags |= ThreadFlags::Main;

    if (isApplicationUIThread()) {
        ASSERT(!applicationUIThreadIdentifer);
        applicationUIThreadIdentifer = Thread::currentID();
    }

    initializeGCThreads();
}

void initializeWebThread()
{
    threadFlags |= ThreadFlags::Web;
}

void initializeUIThread()
{
    threadFlags |= ThreadFlags::UI;
}

void scheduleDispatchFunctionsOnMainThread()
{
    RunLoop::main().dispatch(Function<void()>(&dispatchFunctionsFromMainThread));
}

bool isWebThread()
{
    return Natives::WTFMainThread::isWebThread();
}

bool isUIThread()
{
    return Natives::WTFMainThread::isMainThread();
}

bool isApplicationUIThread()
{
    return Natives::WTFMainThread::isUIThread();
}

namespace Natives {

void WTFMainThread::initializeUIThread()
{
    WTF::initializeUIThread();
}

void WTFMainThread::initializeUIWorkerThread()
{
    ASSERT(applicationUIThreadIdentifer);
    threadFlags |= ThreadFlags::Worker;
}

bool WTFMainThread::isMainThread()
{
    return (threadFlags & ThreadFlags::Main) != 0;
}

bool WTFMainThread::isWebThread()
{
    return (threadFlags & ThreadFlags::Web) != 0;
}

bool WTFMainThread::isUIThread()
{
    return (threadFlags & ThreadFlags::UI) != 0;
}

WTFMainThread* WTFMainThread::CTOR()
{
    ASSERT_NOT_REACHED();
    return new WTFMainThread;
}

}
} // namespace WTF
