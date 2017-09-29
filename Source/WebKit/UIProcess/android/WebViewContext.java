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

import android.content.res.Configuration;
import android.graphics.Rect;
import android.graphics.RectF;
import android.view.InputEvent;
import android.view.MotionEvent;

public final class WebViewContext {
    public AWKWebContent getWebContent() {
        return mWebContent;
    }

    public AWKView getView() {
        return mView;
    }

    public void scrollBegin(float x, float y) {
        mView.scrollBegin(x, y);
    }

    public boolean scrollBy(float deltaX, float deltaY) {
        return mView.scrollBy(deltaX, deltaY, true);
    }

    public boolean scrollTo(float x, float y) {
        return mView.scrollTo(x, y);
    }

    public void scrollEnd(float x, float y) {
        mView.scrollEnd(x, y);
    }

    public void pinchBegin(float x, float y) {
        mView.pinchBegin(x, y);
    }

    public void pinchTo(RectF rect) {
    }

    public void pinchBy(float x, float y, float magnification) {
        mView.adjustTransientZoom(x, y, magnification);
    }

    public void pinchEnd() {
        mView.commitTransientZoom();
    }

    public void setActive(boolean active) {
        mView.setActive(active);
    }

    public boolean isActive() {
        return mView.isActive();
    }

    public void setFocus(boolean focus) {
        mView.setFocus(focus);
    }

    public boolean isFocused() {
        return mView.isFocused();
    }

    public void setVisible(boolean visible) {
        mView.setVisible(visible);
    }

    public boolean isVisible() {
        return mView.isVisible();
    }

    public void sendSingleTap(float x, float y) {
        mView.sendSingleTap(x, y);
    }

    public void sendDoubleTap(float x, float y) {
        mView.sendDoubleTap(x, y);
    }

    public boolean sendGenericEvent(InputEvent event) {
        return false;
    }

    public boolean sendTouchEvent(MotionEvent event, int touchEventId) {
        mView.sendTouchEvent(event.getEventTime(),
                WebEventEnumerations.getWebEventType(event),
                WebEventEnumerations.getWebEventModifiers(event), event.getX(),
                event.getY(), event.getRawX(), event.getRawY(), touchEventId);
        return true;
    }

    public MotionDetector.Settings getMotionSettings() {
        return new MotionDetector.Settings();
    }

    public float getCurrentScale() {
        return mView.getPageScaleFactor();
    }

    public float getMinimumScale() {
        return 0.5f;
    }

    public float getMaximumScale() {
        return 8.0f;
    }

    public int getWidth() {
        return 0;
    }

    public int getHeight() {
        return 0;
    }

    public int getContentWidth() {
        return 0;
    }

    public int getContentHeight() {
        return 0;
    }

    public RectF getViewport() {
        return new RectF();
    }

    public void onConfigurationChanged(Configuration config) {
    }

    public void onLayout(int width, int height) {
        mView.onLayout(width, height);
    }

    public void onPause() {
        mView.onPause();
    }

    public void onResume() {
        mView.onResume();
    }

    public WebEditableContext getEditable() {
        return new WebEditableContext(mView);
    }

    public WebRenderContext getRenderer() {
        if (mRenderer == null)
            mRenderer = new WebRenderContext(mView);

        return mRenderer;
    }

    public void invalidate() {
        mView.setViewNeedsDisplay(0, 0, 0, 0);
    }

    public void invalidate(Rect rect) {
        mView.setViewNeedsDisplay(rect.left, rect.top, rect.right, rect.bottom);
    }

    private WebViewPrivate mWebView;
    private AWKWebContent mWebContent;
    private final AWKView mView;
    private WebRenderContext mRenderer;

    public WebViewContext(WebViewPrivate webView,
            AWKWebContentConfiguration configuration) {
        mWebView = webView;
        mWebContent = new AWKWebContent(null, configuration);
        mView = mWebContent.getView();
        mView.setContainerView(mWebView);
    }
}
