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

#ifndef WebEditableContext_h
#define WebEditableContext_h

#include "AWKView.h"
#include "CompositionClause.h"
#include "WebEditable.h"
#include <android/graphics/Rect.h>
#include <android/view/KeyEvent.h>

namespace org {
namespace webkit {

class WebEditableContext final {
public:
    WebEditableContext(std::shared_ptr<AWKView>&);
    ~WebEditableContext();

    Rect editableCaretRectAtStart();
    Rect editableCaretRectAtEnd();
    std::wstring editableSelectionText();
    std::wstring editableTextBeforeCaret(int32_t);
    std::wstring editableTextAfterCaret(int32_t);

    bool setComposition(const std::wstring&, const std::vector<CompositionClause>&, int32_t, int32_t, int32_t, int32_t);
    bool confirmComposition(const std::wstring&);
    bool finishComposition();
    void cancelComposition();

    bool performEditorAction(int32_t);
    bool sendKeyEvent(KeyEvent&);
    bool sendKeyPressEvent(KeyEvent&);

private:
    std::shared_ptr<WebEditable> mEditable;
};

} // namespace webkit
} // namespace org

#endif
