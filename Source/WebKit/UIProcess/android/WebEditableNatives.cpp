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
#include "WebEditableNatives.h"

#include "AWKViewNatives.h"
#include "AWKWebContentNatives.h"
#include "GraphicsUtilities.h"
#include "StringUtilities.h"
#include <WebCore/Editor.h>
#include <java/util/Natives/Vector.h>
#include <wtf/Vector.h>
#include <wtf/text/CString.h>

using namespace WebCore;
using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class WebEditablePrivate : public WebEditable::Private {
public:
    JNI::GlobalRef<AWKView> m_view;
};

static org::webkit::AWKView& view(WebEditable::Private& self)
{
    return *static_cast<WebEditablePrivate&>(self).m_view.getPtr<org::webkit::AWKView>();
}

static WebPageProxy& webPage(WebEditable::Private& self)
{
    return view(self).webContent().webPage();
}

static const EditorState& editorState(WebEditable::Private& self)
{
    return webPage(self).editorState();
}

JNI::PassLocalRef<Rect> WebEditable::editableCaretRectAtStart()
{
    return graphicsRectFromIntRect(editorState(*m_private).caretRectAtStart);
}

JNI::PassLocalRef<Rect> WebEditable::editableCaretRectAtEnd()
{
    return graphicsRectFromIntRect(editorState(*m_private).caretRectAtEnd);
}

std::string WebEditable::editableSelectionText()
{
    return stdStringFromWebCoreString(editorState(*m_private).selectedText);
}

std::string WebEditable::editableTextBeforeCaret(int32_t length)
{
    int32_t beginOffset = std::max(editorState(*m_private).selectionRangeStart - length, 0);
    String text = editorState(*m_private).editableText.substring(beginOffset, editorState(*m_private).selectionRangeStart - beginOffset);
    return stdStringFromWebCoreString(text);
}

std::string WebEditable::editableTextAfterCaret(int32_t length)
{
    int32_t endOffset = std::min(editorState(*m_private).selectionRangeEnd + length, int32_t(editorState(*m_private).editableText.length()));
    String text = editorState(*m_private).editableText.substring(editorState(*m_private).selectionRangeEnd, endOffset - editorState(*m_private).selectionRangeEnd);
    return stdStringFromWebCoreString(text);
}

bool WebEditable::setComposition(const std::string& text, JNI::PassLocalRef<Vector> clauses, int32_t selectionStart, int32_t selectionEnd, int32_t replacementStart, int32_t replacementEnd)
{
    // FIXME: Pass underlines properly.
    WTF::Vector<CompositionUnderline> underlines;
    webPage(*m_private).setComposition(String::fromUTF8(text.c_str()), underlines, selectionStart, selectionEnd, replacementStart, replacementEnd);
    return true;
}

bool WebEditable::confirmComposition(const std::string& text)
{
    webPage(*m_private).confirmComposition(String::fromUTF8(text.c_str()));
    return true;
}

bool WebEditable::finishComposition()
{
    webPage(*m_private).finishComposition();
    return true;
}

void WebEditable::cancelComposition()
{
    webPage(*m_private).cancelComposition();
}

bool WebEditable::performEditorAction(int32_t editorAction)
{
    return false;
}

bool WebEditable::sendKeyEvent(int64_t timestamp, int32_t action, int32_t modifiers, int32_t virtualKeyCode, int32_t nativeKeyCode,
    const std::string& text, const std::string& unmodifiedText, bool isAutoRepeat, bool isSystemKey, bool isKeypadKey, int64_t downTime)
{
    view(*m_private).sendKeyEvent(timestamp, action, modifiers, virtualKeyCode, nativeKeyCode, text, unmodifiedText, isAutoRepeat, isSystemKey, isKeypadKey, downTime);
    return true;
}

WebEditable* WebEditable::nativeCreate(JNI::PassLocalRef<AWKView> view)
{
    std::unique_ptr<WebEditable> ptr(new WebEditable);
    ptr->m_private.reset(new WebEditablePrivate);
    ptr->p<WebEditablePrivate>().m_view = view;
    return ptr.release();
}

} // namespace Natives
} // namespace webkit
} // namespace org
