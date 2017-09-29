/*
 * Copyright (C) 2017 NAVER Corp. All rights reserved.
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

package org.webkit;

import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;

@NativeNamespace("org.webkit")
@NativeExportMacro("ANDROID_EXTERN")
public class WebEventEnumerations {
    // WebCore/PlatformEvent
    @AccessedByNative
    public static final int EVENT_NO_TYPE = 0;

    // WebCore/PlatformKeyboardEvent
    @AccessedByNative
    public static final int EVENT_KEY_DOWN = 1;
    @AccessedByNative
    public static final int EVENT_KEY_UP = 2;
    @AccessedByNative
    public static final int EVENT_RAW_KEY_DOWN = 3;
    @AccessedByNative
    public static final int EVENT_CHAR = 4;

    // WebCore/PlatformMouseEvent
    @AccessedByNative
    public static final int EVENT_MOUSE_MOVED = 5;
    @AccessedByNative
    public static final int EVENT_MOUSE_PRESSED = 6;
    @AccessedByNative
    public static final int EVENT_MOUSE_RELEASED = 7;
    @AccessedByNative
    public static final int EVENT_MOUSE_FORCE_CHANGED = 8;
    @AccessedByNative
    public static final int EVENT_MOUSE_FORCE_DOWN = 9;
    @AccessedByNative
    public static final int EVENT_MOUSE_FORCE_UP = 10;
    @AccessedByNative
    public static final int EVENT_MOUSE_SCROLL = 11;

    // WebCore/PlatformWheelEvent
    @AccessedByNative
    public static final int EVENT_WHEEL = 12;

    // WebCore/PlatformTouchEvent
    @AccessedByNative
    public static final int EVENT_TOUCH_START = 13;
    @AccessedByNative
    public static final int EVENT_TOUCH_MOVE = 14;
    @AccessedByNative
    public static final int EVENT_TOUCH_END = 15;
    @AccessedByNative
    public static final int EVENT_TOUCH_CANCEL = 16;
    @AccessedByNative
    public static final int EVENT_TOUCH_FORCE_CHANGE = 17;

    @AccessedByNative
    public static final int EVENT_ALT_KEY = 1; // 1 << 0
    @AccessedByNative
    public static final int EVENT_CTRL_KEY = 2; // 1 << 1
    @AccessedByNative
    public static final int EVENT_META_KEY = 4; // 1 << 2
    @AccessedByNative
    public static final int EVENT_SHIFT_KEY = 8; // 1 << 3
    @AccessedByNative
    public static final int EVENT_CAPS_LOCK_KEY = 16; // 1 << 4

    // WebCore/PlatformMouseEvent
    @AccessedByNative
    public static final int EVENT_MOUSE_NO_BUTTON = -1;
    @AccessedByNative
    public static final int EVENT_MOUSE_LEFT_BUTTON = 0;
    @AccessedByNative
    public static final int EVENT_MOUSE_MIDDLE_BUTTON = 1;
    @AccessedByNative
    public static final int EVENT_MOUSE_RIGHT_BUTTON = 2;

    // WebKit/WebWheelEvent
    @AccessedByNative
    public static final int GRANULARITY_SCROLL_BY_PAGE_WHEEL_EVENT = 0;
    @AccessedByNative
    public static final int GRANULARITY_SCROLL_BY_PIXEL_WHEEL_EVENT = 1;

    // WebKit/WebTouchEvent
    @AccessedByNative
    public static final int TOUCH_POINT_STATE_TOUCH_RELEASED = 0;
    @AccessedByNative
    public static final int TOUCH_POINT_STATE_TOUCH_PRESSED = 1;
    @AccessedByNative
    public static final int TOUCH_POINT_STATE_TOUCH_MOVED = 2;
    @AccessedByNative
    public static final int TOUCH_POINT_STATE_TOUCH_STATIONARY = 3;
    @AccessedByNative
    public static final int TOUCH_POINT_STATE_TOUCH_CANCELLED = 4;

    @API
    @CallByReference
    public static int getWebEventType(android.view.InputEvent event) {
        switch (event.getSource()) {
        case InputDevice.SOURCE_TOUCHSCREEN: {
            MotionEvent touchEvent = (MotionEvent) event;
            switch (touchEvent.getActionMasked()) {
            case MotionEvent.ACTION_DOWN:
                return EVENT_TOUCH_START;
            case MotionEvent.ACTION_UP:
                return EVENT_TOUCH_END;
            case MotionEvent.ACTION_MOVE:
                return EVENT_TOUCH_MOVE;
            case MotionEvent.ACTION_CANCEL:
                return EVENT_TOUCH_CANCEL;
            default:
                break;
            }
        }
        default:
            break;
        }

        return EVENT_NO_TYPE;
    }

    @API
    @CallByReference
    public static int getWebEventModifiers(android.view.InputEvent event) {
        int modifiers = 0;
        switch (event.getSource()) {
        case InputDevice.SOURCE_TOUCHSCREEN:
        case InputDevice.SOURCE_MOUSE: {
            MotionEvent touchEvent = (MotionEvent) event;
            if ((touchEvent.getMetaState() & KeyEvent.META_ALT_LEFT_ON) != 0)
                modifiers |= EVENT_ALT_KEY;
            if ((touchEvent.getMetaState() & KeyEvent.META_CTRL_LEFT_ON) != 0)
                modifiers |= EVENT_CTRL_KEY;
            if ((touchEvent.getMetaState() & KeyEvent.META_META_LEFT_ON) != 0)
                modifiers |= EVENT_META_KEY;
            if ((touchEvent.getMetaState() & KeyEvent.META_SHIFT_LEFT_ON) != 0)
                modifiers |= EVENT_SHIFT_KEY;
            if ((touchEvent.getMetaState() & KeyEvent.META_CAPS_LOCK_ON) != 0)
                modifiers |= EVENT_CAPS_LOCK_KEY;
        }
        default:
            break;
        }

        return modifiers;
    }

    private WebEventEnumerations() {
        // Utility class, do not instantiate.
    }
}
