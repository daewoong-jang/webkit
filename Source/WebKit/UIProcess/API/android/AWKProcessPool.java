/*
 * Copyright (C) 2017 NAVER Corp. All rights reserved.
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

import org.webkit.AWKDownloadDelegate;
import org.webkit.WKPrivate;
import org.webkit.AWKProcessPoolConfiguration;

@NativeNamespace("org.webkit")
@NativeExportMacro("ANDROID_EXTERN")
public final class AWKProcessPool {
    @AbstractMethod
    @WKPrivate
    public native void warmInitialProcess();

    @AbstractMethod
    @WKPrivate
    public native void setDownloadDelegate(AWKDownloadDelegate downloadDelegate);

    @CalledByNative
    public AWKProcessPool() {
        nativeCreate();
    }

    @CalledByNative
    @WKPrivate
    public AWKProcessPool(AWKProcessPoolConfiguration configuration) {
        nativeCreateWithConfiguration(configuration);
    }

    @NativeObjectField
    private long mNativePtr;

    @Override
    protected void finalize() {
        nativeDestroy();
    }

    @NativeConstructor
    private native void nativeCreate();

    @NativeConstructor
    private native void nativeCreateWithConfiguration(
            AWKProcessPoolConfiguration configuration);

    @NativeDestructor
    private native void nativeDestroy();
}
