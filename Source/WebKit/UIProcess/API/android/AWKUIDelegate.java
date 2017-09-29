/*
 * Copyright (C) 2014-2015 NAVER Corp. All rights reserved.
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
public abstract class AWKUIDelegate {
    @CalledByNative
    public AWKWebContent createWebViewWithConfiguration(
            AWKWebContentConfiguration configuration,
            AWKNavigationAction navigationAction,
            AWKWindowFeatures windowFeatures) {
        return null;
    }

    @CalledByNative
    public boolean runJavaScriptAlertPanelWithMessage(AWKWebContent webContent,
            String message, AWKFrameInfo frame,
            final AWKFunction/* void */ completionHandler) {
        return false;
    }

    @CalledByNative
    public boolean runJavaScriptConfirmPanelWithMessage(
            AWKWebContent webContent, String message, AWKFrameInfo frame,
            final AWKFunction/* boolean */ completionHandler) {
        return false;
    }

    @CalledByNative
    public boolean runBeforeUnloadConfirmPanelWithMessage(
            AWKWebContent webContent, String message, AWKFrameInfo frame,
            final AWKFunction/* boolean */ completionHandler) {
        return false;
    }

    @CalledByNative
    public boolean runJavaScriptTextInputPanelWithPrompt(
            AWKWebContent webContent, String prompt, String defaultText,
            AWKFrameInfo frame, final AWKFunction/* String */ completionHandler) {
        return false;
    }

    @CalledByNative
    public void webViewDidClose(AWKWebContent webContent) {
    }

    @CalledByNative
    public boolean runOpenPanelWithParameters(
            AWKOpenPanelParameters parameters, AWKFrameInfo frame,
            final AWKFunction/*Vector<WKURL>*/ completionHandler) {
        return false;
    }

    @CalledByNative
    public boolean shouldPreviewElement(AWKPreviewElementInfo elementInfo) {
        return false;
    }

    @CalledByNative
    public AWKWebContent previewingViewControllerForElement(
            AWKPreviewElementInfo elementInfo,
            Vector<AWKPreviewActionItem> previewActions) {
        return null;
    }

    @CalledByNative
    public void commitPreviewingViewController(AWKWebContent previewingViewController) {
    }
}
