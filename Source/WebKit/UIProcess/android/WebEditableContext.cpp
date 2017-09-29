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
#include "WebEditableContext.h"

#include "VirtualKeyMap.h"
#include "WebEditable.h"
#include "WebEventEnumerations.h"
#include <android++/StringConversion.h>

namespace org {
namespace webkit {

WebEditableContext::WebEditableContext(std::shared_ptr<AWKView>& view)
    : mEditable(WebEditable::create(view))
{
}

WebEditableContext::~WebEditableContext()
{
}

Rect WebEditableContext::editableCaretRectAtStart()
{
    return Rect(*mEditable->editableCaretRectAtStart());
}

Rect WebEditableContext::editableCaretRectAtEnd()
{
    return Rect(*mEditable->editableCaretRectAtEnd());
}

std::wstring WebEditableContext::editableSelectionText()
{
    return std::s2ws(mEditable->editableSelectionText());
}

std::wstring WebEditableContext::editableTextBeforeCaret(int32_t length)
{
    return std::s2ws(mEditable->editableTextBeforeCaret(length));
}

std::wstring WebEditableContext::editableTextAfterCaret(int32_t length)
{
    return std::s2ws(mEditable->editableTextAfterCaret(length));
}

bool WebEditableContext::setComposition(const std::wstring& text, const std::vector<CompositionClause>& clauses, int32_t selectionStart, int32_t selectionEnd, int32_t replacementStart, int32_t replacementEnd)
{
    return mEditable->setComposition(std::ws2s(text), nullptr, selectionStart, selectionEnd, replacementStart, replacementEnd);
}

bool WebEditableContext::confirmComposition(const std::wstring& text)
{
    return mEditable->confirmComposition(std::ws2s(text));
}

bool WebEditableContext::finishComposition()
{
    return mEditable->finishComposition();
}

void WebEditableContext::cancelComposition()
{
    mEditable->cancelComposition();
}

bool WebEditableContext::performEditorAction(int32_t editorAction)
{
    return mEditable->performEditorAction(editorAction);
}

bool WebEditableContext::sendKeyEvent(KeyEvent& event)
{
    int32_t action = WebEventEnumerations::EVENT_NO_TYPE;
    switch (event.getAction()) {
    case KeyEvent::ACTION_DOWN:
        action = WebEventEnumerations::EVENT_RAW_KEY_DOWN;
        break;
    case KeyEvent::ACTION_UP:
        action = WebEventEnumerations::EVENT_KEY_UP;
        break;
    default:
        ASSERT_NOT_REACHED();
    }
    std::string text = std::ws2s(VirtualKeyMap::fromUnicode(event.getUnicodeChar()));
    return mEditable->sendKeyEvent(event.getEventTime().count(), action, event.getModifiers(),
        VirtualKeyMap::toVirtualKeyCode(event.getKeyCode()),
        event.getKeyCode(), text, text,
        true /* isAutoRepeat */, event.isSystem(), VirtualKeyMap::isKeypadKey(event.getKeyCode()),
        event.getDownTime().count());
}

bool WebEditableContext::sendKeyPressEvent(KeyEvent& event)
{
    std::string text = std::ws2s(VirtualKeyMap::fromUnicode(event.getUnicodeChar()));
    return mEditable->sendKeyEvent(event.getEventTime().count(), WebEventEnumerations::EVENT_KEY_DOWN, event.getModifiers(),
        VirtualKeyMap::toVirtualKeyCode(event.getKeyCode()),
        event.getKeyCode(), text, text,
        true /* isAutoRepeat */, event.isSystem(), VirtualKeyMap::isKeypadKey(event.getKeyCode()),
        event.getDownTime().count());
}

} // namespace webkit
} // namespace org
