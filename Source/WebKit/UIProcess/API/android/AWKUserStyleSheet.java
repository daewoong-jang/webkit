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

import java.util.Vector;

@NativeNamespace("org.webkit")
@NativeExportMacro("ANDROID_EXTERN")
public class AWKUserStyleSheet {
    public native String getSource();

    public native AWKURL getBaseURL();

    public native boolean isForMainFrameOnly();

    @CalledByNative
    private AWKUserStyleSheet() {
        nativeCreate();
    }

    public AWKUserStyleSheet(String source, boolean forMainFrameOnly) {
        nativeCreateWithSource(source, forMainFrameOnly);
    }

    public AWKUserStyleSheet(String source, boolean forMainFrameOnly,
            Vector<String> whitelist, Vector<String> blacklist) {
        nativeCreateWithLists(source, forMainFrameOnly, whitelist, blacklist);
    }

    public AWKUserStyleSheet(String source, boolean forMainFrameOnly,
            Vector<String> whitelist, Vector<String> blacklist, AWKURL baseURL) {
        nativeCreateWithBaseURL(source, forMainFrameOnly, whitelist, blacklist,
                baseURL);
    }

    @NativeObjectField
    private long mNativePtr;

    @Override
    protected void finalize() throws Throwable {
        try {
            nativeDestroy();
        } finally {
            super.finalize();
        }
    }

    @NativeConstructor
    private native void nativeCreate();

    @NativeConstructor
    private native void nativeCreateWithSource(String source,
            boolean forMainFrameOnly);

    @NativeConstructor
    private native void nativeCreateWithLists(String source,
            boolean forMainFrameOnly, Vector<String> whitelist,
            Vector<String> blacklist);

    @NativeConstructor
    private native void nativeCreateWithBaseURL(String source,
            boolean forMainFrameOnly, Vector<String> whitelist,
            Vector<String> blacklist, AWKURL baseURL);

    @NativeDestructor
    private native void nativeDestroy();
}
