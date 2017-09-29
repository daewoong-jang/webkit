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

@NativeNamespace("org.webkit")
@NativeExportMacro("ANDROID_EXTERN")
public class AWKPreferences {
    public native void setMinimumFontSize(int size);

    public native int getMinimumFontSize();

    public native void setJavaScriptEnabled(boolean enable);

    public native boolean getJavaScriptEnabled();

    public native void setJavaScriptCanOpenWindowsAutomatically(boolean enable);

    public native boolean getJavaScriptCanOpenWindowsAutomatically();

    @WKPrivate
    public native void setTelephoneNumberDetectionIsEnabled(
            boolean telephoneNumberDetectionIsEnabled);

    @WKPrivate
    public native boolean getTelephoneNumberDetectionIsEnabled();

    @WKPrivate
    public static final int STORAGE_BLOCKING_POLICY_ALLOW_ALL = 0;
    @WKPrivate
    public static final int STORAGE_BLOCKING_POLICY_BLOCK_THIRD_PARTY = 0;
    @WKPrivate
    public static final int STORAGE_BLOCKING_POLICY_BLOCK_ALL = 0;

    @WKPrivate
    public native void setStorageBlockingPolicy(int storageBlockingPolicy);

    @WKPrivate
    public native int getStorageBlockingPolicy();

    @WKPrivate
    public native void setCompositingBordersVisible(
            boolean compositingBordersVisible);

    @WKPrivate
    public native boolean getCompositingBordersVisible();

    @WKPrivate
    public native void setCompositingRepaintCountersVisible(
            boolean compositingRepaintCountersVisible);

    @WKPrivate
    public native boolean getCompositingRepaintCountersVisible();

    @WKPrivate
    public native void setTiledScrollingIndicatorVisible(
            boolean tiledScrollingIndicatorVisible);

    @WKPrivate
    public native boolean getTiledScrollingIndicatorVisible();

    @WKPrivate
    public static final int DEBUG_OVERLAY_REGION_NON_FAST_SCROLLABLE_REGION = 1 << 0;
    @WKPrivate
    public static final int DEBUG_OVERLAY_REGION_WHEEL_EVENT_HANDLER_REGION = 1 << 1;

    @WKPrivate
    public native void setVisibleDebugOverlayRegions(
            int visibleDebugOverlayRegions);

    @WKPrivate
    public native int getVisibleDebugOverlayRegions();

    @WKPrivate
    public native void setSimpleLineLayoutDebugBordersEnabled(
            boolean simpleLineLayoutDebugBordersEnabled);

    @WKPrivate
    public native boolean getSimpleLineLayoutDebugBordersEnabled();

    @WKPrivate
    public native void setVisualViewportEnabled(boolean visualViewportEnabled);

    @WKPrivate
    public native boolean getVisualViewportEnabled();

    @WKPrivate
    public native void setLargeImageAsyncDecodingEnabled(
            boolean largeImageAsyncDecodingEnabled);

    @WKPrivate
    public native boolean getLargeImageAsyncDecodingEnabled();

    @WKPrivate
    public native void setAnimatedImageAsyncDecodingEnabled(
            boolean animatedImageAsyncDecodingEnabled);

    @WKPrivate
    public native boolean getAnimatedImageAsyncDecodingEnabled();

    @WKPrivate
    public native void setTextAutosizingEnabled(boolean enable);

    @WKPrivate
    public native boolean getTextAutosizingEnabled();

    @WKPrivate
    public native void setDeveloperExtrasEnabled(boolean enable);

    @WKPrivate
    public native boolean getDeveloperExtrasEnabled();

    @WKPrivate
    public native void setLogsPageMessagesToSystemConsoleEnabled(
            boolean logsPageMessagesToSystemConsoleEnabled);

    @WKPrivate
    public native boolean getLogsPageMessagesToSystemConsoleEnabled();

    @WKPrivate
    public native void setHiddenPageDOMTimerThrottlingEnabled(
            boolean hiddenPageDOMTimerThrottlingEnabled);

    @WKPrivate
    public native boolean getHiddenPageDOMTimerThrottlingEnabled();

    @WKPrivate
    public native void setHiddenPageDOMTimerThrottlingAutoIncreases(
            boolean hiddenPageDOMTimerThrottlingAutoIncreases);

    @WKPrivate
    public native boolean getHiddenPageDOMTimerThrottlingAutoIncreases();

    @WKPrivate
    public native void setPageVisibilityBasedProcessSuppressionEnabled(
            boolean pageVisibilityBasedProcessSuppressionEnabled);

    @WKPrivate
    public native boolean getPageVisibilityBasedProcessSuppressionEnabled();

    @WKPrivate
    public native void setAllowFileAccessFromFileURLs(
            boolean allowFileAccessFromFileURLs);

    @WKPrivate
    public native boolean getAllowFileAccessFromFileURLs();

    @WKPrivate
    public static final int JAVA_SCRIPT_RUNTIME_FLAGS_SYMBOL_ENABLED = 1 << 0;
    @WKPrivate
    public static final int JAVA_SCRIPT_RUNTIME_FLAGS_PROMISE_DISABLED = 1 << 1;
    @WKPrivate
    public static final int JAVA_SCRIPT_RUNTIME_FLAGS_ALL_ENABLED = JAVA_SCRIPT_RUNTIME_FLAGS_SYMBOL_ENABLED;

    @WKPrivate
    public native void setJavaScriptRuntimeFlags(int javaScriptRuntimeFlags);

    @WKPrivate
    public native int getJavaScriptRuntimeFlags();

    @WKPrivate
    public native void setStandalone(boolean standalone);

    @WKPrivate
    public native boolean isStandalone();

    @WKPrivate
    public native void setDiagnosticLoggingEnabled(
            boolean diagnosticLoggingEnabled);

    @WKPrivate
    public native boolean getDiagnosticLoggingEnabled();

    @WKPrivate
    public native void setDefaultFontSize(int size);

    @WKPrivate
    public native int getDefaultFontSize();

    @WKPrivate
    public native void setDefaultFixedFontSize(int size);

    @WKPrivate
    public native int getDefaultFixedFontSize();

    @WKPrivate
    public native void setFixedFontFamily(String fixedFontFamily);

    @WKPrivate
    public native String getFixedFontFamily();

    @WKPrivate
    public native void setOfflineWebApplicationCacheEnabled(
            boolean offlineWebApplicationCacheEnabled);

    @WKPrivate
    public native boolean getOfflineWebApplicationCacheEnabled();

    @WKPrivate
    public native void setFullScreenEnabled(boolean fullScreenEnabled);

    @WKPrivate
    public native boolean getFullScreenEnabled();

    @CalledByNative
    public AWKPreferences() {
        nativeCreate();
    }

    @NativeObjectField
    private long mNativePtr;

    @Override
    protected void finalize() {
        nativeDestroy();
    }

    @NativeConstructor
    private native void nativeCreate();

    @NativeDestructor
    private native void nativeDestroy();
}
