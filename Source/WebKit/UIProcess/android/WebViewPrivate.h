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

#include "AWKWebContentConfiguration.h"
#include "ContainerView.h"
#include "MotionDetector.h"
#include "WebEditableContext.h"
#include "WebEditableInputConnection.h"
#include <android/opengl/GLSurfaceView.h>
#include <android/os/Handler.h>
#include <android/view/MotionEvent.h>
#include <android++/KeywordMacros.h>

namespace org {
namespace webkit {

class WebProcessContext;
class WebRenderContextList;
class WebView;
class WebViewContext;

class WebViewPrivate final : public ContainerView
    , public GLSurfaceView::Renderer
    , public MotionDetector::Client {
public:
    WebViewPrivate(Context&, WebView&);
    ~WebViewPrivate();

    static Context& getApplicationContext();

    const std::shared_ptr<GLSurfaceView>& getSurfaceView();

    bool isVisible();

    void addView(const std::shared_ptr<WebViewContext>&);
    void removeView(const std::shared_ptr<WebViewContext>&);
    void moveAfter(const std::shared_ptr<WebViewContext>&, const std::shared_ptr<WebViewContext>&);
    void bringChildToFront(const std::shared_ptr<WebViewContext>&);
    void sendToBack(const std::shared_ptr<WebViewContext>&);

    std::shared_ptr<WebViewContext> createView();

    const std::shared_ptr<WebViewContext>& getForegroundView();

    float getDeviceScaleFactor();

    void onPause();
    void onResume();

    bool sendGenericEvent(InputEvent&);
    bool sendUnhandledTouchEvent(InputEvent&);
    void resetMotionEvent();

    std::shared_ptr<WebEditableContext> getEditableContext();

    // View
    void invalidate() override;
    void invalidate(Rect&) override;

private:
    DECLARE_INNER_CLASS(ViewObserver);

    void deleteView(const std::shared_ptr<WebViewContext>&);
    void rebuildRenderList();
    void destroyRenderListOnMainThread();
    void onVisibilityChanged();

    // ContainerView
    void doneWithTouchEvent(bool wasEventHandled) override;
    void editorStateChanged(String text, int32_t selectionStart, int32_t selectionEnd, int32_t compositionStart, int32_t compositionEnd) override;

    // View
    bool onGenericMotionEvent(MotionEvent& event) override;
    void onWindowFocusChanged(bool hasWindowFocus);
    void onAttachedToWindow() override;
    void onDetachedFromWindow() override;
    std::shared_ptr<InputConnection> onCreateInputConnection(EditorInfo& outAttrs) override;
    void onVisibilityChanged(View* changedView, int32_t visibility) override;
    void onWindowVisibilityChanged(int32_t visibility) override;
    void onMeasure(int32_t, int32_t) override;
    void onLayout(bool changed, int32_t left, int32_t top, int32_t right, int32_t bottom) override;
    void onConfigurationChanged(Configuration&) override;

    // opengl.GLSurfaceView.Renderer
    void onSurfaceCreated(GL10 gl) override;
    void onSurfaceChanged(GL10 gl, int32_t width, int32_t height) override;
    void onDrawFrame(GL10 gl) override;

    // MotionDetector.Client
    MotionDetector::Settings clientSettings() override;
    float clientWidth() override;
    float clientHeight() override;
    float clientScaleFactor() override;
    float clientMinimumScaleFactor() override;
    float clientMaximumScaleFactor() override;
    float clientContentWidth() override;
    float clientContentHeight() override;
    RectF clientContentViewport() override;

    void handleSingleTap(float x, float y) override;
    void handleSingleTapUnconfirmed(float x, float y) override;
    void handleDoubleTap(float x, float y) override;
    void handleLongTap(float x, float y) override;

    void handleShowPressState(float x, float y) override;
    void handleShowPressCancel(float x, float y) override;
    void handleLongPress(float x, float y) override;

    void handleScrollBegin(float x, float y) override;
    void handleScrollBy(float x, float y, float deltaX, float deltaY) override;
    void handleScrollEnd(float x, float y) override;

    void handleFlingStart(float x, float y, int32_t vx, int32_t vy) override;
    void handleFlingCancel() override;

    void handlePinchBegin(float x, float y) override;
    void handlePinchTo(RectF& rect) override;
    void handlePinchBy(float x, float y, float magnification) override;
    void handlePinchEnd() override;

    bool handleTouchEvent(MotionEvent&) override;
    bool handleUnconfirmed(InputEvent&) override;

private:
    WebView& mWebView;

    std::shared_ptr<GLSurfaceView> mSurfaceView;
    int32_t mViewVisibility { View::INVISIBLE };
    int32_t mWindowVisibility { View::INVISIBLE };
    bool mIsVisible { false };
    bool mSurfaceCreatedOnce { false };

    std::shared_ptr<WebProcessContext> mProcessContext;
    std::shared_ptr<AWKWebContentConfiguration> mConfiguration;
    MotionDetector mMotionDetector;
    std::shared_ptr<Handler> mHandler;

    std::shared_ptr<WebRenderContextList> mCurrentRenderList;
    std::shared_ptr<WebRenderContextList> mPendingRenderList;
    std::vector<std::shared_ptr<WebViewContext>> mViewList;

    std::vector<MotionEvent> mTouchEvents;
    std::shared_ptr<WebEditableInputConnection> mInputConnection;
};

} // namespace webkit
} // namespace org
