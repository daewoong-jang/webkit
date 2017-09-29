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

import android.content.Context;
import android.view.inputmethod.InputMethodManager;

@NativeNamespace("org.webkit")
@NativeExportMacro("ANDROID_EXTERN")
public class AWKView {
    @AbstractMethod
    public native void scrollBegin(float x, float y);

    @AbstractMethod
    public native boolean scrollBy(float deltaX, float deltaY,
            boolean isOverScrollAllowed);

    @AbstractMethod
    public native boolean scrollTo(float x, float y);

    @AbstractMethod
    public native void scrollEnd(float x, float y);

    @AbstractMethod
    public native void pinchBegin(float x, float y);

    @AbstractMethod
    public native void adjustTransientZoom(float x, float y, float magnification);

    @AbstractMethod
    public native void commitTransientZoom();

    @AbstractMethod
    public native boolean isActive();

    @AbstractMethod
    public native void setActive(boolean active);

    @AbstractMethod
    public native boolean isFocused();

    @AbstractMethod
    public native void setFocus(boolean focus);

    @AbstractMethod
    public native boolean isVisible();

    @AbstractMethod
    public native void setVisible(boolean visible);

    @AbstractMethod
    public native void setPageScaleFactor(float scale, int x, int y);

    @AbstractMethod
    public native float getPageScaleFactor();

    @AbstractMethod
    public native void setIntrinsicDeviceScaleFactor(float scale);

    @AbstractMethod
    public native float getIntrinsicDeviceScaleFactor();

    @AbstractMethod
    public native void setOverrideDeviceScaleFactor(
            float overrideDeviceScaleFactor);

    @AbstractMethod
    public native float getOverrideDeviceScaleFactor();

    @AbstractMethod
    public native void setUseFixedLayout(boolean use);

    @AbstractMethod
    public native boolean getUseFixedLayout();

    @AbstractMethod
    public native void setBackgroundColor(int color);

    @AbstractMethod
    public native int getBackgroundColor();

    @AbstractMethod
    public native boolean requestExitFullScreen();

    @AbstractMethod
    public native void sendSingleTap(float x, float y);

    @AbstractMethod
    public native void sendSingleTapUnconfirmed(float x, float y);

    @AbstractMethod
    public native void sendDoubleTap(float x, float y);

    @AbstractMethod
    public native void sendLongTap(float x, float y);

    @AbstractMethod
    public native void sendShowPressState(float x, float y);

    @AbstractMethod
    public native void sendShowPressCancel(float x, float y);

    @AbstractMethod
    public native void sendLongPress(float x, float y);

    @AbstractMethod
    public native void sendTouchEvent(long timestamp, int action,
            int modifiers, float x, float y, float globalX, float globalY,
            int id);

    @AbstractMethod
    public native void sendMouseEvent(long timestamp, int action,
            int modifiers, float x, float y, float globalX, float globalY,
            int clickCount);

    @AbstractMethod
    public native void sendWheelEvent(long timestamp, int action,
            int modifiers, float deltaX, float deltaY, float x, float y,
            float globalX, float globalY);

    @AbstractMethod
    public native void sendKeyEvent(long timestamp, int action, int modifiers,
            int windowsVirtualKeyCode, int nativeVirtualKeyCode, String text,
            String unmodifiedText, boolean isAutoRepeat, boolean isSystemKey,
            boolean isKeypadKey, long downTime);

    @AbstractMethod
    public native void onLayout(int width, int height);

    @AbstractMethod
    public native void onPause();

    @AbstractMethod
    public native void onResume();

    private ContainerView mView = null;
    private boolean mContentEditable = false;

    @API
    protected void onFocusChanged(boolean gainFocus) {
    }

    @API
    @CallByReference
    public void setContainerView(org.webkit.ContainerView view) {
        mView = view;
    }

    @API
    @CalledByNative
    private void didChangeContentsSize(int width, int height) {
    }

    @API
    @CalledByNative
    private void doneWithTouchEvent(boolean wasEventHandled) {
        mView.doneWithTouchEvent(wasEventHandled);
    }

    @API
    @CalledByNative
    private void startInputMethod(boolean isInPasswordField) {
        mContentEditable = true;
        InputMethodManager imm = (InputMethodManager) mView.getContext()
                .getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.restartInput(mView);
        imm.showSoftInput(mView, 0);
    }

    @API
    @CalledByNative
    private void endInputMethod() {
        mContentEditable = false;
        InputMethodManager imm = (InputMethodManager) mView.getContext()
                .getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(mView.getWindowToken(), 0);
    }

    @API
    @CalledByNative
    private void enterFullScreen() {
    }

    @API
    @CalledByNative
    private void exitFullScreen() {
    }

    @API
    @CalledByNative
    protected void setViewNeedsDisplay(int x, int y, int width, int height) {
        mView.invalidate();
    }

    @API
    @CalledByNative
    private void editorStateChanged(String text, int selectionStart,
            int selectionEnd, int compositionStart, int compositionEnd) {
        mView.editorStateChanged(text, selectionStart, selectionEnd,
                compositionStart, compositionEnd);
    }

    @API
    @CalledByNative
    private void setCursor(int cursorType) {
    }

    @API
    @CalledByNative
    private void webProcessDidExit(String url) {
    }

    @API
    @CalledByNative
    private void didRelaunchWebProcess() {
    }

    @CalledByNative
    public AWKView(AWKWebContent webContent) {
        nativeCreate(webContent);
    }

    @NativeObjectField
    private long mNativePtr;

    @Override
    protected void finalize() throws Throwable {
        try {
            nativeDestroy();
        } finally {
            super.finalize();
        }
    }

    @NativeConstructor
    private native void nativeCreate(AWKWebContent webContent);

    @NativeDestructor
    private native void nativeDestroy();
}
