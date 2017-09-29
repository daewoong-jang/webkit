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
#include "AWKView.h"

#include "AWKCallback.h"
#include "ContainerView.h"
#include <android/content/Context.h>
#include <android/view/inputmethod/BaseInputConnection.h>
#include <android/view/inputmethod/InputMethodManager.h>
#include <android++/StringConversion.h>

namespace org {
namespace webkit {
namespace Managed {

class AWKViewPrivate : public AWKView::Private {
public:
    ContainerView* mView;
    bool mContentEditable = false;
};

void AWKView::onFocusChanged(bool gainFocus)
{
}

void AWKView::setContainerView(ContainerView& view)
{
    p<AWKViewPrivate>().mView = &view;
}

void AWKView::didChangeContentsSize(int32_t width, int32_t height)
{
}

void AWKView::doneWithTouchEvent(bool wasEventHandled)
{
    p<AWKViewPrivate>().mView->doneWithTouchEvent(wasEventHandled);
}

void AWKView::startInputMethod(bool isInPasswordField)
{
    p<AWKViewPrivate>().mContentEditable = true;
    std::shared_ptr<InputMethodManager> imm = std::static_pointer_cast<InputMethodManager>(
        p<AWKViewPrivate>().mView->getContext().getSystemService(Context::INPUT_METHOD_SERVICE));
    imm->restartInput(*p<AWKViewPrivate>().mView);
    imm->showSoftInput(*p<AWKViewPrivate>().mView, 0);
}

void AWKView::endInputMethod()
{
    p<AWKViewPrivate>().mContentEditable = false;
    std::shared_ptr<InputMethodManager> imm = std::static_pointer_cast<InputMethodManager>(
        p<AWKViewPrivate>().mView->getContext().getSystemService(Context::INPUT_METHOD_SERVICE));
    imm->hideSoftInputFromWindow(p<AWKViewPrivate>().mView->getWindowToken(), 0);
}

void AWKView::enterFullScreen()
{
}

void AWKView::exitFullScreen()
{
}

void AWKView::setViewNeedsDisplay(int32_t x, int32_t y, int32_t width, int32_t height)
{
    p<AWKViewPrivate>().mView->invalidate();
}

void AWKView::editorStateChanged(const std::string& text, int32_t selectionStart, int32_t selectionEnd, int32_t compositionStart, int32_t compositionEnd)
{
    p<AWKViewPrivate>().mView->editorStateChanged(std::s2ws(text), selectionStart, selectionEnd, compositionStart, compositionEnd);
}

void AWKView::setCursor(int32_t cursorType)
{
}

void AWKView::webProcessDidExit(const std::string& url)
{
}

void AWKView::didRelaunchWebProcess()
{
}

void AWKView::INIT(const std::shared_ptr<AWKWebContent>& webContent)
{
    m_private.reset(new AWKViewPrivate);
    nativeCreate(webContent);
}

}
} // namespace webkit
} // namespace org
