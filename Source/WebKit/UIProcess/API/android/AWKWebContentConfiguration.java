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
public class AWKWebContentConfiguration {
    public native void setApplicationNameForUserAgent(
            String applicationNameForUserAgent);

    public native String getApplicationNameForUserAgent();

    public native void setPreferences(AWKPreferences preferences);

    public native AWKPreferences getPreferences();

    public native void setProcessPool(AWKProcessPool processPool);

    public native AWKProcessPool getProcessPool();

    public native void setUserContentController(
            AWKUserContentController userContentController);

    public native AWKUserContentController getUserContentController();

    public native void setWebsiteDataStore(AWKWebsiteDataStore websiteDataStore);

    public native AWKWebsiteDataStore getWebsiteDataStore();

    public native void setIgnoresViewportScaleLimits(
            boolean ignoresViewportScaleLimits);

    public native boolean getIgnoresViewportScaleLimits();

    public native void setSuppressesIncrementalRendering(
            boolean suppressesIncrementalRendering);

    public native boolean getSuppressesIncrementalRendering();

    public native void setAllowsInlineMediaPlayback(
            boolean allowsInlineMediaPlayback);

    public native boolean getAllowsInlineMediaPlayback();

    @AccessedByNative
    public static final int SELECTION_GRANULARITY_DYNAMIC = 0;
    @AccessedByNative
    public static final int SELECTION_GRANULARITY_CHARACTER = 1;

    public native void setSelectionGranularity(int selectionGranularity);

    public native int getSelectionGranularity();

    @AccessedByNative
    public static final int USER_INTERFACE_DIRECTION_POLICY_CONTENT = 0;
    @AccessedByNative
    public static final int USER_INTERFACE_DIRECTION_POLICY_SYSTEM = 1;

    public native void setUserInterfaceDirectionPolicy(
            int userInterfaceDirectionPolicy);

    public native int getUserInterfaceDirectionPolicy();

    @AccessedByNative
    public static final int DATA_DETECTOR_TYPE_NONE = 0;
    @AccessedByNative
    public static final int DATA_DETECTOR_TYPE_PHONE_NUMBER = 1; // 1 << 0
    @AccessedByNative
    public static final int DATA_DETECTOR_TYPE_LINK = 2; // 1 << 1
    @AccessedByNative
    public static final int DATA_DETECTOR_TYPE_ADDRESS = 4; // 1 << 2
    @AccessedByNative
    public static final int DATA_DETECTOR_TYPE_CALENDAR_EVENT = 8; // 1 << 3
    @AccessedByNative
    public static final int DATA_DETECTOR_TYPE_TRACKING_NUMBER = 16; // 1 << 4
    @AccessedByNative
    public static final int DATA_DETECTOR_TYPE_FLIGHT_NUMBER = 32; // 1 << 5
    @AccessedByNative
    public static final int DATA_DETECTOR_TYPE_LOOKUP_SUGGESTION = 64; // 1 << 6
    @AccessedByNative
    public static final int DATA_DETECTOR_TYPE_ALL = -1;

    public native void setDataDetectorTypes(int dataDetectorTypes);

    public native int getDataDetectorTypes();

    @CalledByNative
    public AWKWebContentConfiguration() {
        nativeCreate();
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

    @NativeDestructor
    private native void nativeDestroy();
}
