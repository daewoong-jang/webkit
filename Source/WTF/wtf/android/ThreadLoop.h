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

#ifndef ThreadLoop_h
#define ThreadLoop_h

#include <wtf/Noncopyable.h>
#include <functional>

namespace WTF {

class ThreadLoop {
    WTF_MAKE_NONCOPYABLE(ThreadLoop)
public:
    WTF_EXPORT_PRIVATE static ThreadLoop& current();
    virtual ~ThreadLoop() = default;

    WTF_EXPORT_PRIVATE virtual void dispatch(std::function<void ()> function) = 0;
    WTF_EXPORT_PRIVATE virtual int32_t registerTimer() = 0;
    WTF_EXPORT_PRIVATE virtual void dispatchTimer(std::function<void ()> function, int32_t timerID, double nextFireIntervalInMillis) = 0;
    WTF_EXPORT_PRIVATE virtual void removeTimer(int32_t timerID) = 0;

    WTF_EXPORT_PRIVATE virtual void run() = 0;
    WTF_EXPORT_PRIVATE virtual void step() = 0;
    WTF_EXPORT_PRIVATE virtual void stop() = 0;

protected:
    ThreadLoop() = default;
};

}

#endif
