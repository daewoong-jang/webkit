/*
 * Copyright (C) 2016 NAVER Corp. All rights reserved.
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
#include "MotionDetector.h"

#include "VirtualKeyMap.h"
#include <android/view/InputDevice.h>
#include <android/view/KeyEvent.h>

#include <algorithm>

namespace org {
namespace webkit {

MotionDetector::MotionDetector(Client& c)
    : mClient(c)
    , mState(STATE_DEFAULT)
{
}

MotionDetector::~MotionDetector()
{
}

bool MotionDetector::dispatchGenericEvent(InputEvent& event)
{
    if (isDisabled())
        return mClient.handleUnconfirmed(event);

    if (processStateTransitionEvent(event))
        return true;

    switch (event.getSource()) {
    case InputDevice::SOURCE_TOUCHSCREEN:
        return processTouchEvent(event);
    case InputDevice::SOURCE_MOUSE:
        return processMouseEvent(event);
    case InputDevice::SOURCE_KEYBOARD:
        return processKeyEvent(event);
    default:
        break;
    };

    return mClient.handleUnconfirmed(event);
}

bool MotionDetector::dispatchUnhandledTouchEvent(InputEvent& event)
{
    if (event.getSource() != InputDevice::SOURCE_TOUCHSCREEN)
        return false;

    return processUnhandledTouchEvent(event);
}

void MotionDetector::reset()
{
    mClientSettings = mClient.clientSettings();

    if (isDisabled())
        clearState();
}

void MotionDetector::clearState()
{
    switch (mState) {
    case STATE_PINCH_TO_ZOOM:
        mState = STATE_DEFAULT;
        mClient.handlePinchEnd();
        return;
    case STATE_DEFAULT:
    default:
        return;
    }
}

bool MotionDetector::processStateTransitionEvent(InputEvent& event)
{
    switch (mState) {
    case STATE_DEFAULT:
        if (event.getSource() == InputDevice::SOURCE_MOUSE) {
            MotionEvent& mouseEvent = static_cast<MotionEvent&>(event);
            if (mouseEvent.getActionMasked() == MotionEvent::ACTION_SCROLL &&
                mouseEvent.getMetaState() & KeyEvent::META_CTRL_LEFT_ON) {
                mState = STATE_PINCH_TO_ZOOM;
                mClient.handlePinchBegin(mouseEvent.getX(), mouseEvent.getY());
                return false;
            }
        }
        break;
    case STATE_PINCH_TO_ZOOM:
        if (event.getSource() == InputDevice::SOURCE_KEYBOARD) {
            KeyEvent& keyEvent = static_cast<KeyEvent&>(event);
            if (keyEvent.getAction() == KeyEvent::ACTION_UP &&
                VirtualKeyMap::toVirtualKeyCode(keyEvent.getKeyCode()) == VK_CONTROL) {
                mState = STATE_DEFAULT;
                mClient.handlePinchEnd();
                return false;
            }
        }
        break;
    default:
        break;
    };
    return false;
}

bool MotionDetector::processTouchEvent(InputEvent& event)
{
    MotionEvent& touchEvent = static_cast<MotionEvent&>(event);
    if (touchEvent.getPointerCount() > 1)
        return processUnhandledTouchEvent(event);
    return mClient.handleTouchEvent(touchEvent);
}

bool MotionDetector::processUnhandledTouchEvent(InputEvent& event)
{
    MotionEvent& touchEvent = static_cast<MotionEvent&>(event);
    if (touchEvent.getActionMasked() == MotionEvent::ACTION_DOWN) {
        mLastX = touchEvent.getX();
        mLastY = touchEvent.getY();
        mIsScrolled = false;
    } else if (touchEvent.getActionMasked() == MotionEvent::ACTION_MOVE) {
        float distanceX = mLastX - touchEvent.getX();
        float distanceY = mLastY - touchEvent.getY();

        if (!mIsScrolled
                && sqrt(distanceX * distanceX + distanceY * distanceY) > 40)
            mIsScrolled = true;

        if (mIsScrolled) {
            mClient.handleScrollBy(touchEvent.getX(), touchEvent.getY(),
                    distanceX, distanceY);
            mLastX = touchEvent.getX();
            mLastY = touchEvent.getY();
        }
    } else if (touchEvent.getActionMasked() == MotionEvent::ACTION_UP) {
        if (!mIsScrolled)
            mClient.handleSingleTap(touchEvent.getX(), touchEvent.getY());
    }

    return false;
}

bool MotionDetector::processMouseEvent(InputEvent& event)
{
    MotionEvent& mouseEvent = static_cast<MotionEvent&>(event);
    if (mouseEvent.getActionMasked() == MotionEvent::ACTION_SCROLL)
        return processWheelEvent(event);

    if (mClientSettings.mouseTouchEvents &&
        (mouseEvent.getButtonState() & MotionEvent::BUTTON_PRIMARY)) {
        mouseEvent.setSource(InputDevice::SOURCE_TOUCHSCREEN);
        return processTouchEvent(event);
    }

    return mClient.handleUnconfirmed(event);
}

bool MotionDetector::processWheelEvent(InputEvent& event)
{
    static const float scalePerWheelTick = 1.1f;

    MotionEvent& wheelEvent = static_cast<MotionEvent&>(event);
    float scrollX = wheelEvent.getAxisValue(MotionEvent::AXIS_HSCROLL);
    float scrollY = wheelEvent.getAxisValue(MotionEvent::AXIS_VSCROLL);
    PointF scrollDelta = translateScrollBy(scrollX, scrollY);

    switch (mState) {
    case STATE_DEFAULT: {
        mClient.handleScrollBegin(wheelEvent.getX(), wheelEvent.getY());
        mClient.handleScrollBy(wheelEvent.getX(), wheelEvent.getY(), -scrollDelta.x, -scrollDelta.y);
        mClient.handleScrollEnd(wheelEvent.getX(), wheelEvent.getY());
        return true;
    }
    case STATE_PINCH_TO_ZOOM: {
        float magnification = pow(scalePerWheelTick, scrollDelta.y);
        if (mClientSettings.canHandlePinchBy)
            mClient.handlePinchBy(wheelEvent.getX(), wheelEvent.getY(), magnification);
        else
            mClient.handlePinchTo(translatePinchBy(wheelEvent.getX(), wheelEvent.getY(), magnification));
        return true;
    }
    default:
        break;
    }

    return mClient.handleUnconfirmed(event);
}

bool MotionDetector::processKeyEvent(InputEvent& event)
{
    switch (mState) {
    case STATE_DEFAULT:
        if (translateAccelerator(event))
            return true;
        break;
    default:
        break;
    }

    return mClient.handleUnconfirmed(event);
}

PointF MotionDetector::translateScrollBy(float deltaX, float deltaY)
{
    static const float pixelsPerLineStep = 60.f;
    float clientScaleFactor = mClient.clientScaleFactor();
    return PointF(deltaX * pixelsPerLineStep / clientScaleFactor, deltaY * pixelsPerLineStep / clientScaleFactor);
}

RectF MotionDetector::translatePinchBy(float anchorX, float anchorY, float deltaScale)
{
    RectF viewport = mClient.clientContentViewport();
    float oldScale = mClient.clientScaleFactor();
    float newScale = std::max(mClient.clientMinimumScaleFactor(), std::min(mClient.clientMaximumScaleFactor(), oldScale * deltaScale));

    float newLeft = viewport.left + anchorX / oldScale - anchorX / newScale;
    float newTop = viewport.top + anchorY / oldScale - anchorY / newScale;    
    float newRight = newLeft + mClient.clientWidth() / newScale;
    float newBottom = newTop + mClient.clientHeight() / newScale;

    return RectF(newLeft, newTop, newRight, newBottom);
}

bool MotionDetector::translateAccelerator(InputEvent& event)
{
    return false;
}

} // namespace webkit
} // namespace org
