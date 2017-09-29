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

#include "WebEditableContext.h"
#include <android/view/View.h>
#include <android/view/inputmethod/BaseInputConnection.h>

namespace org {
namespace webkit {

class WebEditableInputConnection final : public BaseInputConnection {
public:
    WebEditableInputConnection(const std::shared_ptr<WebEditableContext>& editable, View& webView, bool fullEditor);
    ~WebEditableInputConnection() = default;

    bool commitText(CharSequence& text, int32_t newCursorPosition) override;
    bool deleteSurroundingText(int32_t beforeLength, int32_t afterLength) override;
    CharSequence getSelectedText(int32_t flags) override;
    bool finishComposingText() override;
    bool setComposingText(CharSequence& text, int32_t newCursorPosition) override;
    bool setComposingRegion(int32_t start, int32_t end) override;
    bool sendKeyEvent(KeyEvent& event) override;

    void updateInputState(String text, int32_t selectionStart, int32_t selectionEnd, int32_t compositionStart, int32_t compositionEnd);

private:
    void getComposingRegion();
    void getSurroundingRegion();
    void updateSelection();

    std::shared_ptr<WebEditableContext> mEditable;
    View& mWebView;
    int32_t mRegionStart { 0 };
    int32_t mRegionEnd { 0 };
};

} // namespace webkit
} // namespace org
