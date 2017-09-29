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

#ifndef ViewController_h
#define ViewController_h

#include <WebCore/Cursor.h>
#include <WebCore/IntRect.h>
#include <WebCore/ViewportArguments.h>
#include <androidjni/WeakGlobalRef.h>

namespace org {
namespace webkit {
class AWKView;
class AWKWebContent;
}
}

namespace WebKit {

class ContextMenuContextData;
class NativeWebKeyboardEvent;
class UserData;
class WebEditable;
class WebPageProxy;
class WebPopupMenuProxy;

#if ENABLE(CONTEXT_MENUS)
class WebContextMenuProxy;
#endif

class ViewController {
    friend class org::webkit::AWKWebContent;
public:
    ~ViewController();

    RefPtr<WebPopupMenuProxy> createPopupMenuProxy(WebPageProxy&);
#if ENABLE(CONTEXT_MENUS)
    RefPtr<WebContextMenuProxy> createContextMenuProxy(WebPageProxy&, const ContextMenuContextData&, const UserData&);
#endif

    void didCommitLoad();
    void didChangeContentsSize(const WebCore::IntSize&);
    void didChangeContentsPosition(const WebCore::IntPoint&);
    void didChangeViewportAttributes(const WebCore::ViewportAttributes&);
    void didRenderFrame(const WebCore::IntSize&, const WebCore::IntRect&);
    void didCompletePageTransition();

    void doneWithKeyEvent(const NativeWebKeyboardEvent&, bool wasEventHandled);
#if ENABLE(TOUCH_EVENTS)
    void doneWithTouchEvent(bool wasEventHandled);
#endif

    void enterFullScreen();
    void exitFullScreen();

    void setViewNeedsDisplay(const WebCore::IntRect&);

    void editorStateChanged();

    void setCursor(const WebCore::Cursor&);

    void webProcessDidExit(const String&);
    void didRelaunchWebProcess();

    const WebCore::IntSize& getContentSize() const { return m_contentSize; }

private:
    ViewController(org::webkit::AWKWebContent&);

    JNI::PassLocalRef<org::webkit::AWKView> getOrCreateView(org::webkit::AWKWebContent*);
    JNI::PassLocalRef<org::webkit::AWKView> view();

    JNI::WeakGlobalRef<org::webkit::AWKView> m_view;
    org::webkit::AWKWebContent& m_content;

    WebCore::IntSize m_contentSize;
};

} // namespace WebKit

#endif // ViewController_h
