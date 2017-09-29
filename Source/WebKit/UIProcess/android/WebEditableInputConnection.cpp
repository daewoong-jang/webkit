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
#include "WebEditableInputConnection.h"

#include <android/text/Selection.h>
#include <android/view/inputmethod/InputMethodManager.h>
#include <algorithm>

namespace org {
namespace webkit {

WebEditableInputConnection::WebEditableInputConnection(const std::shared_ptr<WebEditableContext>& editable, View& webView, bool fullEditor)
    : BaseInputConnection(webView, fullEditor)
    , mEditable(editable)
    , mWebView(webView)
{
}

bool WebEditableInputConnection::commitText(CharSequence& text, int32_t newCursorPosition)
{
    getComposingRegion();
    BaseInputConnection::commitText(text, newCursorPosition);
    mEditable->confirmComposition(text);
    return true;
}

bool WebEditableInputConnection::deleteSurroundingText(int32_t beforeLength, int32_t afterLength)
{
    if (!getEditable())
        return false;

    getSurroundingRegion();
    BaseInputConnection::deleteSurroundingText(beforeLength, afterLength);
    mEditable->setComposition(L"", {}, 0, 0, mRegionStart, mRegionEnd);
    return true;
}

CharSequence WebEditableInputConnection::getSelectedText(int32_t flags)
{
    return BaseInputConnection::getSelectedText(flags);
}

bool WebEditableInputConnection::finishComposingText()
{
    BaseInputConnection::finishComposingText();
    return mEditable->finishComposition();
}

bool WebEditableInputConnection::setComposingText(CharSequence& text, int32_t newCursorPosition)
{
    getComposingRegion();
    mEditable->setComposition(text, {}, 0, 0, mRegionStart, mRegionEnd);
    return true;
}

bool WebEditableInputConnection::setComposingRegion(int32_t start, int32_t end)
{
    if (start == end)
        removeComposingSpans(*getEditable());
    else
        BaseInputConnection::setComposingRegion(start, end);
    updateSelection();
    return true;
}

bool WebEditableInputConnection::sendKeyEvent(KeyEvent& event)
{
    if (event.getAction() == KeyEvent::ACTION_UP) {
        if (event.getKeyCode() == KeyEvent::KEYCODE_DEL) {
            BaseInputConnection::deleteSurroundingText(1, 0);
        } else if (event.getKeyCode() == KeyEvent::KEYCODE_FORWARD_DEL) {
            BaseInputConnection::deleteSurroundingText(0, 1);
        } else {
            int32_t unicodeChar = event.getUnicodeChar();
            if (unicodeChar != 0) {
                auto& editable = getEditable();
                int32_t selectionStart = Selection::getSelectionStart(*editable);
                int32_t selectionEnd = Selection::getSelectionEnd(*editable);
                if (selectionStart > selectionEnd) {
                    int32_t temp = selectionStart;
                    selectionStart = selectionEnd;
                    selectionEnd = temp;
                }
                editable->replace(selectionStart, selectionEnd,
                        String(1, static_cast<wchar_t>(unicodeChar)));
            }
        }
    }

    if (event.getAction() == KeyEvent::ACTION_DOWN
            && event.getUnicodeChar() != 0) {
        mEditable->sendKeyEvent(event);
        mEditable->sendKeyPressEvent(event);
    } else {
        mEditable->sendKeyEvent(event);
    }
    return true;
}

void WebEditableInputConnection::updateInputState(String text, int32_t selectionStart, int32_t selectionEnd, int32_t compositionStart, int32_t compositionEnd)
{
    // Non-breaking spaces can cause the IME to get confused. Replace with
    // normal spaces.
    std::replace(text.begin(), text.end(), L'\u00A0', L' ');

    auto& editable = getEditable();

    int32_t currentSelectionStart = Selection::getSelectionStart(*editable);
    int32_t currentSelectionEnd = Selection::getSelectionEnd(*editable);
    int32_t currentCompositionStart = getComposingSpanStart(*editable);
    int32_t currentCompositionEnd = getComposingSpanEnd(*editable);
    String currentText = *editable;

    selectionStart = std::min(selectionStart, (int32_t)text.length());
    selectionEnd = std::min(selectionEnd, (int32_t)text.length());
    compositionStart = std::min(compositionStart, (int32_t)text.length());
    compositionEnd = std::min(compositionEnd, (int32_t)text.length());

    bool textUnchanged = currentText == text;

    if (!textUnchanged)
        editable->replace(0, (**editable).length(), text);

    if (currentSelectionStart == selectionStart
            && currentSelectionEnd == selectionEnd
            && currentCompositionStart == compositionStart
            && currentCompositionEnd == compositionEnd)
        // Nothing has changed; don't need to do anything
        return;

    Selection::setSelection(*editable, selectionStart, selectionEnd);

    if (compositionStart == compositionEnd)
        removeComposingSpans(*editable);
    else
        BaseInputConnection::setComposingRegion(compositionStart, compositionEnd);

    updateSelection();
}

void WebEditableInputConnection::getComposingRegion()
{
    auto& editable = getEditable();
    int32_t start = getComposingSpanStart(*editable);
    int32_t end = getComposingSpanEnd(*editable);
    if (start < 0 || end < 0) {
        start = Selection::getSelectionStart(*editable);
        end = Selection::getSelectionEnd(*editable);
    }
    if (end < start) {
        int32_t temp = end;
        end = start;
        start = temp;
    }
    mRegionStart = start;
    mRegionEnd = end;
}

void WebEditableInputConnection::getSurroundingRegion()
{
    // This code is from BaseInputConnection#deleteSurroundText.
    auto& editable = getEditable();
    int32_t start = Selection::getSelectionStart(*editable);
    int32_t end = Selection::getSelectionEnd(*editable);

    if (start > end) {
        int32_t temp = start;
        start = end;
        end = temp;
    }

    int32_t cstart = getComposingSpanStart(*editable);
    int32_t cend = getComposingSpanEnd(*editable);
    if (cend < cstart) {
        int32_t temp = cstart;
        cstart = cend;
        cend = temp;
    }
    if (cstart != -1 && cend != -1) {
        if (cstart < start)
            start = cstart;
        if (cend > end)
            end = cend;
    }
    mRegionStart = start;
    mRegionEnd = end;
}

void WebEditableInputConnection::updateSelection()
{
    std::shared_ptr<InputMethodManager> imm = std::static_pointer_cast<InputMethodManager>(
        mWebView.getContext().getSystemService(Context::INPUT_METHOD_SERVICE));
    if (!imm)
        return;

    auto& editable = getEditable();
    int32_t selectionStart = Selection::getSelectionStart(*editable);
    int32_t selectionEnd = Selection::getSelectionEnd(*editable);
    int32_t composingStart = getComposingSpanStart(*editable);
    int32_t composingEnd = getComposingSpanEnd(*editable);
    imm->updateSelection(mWebView, selectionStart, selectionEnd,
            composingStart, composingEnd);
}

} // namespace webkit
} // namespace org
