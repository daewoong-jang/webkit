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
#include "WebViewContext.h"

#include "AWKCallback.h"
#include "WebEditableContext.h"
#include "WebEventEnumerations.h"
#include "WebRenderContext.h"
#include "WebViewPrivate.h"

namespace org {
namespace webkit {

WebViewContext::WebViewContext(WebViewPrivate& webView, const std::shared_ptr<AWKWebContentConfiguration>& configuration)
    : mWebView(webView)
    , mWebContent(AWKWebContent::create(nullptr, configuration))
    , mView(mWebContent->getView())
{
    mView->setContainerView(mWebView);
}

WebViewContext::~WebViewContext()
{
}

std::passed_ptr<AWKWebContent> WebViewContext::getWebContent()
{
    return mWebContent;
}

std::passed_ptr<AWKView> WebViewContext::getView()
{
    return mView;
}

void WebViewContext::scrollBegin(float x, float y)
{
    return mView->scrollBegin(x, y);
}

bool WebViewContext::scrollBy(float deltaX, float deltaY)
{
    return mView->scrollBy(deltaX, deltaY, true);
}

bool WebViewContext::scrollTo(float x, float y)
{
    return false;
}

void WebViewContext::scrollEnd(float x, float y)
{
    return mView->scrollEnd(x, y);
}

void WebViewContext::pinchBegin(float x, float y)
{
    mView->pinchBegin(x, y);
}

void WebViewContext::pinchTo(RectF& rect)
{
}

void WebViewContext::pinchBy(float x, float y, float magnification)
{
    mView->adjustTransientZoom(x, y, magnification);
}

void WebViewContext::pinchEnd()
{
    mView->commitTransientZoom();
}

void WebViewContext::setActive(bool active)
{
    mView->setActive(active);
}

bool WebViewContext::isActive()
{
    return mView->isActive();
}

void WebViewContext::setFocus(bool focus)
{
    mView->setFocus(focus);
}

bool WebViewContext::isFocused()
{
    return mView->isFocused();
}

void WebViewContext::setVisible(bool visible)
{
    mView->setVisible(visible);
}

bool WebViewContext::isVisible()
{
    return mView->isVisible();
}

void WebViewContext::sendSingleTap(float x, float y)
{
    mView->sendSingleTap(x, y);
}

void WebViewContext::sendDoubleTap(float x, float y)
{
    mView->sendDoubleTap(x, y);
}

bool WebViewContext::sendGenericEvent(InputEvent& event)
{
    return false;
}

bool WebViewContext::sendTouchEvent(MotionEvent& event, int32_t touchEventId)
{
    mView->sendTouchEvent(event.getEventTime().count(),
            WebEventEnumerations::getWebEventType(event),
            WebEventEnumerations::getWebEventModifiers(event), event.getX(),
            event.getY(), event.getRawX(), event.getRawY(), touchEventId);
    return true;
}

MotionDetector::Settings WebViewContext::getMotionSettings()
{
    MotionDetector::Settings settings;
    settings.mouseTouchEvents = true;
    return settings;
}

float WebViewContext::getCurrentScale()
{
    return mView->getPageScaleFactor();
}

float WebViewContext::getMinimumScale()
{
    return 0.5f;
}

float WebViewContext::getMaximumScale()
{
    return 8.0f;
}

int32_t WebViewContext::getWidth()
{
    return 0;
}

int32_t WebViewContext::getHeight()
{
    return 0;
}

int32_t WebViewContext::getContentWidth()
{
    return 0;
}

int32_t WebViewContext::getContentHeight()
{
    return 0;
}

RectF WebViewContext::getViewport()
{
    return RectF();
}

void WebViewContext::onConfigurationChanged(Configuration& config)
{
}

void WebViewContext::onLayout(int32_t width, int32_t height)
{
    mView->onLayout(width, height);
}

void WebViewContext::onPause()
{
    mView->onPause();
}

void WebViewContext::onResume()
{
    mView->onResume();
}

std::shared_ptr<WebEditableContext> WebViewContext::getEditable()
{
    return std::make_shared<WebEditableContext>(mView);
}

std::shared_ptr<WebRenderContext> WebViewContext::getRenderer()
{
    if (!mRenderer)
        mRenderer = std::make_shared<WebRenderContext>(mView);

    return mRenderer;
}

void WebViewContext::invalidate()
{
    mView->setViewNeedsDisplay(0, 0, 0, 0);
}

void WebViewContext::invalidate(Rect& rect)
{
    mView->setViewNeedsDisplay(rect.left, rect.top, rect.right, rect.bottom);
}

} // namespace webkit
} // namespace org
