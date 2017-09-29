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

#ifndef WebViewContext_h
#define WebViewContext_h

#include "AWKView.h"
#include "AWKWebContentConfiguration.h"
#include "AWKWebContent.h"
#include "MotionDetector.h"
#include <android/content/res/Configuration.h>
#include <android/view/InputEvent.h>

namespace org {
namespace webkit {

class WebEditableContext;
class WebRenderContext;
class WebViewPrivate;

class WebViewContext final {
    class DoneWithTouchEventCallback;
    friend class DoneWithTouchEventCallback;
public:
    WebViewContext(WebViewPrivate&, const std::shared_ptr<AWKWebContentConfiguration>&);
    ~WebViewContext();

    std::passed_ptr<AWKWebContent> getWebContent();
    std::passed_ptr<AWKView> getView();

    void scrollBegin(float x, float y);
    bool scrollBy(float deltaX, float deltaY);
    bool scrollTo(float x, float y);
    void scrollEnd(float x, float y);

    void pinchBegin(float x, float y);
    void pinchTo(RectF& rect);
    void pinchBy(float x, float y, float magnification);
    void pinchEnd();

    void setActive(bool);
    bool isActive();

    void setFocus(bool);
    bool isFocused();

    void setVisible(bool);
    bool isVisible();

    void sendSingleTap(float x, float y);
    void sendDoubleTap(float x, float y);
    bool sendGenericEvent(InputEvent&);
    bool sendTouchEvent(MotionEvent&, int32_t);
    MotionDetector::Settings getMotionSettings();

    float getCurrentScale();
    float getMinimumScale();
    float getMaximumScale();

    int32_t getWidth();
    int32_t getHeight();
    int32_t getContentWidth();
    int32_t getContentHeight();
    RectF getViewport();

    void onConfigurationChanged(Configuration&);
    void onLayout(int32_t, int32_t);

    void onPause();
    void onResume();

    std::shared_ptr<WebEditableContext> getEditable();
    std::shared_ptr<WebRenderContext> getRenderer();

    void invalidate();
    void invalidate(Rect&);

private:
    WebViewPrivate& mWebView;
    std::shared_ptr<AWKWebContent> mWebContent;
    std::shared_ptr<AWKView> mView;
    std::shared_ptr<WebRenderContext> mRenderer;
};

} // namespace webkit
} // namespace org

#endif
