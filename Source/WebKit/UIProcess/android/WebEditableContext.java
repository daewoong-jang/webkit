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

package org.webkit;

import android.graphics.Rect;
import android.view.KeyEvent;
import java.util.Vector;

public final class WebEditableContext {
    public Rect editableCaretRectAtStart() {
        return mEditable.editableCaretRectAtStart();
    }

    public Rect editableCaretRectAtEnd() {
        return mEditable.editableCaretRectAtEnd();
    }

    public String editableSelectionText() {
        return mEditable.editableSelectionText();
    }

    public String editableTextBeforeCaret(int length) {
        return mEditable.editableTextBeforeCaret(length);
    }

    public String editableTextAfterCaret(int length) {
        return mEditable.editableTextAfterCaret(length);
    }

    public boolean setComposition(String text,
            Vector<CompositionClause> clauses, int selectionStart,
            int selectionEnd, int replacementStart, int replacementEnd) {
        return mEditable.setComposition(text, clauses, selectionStart,
                selectionEnd, replacementStart, replacementEnd);
    }

    public boolean confirmComposition(String text) {
        return mEditable.confirmComposition(text);
    }

    public boolean finishComposition() {
        return mEditable.finishComposition();
    }

    public void cancelComposition() {
        mEditable.cancelComposition();
    }

    public boolean performEditorAction(int editorAction) {
        return mEditable.performEditorAction(editorAction);
    }

    public boolean sendKeyEvent(KeyEvent event) {
        int action = WebEventEnumerations.EVENT_NO_TYPE;
        switch (event.getAction()) {
        case KeyEvent.ACTION_DOWN:
            action = WebEventEnumerations.EVENT_RAW_KEY_DOWN;
            break;
        case KeyEvent.ACTION_UP:
            action = WebEventEnumerations.EVENT_KEY_UP;
            break;
        default:
            assert(false);
        }
        String text = VirtualKeyMap.fromUnicode(event.getUnicodeChar());
        return mEditable.sendKeyEvent(event.getEventTime(), action,
                event.getModifiers(),
                VirtualKeyMap.toVirtualKeyCode(event.getKeyCode()),
                event.getKeyCode(), text, text, true /* isAutoRepeat */,
                event.isSystem(),
                VirtualKeyMap.isKeypadKey(event.getKeyCode()),
                event.getDownTime());
    }

    public boolean sendKeyPressEvent(KeyEvent event) {
        String text = VirtualKeyMap.fromUnicode(event.getUnicodeChar());
        return mEditable.sendKeyEvent(event.getEventTime(),
                WebEventEnumerations.EVENT_KEY_DOWN, event.getModifiers(),
                VirtualKeyMap.toVirtualKeyCode(event.getKeyCode()),
                event.getKeyCode(), text, text, true /* isAutoRepeat */,
                event.isSystem(),
                VirtualKeyMap.isKeypadKey(event.getKeyCode()),
                event.getDownTime());
    }

    private WebEditable mEditable;

    public WebEditableContext(AWKView view) {
        mEditable = new WebEditable(view);
    }
}
