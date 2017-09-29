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

package org.webkit;

import android.graphics.PointF;
import android.graphics.RectF;
import android.view.InputDevice;
import android.view.InputEvent;
import android.view.KeyEvent;
import android.view.MotionEvent;
import java.lang.Math;

public class MotionDetector {
    public static class Settings {
        public boolean disableDetection = false;
        public boolean canHandlePinchBy = false;
        public boolean mouseTouchEvents = false;
    }

    public interface Client {
        public Settings clientSettings();
        public float clientWidth();
        public float clientHeight();
        public float clientScaleFactor();
        public float clientMinimumScaleFactor();
        public float clientMaximumScaleFactor();
        public float clientContentWidth();
        public float clientContentHeight();
        public RectF clientContentViewport();

        public void handleSingleTap(float x, float y);
        public void handleSingleTapUnconfirmed(float x, float y);
        public void handleDoubleTap(float x, float y);
        public void handleLongTap(float x, float y);

        public void handleShowPressState(float x, float y);
        public void handleShowPressCancel(float x, float y);
        public void handleLongPress(float x, float y);

        public void handleScrollBegin(float x, float y);
        public void handleScrollBy(float x, float y, float deltaX, float deltaY);
        public void handleScrollEnd(float x, float y);

        public void handleFlingStart(float x, float y, int vx, int vy);
        public void handleFlingCancel();

        public void handlePinchBegin(float x, float y);
        public void handlePinchBy(float x, float y, float magnification);
        public void handlePinchTo(RectF rect);
        public void handlePinchEnd();

        public boolean handleTouchEvent(MotionEvent event);
        public boolean handleUnconfirmed(InputEvent event);
    }

    public static final int STATE_DEFAULT = 0;
    public static final int STATE_PINCH_TO_ZOOM = 1;

    public MotionDetector(Client client) {
        mClient = client;
    }

    private Client mClient;
    private Settings mClientSettings = new Settings();
    private int mState = STATE_DEFAULT;

    public boolean dispatchGenericEvent(InputEvent event) {
        if (isDisabled())
            return mClient.handleUnconfirmed(event);

        if (processStateTransitionEvent(event))
            return true;

        switch (event.getSource()) {
        case InputDevice.SOURCE_TOUCHSCREEN:
            return processTouchEvent(event);
        case InputDevice.SOURCE_MOUSE:
            return processMouseEvent(event);
        case InputDevice.SOURCE_KEYBOARD:
            return processKeyEvent(event);
        default:
            break;
        }

        return mClient.handleUnconfirmed(event);
    }

    public boolean dispatchUnhandledTouchEvent(InputEvent event) {
        if (event.getSource() != InputDevice.SOURCE_TOUCHSCREEN)
            return false;

        return processUnhandledTouchEvent(event);
    }

    public void reset() {
        mClientSettings = mClient.clientSettings();

        if (isDisabled())
            clearState();
    }

    private boolean isDisabled() {
        return mClientSettings.disableDetection;
    }

    private void clearState() {
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

    private boolean processStateTransitionEvent(InputEvent event) {
        switch (mState) {
        case STATE_DEFAULT:
            if (event.getSource() == InputDevice.SOURCE_MOUSE) {
                MotionEvent mouseEvent = (MotionEvent) event;
                if (mouseEvent.getActionMasked() == MotionEvent.ACTION_SCROLL
                        && (mouseEvent.getMetaState() & KeyEvent.META_CTRL_LEFT_ON) != 0) {
                    mState = STATE_PINCH_TO_ZOOM;
                    mClient.handlePinchBegin(mouseEvent.getX(),
                            mouseEvent.getY());
                    return false;
                }
            }
            break;
        case STATE_PINCH_TO_ZOOM:
            if (event.getSource() == InputDevice.SOURCE_KEYBOARD) {
                KeyEvent keyEvent = (KeyEvent) event;
                if (keyEvent.getAction() == KeyEvent.ACTION_UP
                        && VirtualKeyMap
                                .toVirtualKeyCode(keyEvent.getKeyCode()) == VirtualKeyMap.VK_CONTROL) {
                    mState = STATE_DEFAULT;
                    mClient.handlePinchEnd();
                    return false;
                }
            }
            break;
        default:
            break;
        }
        return false;
    }

    private float mLastX = 0;
    private float mLastY = 0;
    private boolean mIsScrolled = false;

    private boolean processTouchEvent(InputEvent event) {
        MotionEvent touchEvent = (MotionEvent) event;
        if (touchEvent.getPointerCount() > 1)
            return processUnhandledTouchEvent(event);
        return mClient.handleTouchEvent(touchEvent);
    }

    private boolean processUnhandledTouchEvent(InputEvent event) {
        MotionEvent touchEvent = (MotionEvent) event;
        if (touchEvent.getActionMasked() == MotionEvent.ACTION_DOWN) {
            mLastX = touchEvent.getX();
            mLastY = touchEvent.getY();
            mIsScrolled = false;
        } else if (touchEvent.getActionMasked() == MotionEvent.ACTION_MOVE) {
            float distanceX = mLastX - touchEvent.getX();
            float distanceY = mLastY - touchEvent.getY();

            if (!mIsScrolled
                    && Math.sqrt(distanceX * distanceX + distanceY * distanceY) > 40)
                mIsScrolled = true;

            if (mIsScrolled) {
                mClient.handleScrollBy(touchEvent.getX(), touchEvent.getY(),
                        distanceX, distanceY);
                mLastX = touchEvent.getX();
                mLastY = touchEvent.getY();
            }
        } else if (touchEvent.getActionMasked() == MotionEvent.ACTION_UP) {
            if (!mIsScrolled)
                mClient.handleSingleTap(touchEvent.getX(), touchEvent.getY());
        }

        return false;
    }

    private boolean processMouseEvent(InputEvent event) {
        MotionEvent mouseEvent = (MotionEvent) event;
        if (mouseEvent.getActionMasked() == MotionEvent.ACTION_SCROLL)
            return processWheelEvent(event);

        if (mClientSettings.mouseTouchEvents
                && (mouseEvent.getButtonState() & MotionEvent.BUTTON_PRIMARY) != 0) {
            mouseEvent.setSource(InputDevice.SOURCE_TOUCHSCREEN);
            return processTouchEvent(event);
        }

        return mClient.handleUnconfirmed(event);
    }

    private boolean processWheelEvent(InputEvent event) {
        final float scalePerWheelTick = 1.1f;

        MotionEvent wheelEvent = (MotionEvent) event;
        float scrollX = wheelEvent.getAxisValue(MotionEvent.AXIS_HSCROLL);
        float scrollY = wheelEvent.getAxisValue(MotionEvent.AXIS_VSCROLL);
        PointF scrollDelta = translateScrollBy(scrollX, scrollY);

        switch (mState) {
        case STATE_DEFAULT: {
            mClient.handleScrollBegin(wheelEvent.getX(), wheelEvent.getY());
            mClient.handleScrollBy(wheelEvent.getX(), wheelEvent.getY(),
                    -scrollDelta.x, -scrollDelta.y);
            mClient.handleScrollEnd(wheelEvent.getX(), wheelEvent.getY());
            return true;
        }
        case STATE_PINCH_TO_ZOOM: {
            float magnification = (float) Math.pow(scalePerWheelTick,
                    scrollDelta.y);
            if (mClientSettings.canHandlePinchBy)
                mClient.handlePinchBy(wheelEvent.getX(), wheelEvent.getY(),
                        magnification);
            else
                mClient.handlePinchTo(translatePinchBy(wheelEvent.getX(),
                        wheelEvent.getY(), magnification));
            return true;
        }
        default:
            break;
        }

        return mClient.handleUnconfirmed(event);
    }

    private boolean processKeyEvent(InputEvent event) {
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

    private PointF translateScrollBy(float deltaX, float deltaY) {
        final float pixelsPerLineStep = 60.f;
        float clientScaleFactor = mClient.clientScaleFactor();
        return new PointF(deltaX * pixelsPerLineStep / clientScaleFactor,
                deltaY * pixelsPerLineStep / clientScaleFactor);
    }

    private RectF translatePinchBy(float anchorX, float anchorY,
            float magnification) {
        RectF viewport = mClient.clientContentViewport();
        float oldScale = mClient.clientScaleFactor();
        float newScale = Math.max(
                mClient.clientMinimumScaleFactor(),
                Math.min(mClient.clientMaximumScaleFactor(), oldScale
                        * magnification));

        float newLeft = viewport.left + anchorX / oldScale - anchorX / newScale;
        float newTop = viewport.top + anchorY / oldScale - anchorY / newScale;
        float newRight = newLeft + mClient.clientWidth() / newScale;
        float newBottom = newTop + mClient.clientHeight() / newScale;

        return new RectF(newLeft, newTop, newRight, newBottom);
    }

    private boolean translateAccelerator(InputEvent event) {
        return false;
    }
}
