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
#include "ViewController.h"

#include "AWKViewNatives.h"
#include "AWKWebContentNatives.h"
#include "StringUtilities.h"
#include "WebViewportAttributes.h"
#include <android/graphics/Natives/Point.h>
#include <android/graphics/Natives/Rect.h>

#if ENABLE(CONTEXT_MENUS)
#include "WebContextMenuProxyAndroid.h"
#endif

using namespace WebCore;
using namespace org::webkit;

namespace WebKit {

ViewController::ViewController(AWKWebContent& content)
    : m_content(content)
{
}

ViewController::~ViewController()
{
}

RefPtr<WebPopupMenuProxy> ViewController::createPopupMenuProxy(WebPageProxy& page)
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return nullptr;
    return view->createPopupMenuProxy(page);
}

#if ENABLE(CONTEXT_MENUS)
RefPtr<WebContextMenuProxy> ViewController::createContextMenuProxy(WebPageProxy& page, const ContextMenuContextData& contextMenuContextData, const UserData& userData)
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return nullptr;
    return view->createContextMenuProxy(page, contextMenuContextData, userData);
}
#endif

void ViewController::didCommitLoad()
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;
}

void ViewController::didChangeContentsSize(const IntSize& size)
{
    if (m_contentSize != size)
        m_contentSize = size;

    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;

    view->didChangeContentsSize(size.width(), size.height());
}

void ViewController::didChangeContentsPosition(const IntPoint& position)
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;
}

void ViewController::didChangeViewportAttributes(const ViewportAttributes& attributes)
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;

    ASSERT(view->getUseFixedLayout());
}

void ViewController::didRenderFrame(const IntSize& contentsSize, const IntRect& coveredRect)
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;
}

void ViewController::didCompletePageTransition()
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;
}

void ViewController::doneWithKeyEvent(const NativeWebKeyboardEvent& event, bool wasEventHandled)
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;
    return view->doneWithKeyEvent(event, wasEventHandled);
}

#if ENABLE(TOUCH_EVENTS)
void ViewController::doneWithTouchEvent(bool wasEventHandled)
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;
    return view->doneWithTouchEvent(wasEventHandled);
}
#endif

void ViewController::enterFullScreen()
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;
    return view->enterFullScreen();
}

void ViewController::exitFullScreen()
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;
    return view->exitFullScreen();
}

void ViewController::setViewNeedsDisplay(const IntRect& area)
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;
    return view->setViewNeedsDisplay(area);
}

void ViewController::editorStateChanged()
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;
    return view->editorStateChanged();
}

void ViewController::setCursor(const Cursor& cursor)
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;
    return view->setCursor(cursor);
}

void ViewController::webProcessDidExit(const String& url)
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;
    return view->webProcessDidExit(url);
}

void ViewController::didRelaunchWebProcess()
{
    JNI::LocalRef<AWKView> view = m_view.tryPromote();
    if (!view)
        return;
    return view->didRelaunchWebProcess();
}

JNI::PassLocalRef<AWKView> ViewController::getOrCreateView(AWKWebContent* webContent)
{
    if (!m_view.isExpired()) {
        JNI::LocalRef<AWKView> view = m_view.tryPromote().as<AWKView>();
        if (view)
            return view.release();
    }

    JNI::LocalRef<AWKView> view = AWKView::create(webContent).as<AWKView>();
    m_view = view;
    return view.release();
}

JNI::PassLocalRef<AWKView> ViewController::view()
{
    return m_view.tryPromote();
}

} // namespace WebKit
