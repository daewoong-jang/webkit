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

#pragma once

#include <android/graphics/PointF.h>
#include <android/graphics/RectF.h>
#include <android/view/MotionEvent.h>

namespace org {
namespace webkit {

class MotionDetector {
public:
    struct Settings {
        bool disableDetection { false };
        bool canHandlePinchBy { false };
        bool mouseTouchEvents { false };
    };

    class Client {
    public:
        virtual Settings clientSettings() = 0;
        virtual float clientWidth() = 0;
        virtual float clientHeight() = 0;
        virtual float clientScaleFactor() = 0;
        virtual float clientMinimumScaleFactor() = 0;
        virtual float clientMaximumScaleFactor() = 0;
        virtual float clientContentWidth() = 0;
        virtual float clientContentHeight() = 0;
        virtual RectF clientContentViewport() = 0;

        virtual void handleSingleTap(float x, float y) = 0;
        virtual void handleSingleTapUnconfirmed(float x, float y) = 0;
        virtual void handleDoubleTap(float x, float y) = 0;
        virtual void handleLongTap(float x, float y) = 0;

        virtual void handleShowPressState(float x, float y) = 0;
        virtual void handleShowPressCancel(float x, float y) = 0;
        virtual void handleLongPress(float x, float y) = 0;

        virtual void handleScrollBegin(float x, float y) = 0;
        virtual void handleScrollBy(float x, float y, float deltaX, float deltaY) = 0;
        virtual void handleScrollEnd(float x, float y) = 0;

        virtual void handleFlingStart(float x, float y, int32_t vx, int32_t vy) = 0;
        virtual void handleFlingCancel() = 0;

        virtual void handlePinchBegin(float x, float y) = 0;
        virtual void handlePinchBy(float x, float y, float magnification) = 0;
        virtual void handlePinchTo(RectF& rect) = 0;
        virtual void handlePinchEnd() = 0;

        virtual bool handleTouchEvent(MotionEvent&) = 0;
        virtual bool handleUnconfirmed(InputEvent&) = 0;
    };

    static const int32_t STATE_DEFAULT = 0;
    static const int32_t STATE_PINCH_TO_ZOOM = 1;

    MotionDetector(Client&);
    ~MotionDetector();

    bool dispatchGenericEvent(InputEvent&);
    bool dispatchUnhandledTouchEvent(InputEvent&);

    void reset();

private:
    bool isDisabled() { return mClientSettings.disableDetection; }

    void clearState();

    bool processStateTransitionEvent(InputEvent&);

    bool processTouchEvent(InputEvent&);
    bool processUnhandledTouchEvent(InputEvent&);
    bool processMouseEvent(InputEvent&);
    bool processWheelEvent(InputEvent&);
    bool processKeyEvent(InputEvent&);

    PointF translateScrollBy(float, float);
    RectF translatePinchBy(float, float, float);
    bool translateAccelerator(InputEvent&);

    Client& mClient;
    Settings mClientSettings;
    int32_t mState;
    float mLastX = 0;
    float mLastY = 0;
    bool mIsScrolled = false;
};

} // namespace webkit
} // namespace org
