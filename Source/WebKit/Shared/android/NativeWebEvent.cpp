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
#include "NativeWebEvent.h"

#include "WebEventEnumerationsNatives.h"
#include <WebCore/Scrollbar.h>
#include <WebCore/WheelEvent.h>
#include <WebCore/WindowsKeyboardCodes.h>

using namespace WebCore;
using namespace org::webkit;

namespace WebKit {

static inline double convertMillisecondToSecond(int64_t timestamp)
{
    return static_cast<double>(timestamp) / 1000;
}

NativeWebMouseEvent NativeWebEvent::createMouseEvent(int64_t timestamp, WebEvent::Type action, WebMouseEvent::Button button, WebEvent::Modifiers modifiers, float x, float y, float globalX, float globalY,
    int32_t clickCount, const AffineTransform& transformToContent)
{
    return NativeWebMouseEvent(WebMouseEvent(action, button,
        IntPoint(transformToContent.mapPoint(FloatPoint(x, y))),
        IntPoint(globalX, globalY),
        0.0f, 0.0f, 0.0f, clickCount, modifiers,
        WallTime::fromRawSeconds(convertMillisecondToSecond(timestamp))));
}

NativeWebWheelEvent NativeWebEvent::createWheelEvent(int64_t timestamp, int32_t modifiers, float deltaX, float deltaY, float x, float y, float globalX, float globalY, const AffineTransform& transformToContent, float deviceScaleFactor)
{
    // How many pixels should we scroll per line? Gecko uses the height of the
    // current line, which means scroll distance changes as you go through the
    // page or go to different pages. IE 7 is ~50 px/line, although the value
    // seems to vary slightly by page and zoom level. Since IE 7 has a
    // smoothing algorithm on scrolling, it can get away with slightly larger
    // scroll values without feeling jerky. Here we use 100 px per three lines.
    static const float scrollbarPixelsPerLine = 100.0f / 3.0f * deviceScaleFactor;

    // FIXME: Need to get scroll chars / scroll lines from Platform.
    static const float numberOfLinePerWheelTick = 3;

    FloatSize wheelTicks = FloatSize(deltaX / WheelEvent::TickMultiplier, deltaY / WheelEvent::TickMultiplier);

    FloatSize delta = wheelTicks;
    delta.scale(numberOfLinePerWheelTick * scrollbarPixelsPerLine);

    return NativeWebWheelEvent(WebWheelEvent(WebEvent::Wheel, IntPoint(transformToContent.mapPoint(FloatPoint(x, y))), IntPoint(globalX, globalY),
        delta, wheelTicks, WebWheelEvent::ScrollByPixelWheelEvent, toModifiers(modifiers), WallTime::fromRawSeconds(convertMillisecondToSecond(timestamp))));
}

// FIXME: This is incomplete. We could change this to mirror
// more like what Firefox does, and generate these switch statements
// at build time.
static String keyIdentifierForWindowsKeyCode(int32_t keyCode)
{
    switch (keyCode) {
        case VK_MENU:
            return "Alt";
        case VK_CONTROL:
            return "Control";
        case VK_SHIFT:
            return "Shift";
        case VK_CAPITAL:
            return "CapsLock";
        case VK_LWIN:
        case VK_RWIN:
            return "Win";
        case VK_CLEAR:
            return "Clear";
        case VK_DOWN:
            return "Down";
        // "End"
        case VK_END:
            return "End";
        // "Enter"
        case VK_RETURN:
            return "Enter";
        case VK_EXECUTE:
            return "Execute";
        case VK_F1:
            return "F1";
        case VK_F2:
            return "F2";
        case VK_F3:
            return "F3";
        case VK_F4:
            return "F4";
        case VK_F5:
            return "F5";
        case VK_F6:
            return "F6";
        case VK_F7:
            return "F7";
        case VK_F8:
            return "F8";
        case VK_F9:
            return "F9";
        case VK_F10:
            return "F11";
        case VK_F12:
            return "F12";
        case VK_F13:
            return "F13";
        case VK_F14:
            return "F14";
        case VK_F15:
            return "F15";
        case VK_F16:
            return "F16";
        case VK_F17:
            return "F17";
        case VK_F18:
            return "F18";
        case VK_F19:
            return "F19";
        case VK_F20:
            return "F20";
        case VK_F21:
            return "F21";
        case VK_F22:
            return "F22";
        case VK_F23:
            return "F23";
        case VK_F24:
            return "F24";
        case VK_HELP:
            return "Help";
        case VK_HOME:
            return "Home";
        case VK_INSERT:
            return "Insert";
        case VK_LEFT:
            return "Left";
        case VK_NEXT:
            return "PageDown";
        case VK_PRIOR:
            return "PageUp";
        case VK_PAUSE:
            return "Pause";
        case VK_SNAPSHOT:
            return "PrintScreen";
        case VK_RIGHT:
            return "Right";
        case VK_SCROLL:
            return "Scroll";
        case VK_SELECT:
            return "Select";
        case VK_UP:
            return "Up";
        // Standard says that DEL becomes U+007F.
        case VK_DELETE:
            return "U+007F";
        default:
            return String::format("U+%04X", toASCIIUpper(keyCode));
    }
}

NativeWebKeyboardEvent NativeWebEvent::createKeyboardEvent(int64_t timestamp, WebEvent::Type action, WebEvent::Modifiers modifiers, int32_t windowsVirtualKeyCode, int32_t nativeVirtualKeyCode,
    const String& text, const String& unmodifiedText, bool isAutoRepeat, bool isSystemKey, bool isKeypadKey, int64_t downTime)
{
    return NativeWebKeyboardEvent(WebKeyboardEvent(action, text, unmodifiedText, keyIdentifierForWindowsKeyCode(windowsVirtualKeyCode),
        windowsVirtualKeyCode, nativeVirtualKeyCode, 0, isAutoRepeat, isKeypadKey, isSystemKey, modifiers, WallTime::fromRawSeconds(convertMillisecondToSecond(timestamp))), downTime);
}

#if ENABLE(TOUCH_EVENTS)
NativeWebTouchEvent NativeWebEvent::createTouchEvent(int64_t timestamp, WebEvent::Type action, WebEvent::Modifiers modifiers, float x, float y, float globalX, float globalY,
    int32_t id, WebPlatformTouchPoint::TouchPointState touchPointState, const AffineTransform& transformToContent)
{
    return NativeWebTouchEvent(WebTouchEvent(action, Vector<WebPlatformTouchPoint>(1, WebPlatformTouchPoint(id, touchPointState,
        IntPoint(globalX, globalY), IntPoint(transformToContent.mapPoint(FloatPoint(x, y))))), modifiers,
        WallTime::fromRawSeconds(convertMillisecondToSecond(timestamp))));
}
#endif

WebEvent::Type NativeWebEvent::toMouseAction(int32_t value)
{
    switch (value) {
    case WebEventEnumerations::EVENT_MOUSE_PRESSED:
        return WebEvent::MouseDown;
    case WebEventEnumerations::EVENT_MOUSE_RELEASED:
        return WebEvent::MouseUp;
    case WebEventEnumerations::EVENT_MOUSE_MOVED:
        return WebEvent::MouseMove;
    default:
        return WebEvent::NoType;
    }
}

WebEvent::Type NativeWebEvent::toKeyAction(int32_t value)
{
    switch (value) {
    case WebEventEnumerations::EVENT_RAW_KEY_DOWN:
        return WebEvent::RawKeyDown;
    case WebEventEnumerations::EVENT_KEY_UP:
        return WebEvent::KeyUp;
    case WebEventEnumerations::EVENT_KEY_DOWN:
        return WebEvent::Char;
    default:
        return WebEvent::NoType;
    }
}

#if ENABLE(TOUCH_EVENTS)
WebEvent::Type NativeWebEvent::toTouchAction(int32_t value)
{
    switch (value) {
    case WebEventEnumerations::EVENT_TOUCH_CANCEL:
        return WebEvent::TouchCancel;
    case WebEventEnumerations::EVENT_TOUCH_START:
        return WebEvent::TouchStart;
    case WebEventEnumerations::EVENT_TOUCH_END:
        return WebEvent::TouchEnd;
    case WebEventEnumerations::EVENT_TOUCH_MOVE:
        return WebEvent::TouchMove;
    default:
        return WebEvent::NoType;
    }
}
#endif

WebEvent::Modifiers NativeWebEvent::toModifiers(int32_t value)
{
    uint32_t modifiers = 0;

    if (value & WebEventEnumerations::EVENT_ALT_KEY)
        modifiers |= WebEvent::AltKey;
    if (value & WebEventEnumerations::EVENT_CTRL_KEY)
        modifiers |= WebEvent::ControlKey;
    if (value & WebEventEnumerations::EVENT_META_KEY)
        modifiers |= WebEvent::MetaKey;
    if (value & WebEventEnumerations::EVENT_SHIFT_KEY)
        modifiers |= WebEvent::ShiftKey;

    return static_cast<WebEvent::Modifiers>(modifiers);
}

WebMouseEvent::Button NativeWebEvent::toButton(int32_t value)
{
    switch (value) {
    case WebEventEnumerations::EVENT_MOUSE_LEFT_BUTTON:
        return WebMouseEvent::LeftButton;
    case WebEventEnumerations::EVENT_MOUSE_RIGHT_BUTTON:
        return WebMouseEvent::RightButton;
    case WebEventEnumerations::EVENT_MOUSE_MIDDLE_BUTTON:
        return WebMouseEvent::MiddleButton;
    default:
        return WebMouseEvent::NoButton;
    }
}

#if ENABLE(TOUCH_EVENTS)
WebPlatformTouchPoint::TouchPointState NativeWebEvent::toTouchPointState(int32_t value)
{
    switch (value) {
    case WebEventEnumerations::EVENT_TOUCH_CANCEL:
        return WebPlatformTouchPoint::TouchCancelled;
    case WebEventEnumerations::EVENT_TOUCH_START:
        return WebPlatformTouchPoint::TouchPressed;
    case WebEventEnumerations::EVENT_TOUCH_END:
        return WebPlatformTouchPoint::TouchReleased;
    case WebEventEnumerations::EVENT_TOUCH_MOVE:
        return WebPlatformTouchPoint::TouchMoved;
    default:
        return WebPlatformTouchPoint::TouchStationary;
    }
}
#endif

NativeWebMouseEvent::NativeWebMouseEvent(WebMouseEvent&& event)
    : WebMouseEvent(event)
{
}

NativeWebWheelEvent::NativeWebWheelEvent(WebWheelEvent&& event)
    : WebWheelEvent(event)
{
}

NativeWebKeyboardEvent::NativeWebKeyboardEvent(WebKeyboardEvent&& event, int64_t downTime)
    : WebKeyboardEvent(event)
    , m_downTime(downTime)
{
}

#if ENABLE(TOUCH_EVENTS)
NativeWebTouchEvent::NativeWebTouchEvent(WebTouchEvent&& event)
    : WebTouchEvent(event)
{
}
#endif

}
