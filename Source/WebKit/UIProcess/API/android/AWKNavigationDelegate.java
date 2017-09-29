/*
 * Copyright (C) 2014 NAVER Corp. All rights reserved.
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

@NativeNamespace("org.webkit")
@NativeExportMacro("ANDROID_EXTERN")
public class AWKNavigationDelegate {
    @AccessedByNative
    public static final int NAVIGATION_ACTION_POLICY_CANCEL = 0;
    @AccessedByNative
    public static final int NAVIGATION_ACTION_POLICY_ALLOW = 1;

    @AccessedByNative
    public static final int NAVIGATION_RESPONSE_POLICY_CANCEL = 0;
    @AccessedByNative
    public static final int NAVIGATION_RESPONSE_POLICY_ALLOW = 1;

    @CalledByNative
    public void didCommitNavigation(AWKWebContent webContent,
            AWKNavigation navigation) {
    }

    @CalledByNative
    public void didStartProvisionalNavigation(AWKWebContent webContent,
            AWKNavigation navigation) {
    }

    @CalledByNative
    public void didReceiveServerRedirectForProvisionalNavigation(
            AWKWebContent webContent, AWKNavigation navigation) {
    }

    @CalledByNative
    public void didFailNavigation(AWKWebContent webContent,
            AWKNavigation navigation, AWKError error) {
    }

    @CalledByNative
    public void didFailProvisionalNavigation(AWKWebContent webContent,
            AWKNavigation navigation, AWKError error) {
    }

    @CalledByNative
    public void didFinishNavigation(AWKWebContent webContent,
            AWKNavigation navigation) {
    }

    @CalledByNative
    public void webViewWebContentProcessDidTerminate(AWKWebContent webContent) {
    }

    /**
     * @param decisionHandler
     *            Call with one of the NAVIGATION_ACTION_POLICY_XXX values.
     */
    @CalledByNative
    public void decidePolicyForNavigationAction(AWKWebContent webContent,
            AWKNavigationAction navigationAction,
            AWKFunction/* int */decisionHandler) {
    }

    /**
     * @param decisionHandler
     *            Call with one of the NAVIGATION_RESPONSE_POLICY_XXX values.
     */
    @CalledByNative
    public void decidePolicyForNavigationResponse(AWKWebContent webContent,
            AWKNavigationResponse navigationResponse,
            AWKFunction/* int */decisionHandler) {
    }
}
