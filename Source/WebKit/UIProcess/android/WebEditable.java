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
import java.util.Vector;

@NativeNamespace("org.webkit")
@NativeExportMacro("ANDROID_EXTERN")
public class WebEditable {
    public native Rect editableCaretRectAtStart();

    public native Rect editableCaretRectAtEnd();

    public native String editableSelectionText();

    public native String editableTextBeforeCaret(int length);

    public native String editableTextAfterCaret(int length);

    public native boolean setComposition(String text,
            Vector<CompositionClause> clauses, int selectionStart,
            int selectionEnd, int replacementStart, int replacementEnd);

    public native boolean confirmComposition(String text);

    public native boolean finishComposition();

    public native void cancelComposition();

    public native boolean performEditorAction(int editorAction);

    public native boolean sendKeyEvent(long timestamp, int action,
            int modifiers, int virtualKeyCode, int nativeKeyCode, String text,
            String unmodifiedText, boolean isAutoRepeat, boolean isSystemKey,
            boolean isKeypadKey, long downTime);

    @NativeObjectField
    private long mNativePtr;

    @CalledByNative
    public WebEditable(AWKView view) {
        nativeCreate(view);
    }

    @Override
    protected void finalize() {
        nativeDestroy();
    }

    @NativeConstructor
    private native void nativeCreate(AWKView view);

    @NativeDestructor
    private native void nativeDestroy();
}
