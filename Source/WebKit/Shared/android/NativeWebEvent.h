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

#ifndef NativeWebEvent_h
#define NativeWebEvent_h

#include "NativeWebMouseEvent.h"
#include "NativeWebKeyboardEvent.h"
#include "NativeWebTouchEvent.h"
#include "NativeWebWheelEvent.h"
#include <WebCore/AffineTransform.h>
#include <wtf/text/WTFString.h>

namespace WebKit {

class NativeWebEvent final {
public:
    static NativeWebMouseEvent createMouseEvent(int64_t timestamp, WebEvent::Type action, WebMouseEvent::Button button,
        WebEvent::Modifiers modifiers, float x, float y, float globalX, float globalY,
        int32_t clickCount, const WebCore::AffineTransform& transformToContent);

    static NativeWebWheelEvent createWheelEvent(int64_t timestamp,
        int32_t modifiers, float deltaX, float deltaY, float x, float y, float globalX, float globalY,
        const WebCore::AffineTransform& transformToContent, float deviceScaleFactor);

    static NativeWebKeyboardEvent createKeyboardEvent(int64_t timestamp, WebEvent::Type action,
        WebEvent::Modifiers modifiers, int32_t windowsVirtualKeyCode, int32_t nativeVirtualKeyCode,
        const String& text, const String& unmodifiedText, bool isAutoRepeat, bool isSystemKey, bool isKeypadKey,
        int64_t downTime);

#if ENABLE(TOUCH_EVENTS)
    static NativeWebTouchEvent createTouchEvent(int64_t timestamp, WebEvent::Type action,
        WebEvent::Modifiers modifiers, float x, float y, float globalX, float globalY,
        int32_t id, WebPlatformTouchPoint::TouchPointState, const WebCore::AffineTransform& transformToContent);
#endif

    static WebEvent::Type toMouseAction(int32_t);
    static WebEvent::Type toKeyAction(int32_t);
    static WebEvent::Type toTouchAction(int32_t);

    static WebEvent::Modifiers toModifiers(int32_t);
    static WebMouseEvent::Button toButton(int32_t);

#if ENABLE(TOUCH_EVENTS)
    static WebPlatformTouchPoint::TouchPointState toTouchPointState(int32_t);
#endif
private:
    NativeWebEvent() = default; // Static class, do not instantiate.
};

}

#endif // NativeWebEvent_h
