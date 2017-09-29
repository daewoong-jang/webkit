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

import android.content.Context;
import android.text.Editable;
import android.text.Selection;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.InputMethodManager;
import static org.webkit.LogHelper.*;

public final class WebEditableInputConnection extends BaseInputConnection {
    private static final boolean DEBUG = false;

    private final WebEditableContext mEditable;
    private final View mWebView;

    public WebEditableInputConnection(WebEditableContext editable,
            View webView, boolean fullEditor) {
        super(webView, fullEditor);

        mEditable = editable;
        mWebView = webView;
    }

    @Override
    public boolean commitText(CharSequence text, int newCursorPosition) {
        if (DEBUG)
            LOGD("commitText(" + text + ", " + newCursorPosition
                    + ") editable=" + getEditable());

        getComposingRegion();
        super.commitText(text, newCursorPosition);
        mEditable.confirmComposition(text.toString());
        return true;
    }

    @Override
    public boolean deleteSurroundingText(int beforeLength, int afterLength) {
        if (DEBUG)
            LOGD("deleteSurroundingText(" + beforeLength + ", " + afterLength
                    + ") editable=" + getEditable());

        if (getEditable() == null)
            return false;

        getSurroundingRegion();
        super.deleteSurroundingText(beforeLength, afterLength);
        mEditable.setComposition("", null, 0, 0, mRegionStart, mRegionEnd);
        return true;
    }

    @Override
    public boolean setSelection(int start, int end) {
        if (DEBUG)
            LOGD("setSelection(" + start + ", " + end + ") editable="
                    + getEditable());

        if (start < 0 || end < 0)
            return true;

        super.setSelection(start, end);
        updateSelection();
        return true;
    }

    @Override
    public CharSequence getSelectedText(int flags) {
        if (DEBUG)
            LOGD("getSelectedText(" + flags + ") editable=" + getEditable());

        return super.getSelectedText(flags);
    }

    @Override
    public boolean finishComposingText() {
        if (DEBUG)
            LOGD("finishComposingText() editable=" + getEditable());

        super.finishComposingText();
        return mEditable.finishComposition();
    }

    @Override
    public boolean setComposingText(CharSequence text, int newCursorPosition) {
        if (DEBUG)
            LOGD("setComposingText(" + text + ", " + newCursorPosition
                    + ") editable=" + getEditable());

        getComposingRegion();
        mEditable.setComposition(text.toString(), null, 0, 0, mRegionStart,
                mRegionEnd);
        return true;
    }

    @Override
    public boolean setComposingRegion(int start, int end) {
        if (DEBUG)
            LOGD("setComposingRegion(" + start + ", " + end + ") editable="
                    + getEditable());

        if (start == end)
            removeComposingSpans(getEditable());
        else
            super.setComposingRegion(start, end);
        updateSelection();
        return true;
    }

    @Override
    public boolean sendKeyEvent(KeyEvent event) {
        if (DEBUG)
            LOGD("sendKeyEvent(" + event + ") editable=" + getEditable());

        if (event.getAction() == KeyEvent.ACTION_UP) {
            if (event.getKeyCode() == KeyEvent.KEYCODE_DEL) {
                super.deleteSurroundingText(1, 0);
            } else if (event.getKeyCode() == KeyEvent.KEYCODE_FORWARD_DEL) {
                super.deleteSurroundingText(0, 1);
            } else {
                int unicodeChar = event.getUnicodeChar();
                if (unicodeChar != 0) {
                    Editable editable = getEditable();
                    int selectionStart = Selection.getSelectionStart(editable);
                    int selectionEnd = Selection.getSelectionEnd(editable);
                    if (selectionStart > selectionEnd) {
                        int temp = selectionStart;
                        selectionStart = selectionEnd;
                        selectionEnd = temp;
                    }
                    editable.replace(selectionStart, selectionEnd,
                            Character.toString((char) unicodeChar));
                }
            }
        }

        if (event.getAction() == KeyEvent.ACTION_DOWN
                && event.getUnicodeChar() != 0) {
            mEditable.sendKeyEvent(event);
            mEditable.sendKeyPressEvent(event);
        } else {
            mEditable.sendKeyEvent(event);
        }
        return true;
    }

    protected void updateInputState(String text, int selectionStart,
            int selectionEnd, int compositionStart, int compositionEnd) {
        if (DEBUG)
            LOGD("updateInputState (" + text + ", " + selectionStart + ", "
                    + selectionEnd + ", " + compositionStart + ", "
                    + compositionEnd + ") editable=" + getEditable());

        // Non-breaking spaces can cause the IME to get confused. Replace with
        // normal spaces.
        text = text.replace('\u00A0', ' ');

        Editable editable = getEditable();

        int currentSelectionStart = Selection.getSelectionStart(editable);
        int currentSelectionEnd = Selection.getSelectionEnd(editable);
        int currentCompositionStart = getComposingSpanStart(editable);
        int currentCompositionEnd = getComposingSpanEnd(editable);
        String currentText = editable.toString();

        selectionStart = Math.min(selectionStart, text.length());
        selectionEnd = Math.min(selectionEnd, text.length());
        compositionStart = Math.min(compositionStart, text.length());
        compositionEnd = Math.min(compositionEnd, text.length());

        boolean textUnchanged = currentText.equals(text);

        if (!textUnchanged)
            editable.replace(0, editable.length(), text);

        if (currentSelectionStart == selectionStart
                && currentSelectionEnd == selectionEnd
                && currentCompositionStart == compositionStart
                && currentCompositionEnd == compositionEnd)
            // Nothing has changed; don't need to do anything
            return;

        Selection.setSelection(editable, selectionStart, selectionEnd);

        if (compositionStart == compositionEnd)
            removeComposingSpans(editable);
        else
            super.setComposingRegion(compositionStart, compositionEnd);

        updateSelection();
    }

    private int mRegionStart = 0;
    private int mRegionEnd = 0;

    private void getComposingRegion() {
        Editable editable = getEditable();
        int start = getComposingSpanStart(editable);
        int end = getComposingSpanEnd(editable);
        if (start < 0 || end < 0) {
            start = Selection.getSelectionStart(editable);
            end = Selection.getSelectionEnd(editable);
        }
        if (end < start) {
            int temp = end;
            end = start;
            start = temp;
        }
        mRegionStart = start;
        mRegionEnd = end;
    }

    private void getSurroundingRegion() {
        // This code is from BaseInputConnection#deleteSurroundText.
        Editable editable = getEditable();
        int start = Selection.getSelectionStart(editable);
        int end = Selection.getSelectionEnd(editable);

        if (start > end) {
            int temp = start;
            start = end;
            end = temp;
        }

        int cstart = getComposingSpanStart(editable);
        int cend = getComposingSpanEnd(editable);
        if (cend < cstart) {
            int temp = cstart;
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

    private void updateSelection() {
        InputMethodManager imm = (InputMethodManager) mWebView.getContext()
                .getSystemService(Context.INPUT_METHOD_SERVICE);
        if (imm == null)
            return;

        Editable editable = getEditable();
        int selectionStart = Selection.getSelectionStart(editable);
        int selectionEnd = Selection.getSelectionEnd(editable);
        int composingStart = getComposingSpanStart(editable);
        int composingEnd = getComposingSpanEnd(editable);
        imm.updateSelection(mWebView, selectionStart, selectionEnd,
                composingStart, composingEnd);
    }
}
