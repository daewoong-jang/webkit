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

@NativeNamespace("org.webkit")
@NativeExportMacro("ANDROID_EXTERN")
public class AWKWebContent {
    @AbstractMethod
    public native AWKWebContentConfiguration getConfiguration();

    @CalledByNative
    public AWKWebContent(Rect frame, AWKWebContentConfiguration configuration) {
        nativeCreate(frame, configuration);
    }

    @AbstractMethod
    public native String getTitle();

    @AbstractMethod
    public native AWKURL getURL();

    @AbstractMethod
    public native String getCustomUserAgent();

    @AbstractMethod
    public native void setCustomUserAgent(String customUserAgent);

    @AbstractMethod
    public native void setNavigationDelegate(
            AWKNavigationDelegate navigationDelegate);

    @AbstractMethod
    public native AWKNavigationDelegate getNavigationDelegate();

    @AbstractMethod
    public native void setUIDelegate(AWKUIDelegate uiDelegate);

    @AbstractMethod
    public native AWKUIDelegate getUIDelegate();

    @AbstractMethod
    public native double getEstimatedProgress();

    @AbstractMethod
    public native boolean hasOnlySecureContent();

    @AbstractMethod
    public native AWKNavigation loadRequest(AWKURLRequest request);

    @AbstractMethod
    public native AWKNavigation loadHTMLString(String htmlString, AWKURL baseURL);

    @AbstractMethod
    public native boolean isLoading();

    @AbstractMethod
    public native AWKNavigation reload();

    @AbstractMethod
    public native AWKNavigation reloadFromOrigin();

    @AbstractMethod
    public native void stopLoading();

    @AbstractMethod
    public native AWKNavigation loadData(AWKData data, String mimeType,
            String characterEncodingName, AWKURL baseURL);

    @AbstractMethod
    public native AWKNavigation loadFileURL(AWKURL url, AWKURL readAccessUrl);

    @AbstractMethod
    public native AWKBackForwardList getBackForwardList();

    @AbstractMethod
    public native boolean canGoBack();

    @AbstractMethod
    public native boolean canGoForward();

    @AbstractMethod
    public native AWKNavigation goBack();

    @AbstractMethod
    public native AWKNavigation goForward();

    @AbstractMethod
    public native AWKNavigation goToBackForwardListItem(
            AWKBackForwardListItem item);

    @AbstractMethod
    public native void evaluateJavaScript(String javaScriptString,
            AWKCallback/* Object, Object */completionHandler);

    @AbstractMethod
    public native AWKView getView();

    @AbstractMethod
    public native AWKView peekView();

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
    private native void nativeCreate(Rect frame,
            AWKWebContentConfiguration configuration);

    @NativeDestructor
    private native void nativeDestroy();
}
