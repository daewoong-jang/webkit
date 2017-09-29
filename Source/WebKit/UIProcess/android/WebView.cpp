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
#include "WebView.h"

#include "AWKURL.h"
#include "AWKURLRequest.h"
#include "WebViewContext.h"
#include <android++/StringConversion.h>

namespace org {
namespace webkit {

WebView::WebView(Context& context)
    : FrameLayout(context)
    , mPrivate(new WebViewPrivate(context, *this))
    , mSurfaceView(mPrivate->getSurfaceView())
{
    addView(mSurfaceView);
    addView(mPrivate);
    mPrivate->createView();
}

WebView::~WebView()
{
    mPrivate.reset();
    mSurfaceView.reset();
}

bool WebView::canGoBack()
{
    return mPrivate->getForegroundView()->getWebContent()->canGoBack();
}

bool WebView::canGoForward()
{
    return mPrivate->getForegroundView()->getWebContent()->canGoForward();
}

void WebView::goBack()
{
    mPrivate->getForegroundView()->getWebContent()->goBack();
}

void WebView::goForward()
{
    mPrivate->getForegroundView()->getWebContent()->goForward();
}

void WebView::loadUrl(StringRef url)
{
    auto urlObject = AWKURL::create(std::ws2s(url));
    auto request = AWKURLRequest::create(urlObject);
    mPrivate->getForegroundView()->getWebContent()->loadRequest(request);
}

void WebView::onPause()
{
    mPrivate->onPause();
}

void WebView::onResume()
{
    mPrivate->onResume();
}

void WebView::invalidate()
{
    if (!mPrivate->isVisible())
        return;

    mSurfaceView->requestRender();
}

void WebView::invalidate(Rect&)
{
    invalidate();
}

} // namespace webkit
} // namespace org
