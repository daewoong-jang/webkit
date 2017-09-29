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

#include "config.h"
#include "WebEventEnumerations.h"

#include <android/view/KeyEvent.h>
#include <android/view/MotionEvent.h>

namespace org {
namespace webkit {
namespace Managed {

int32_t WebEventEnumerations::getWebEventType(InputEvent& event)
{
    switch (event.getSource()) {
    case InputDevice::SOURCE_TOUCHSCREEN: {
        MotionEvent& touchEvent = static_cast<MotionEvent&>(event);
        switch (touchEvent.getActionMasked()) {
        case MotionEvent::ACTION_DOWN:
            return EVENT_TOUCH_START;
        case MotionEvent::ACTION_UP:
            return EVENT_TOUCH_END;
        case MotionEvent::ACTION_MOVE:
            return EVENT_TOUCH_MOVE;
        case MotionEvent::ACTION_CANCEL:
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

int32_t WebEventEnumerations::getWebEventModifiers(InputEvent& event)
{
    int32_t modifiers = 0;
    switch (event.getSource()) {
    case InputDevice::SOURCE_TOUCHSCREEN:
    case InputDevice::SOURCE_MOUSE: {
        MotionEvent& touchEvent = static_cast<MotionEvent&>(event);
        if (touchEvent.getMetaState() & KeyEvent::META_ALT_LEFT_ON)
            modifiers |= EVENT_ALT_KEY;
        if (touchEvent.getMetaState() & KeyEvent::META_CTRL_LEFT_ON)
            modifiers |= EVENT_CTRL_KEY;
        if (touchEvent.getMetaState() & KeyEvent::META_META_LEFT_ON)
            modifiers |= EVENT_META_KEY;
        if (touchEvent.getMetaState() & KeyEvent::META_SHIFT_LEFT_ON)
            modifiers |= EVENT_SHIFT_KEY;
        if (touchEvent.getMetaState() & KeyEvent::META_CAPS_LOCK_ON)
            modifiers |= EVENT_CAPS_LOCK_KEY;
    }
    default:
        break;
    }

    return modifiers;
}

} // namespace Managed
} // namespace webkit
} // namespace org
