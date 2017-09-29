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

import java.util.HashSet;
import java.util.Vector;

import org.webkit.launcher.WebKitProcess;
import org.webkit.launcher.WebKitProcess.MessageCallback;

import android.content.res.Resources;
import android.graphics.Point;
import android.os.Message;
import android.util.DisplayMetrics;

@NativeNamespace("WebCore")
@NativeExportMacro("WEBCORE_EXPORT")
public class WebCorePlatform {
    // PlatformScreen
    @SuppressWarnings("unused")
    @CalledByNative
    private static int screenDepth() {
        return 24;
    }

    @SuppressWarnings("unused")
    @CalledByNative
    private static boolean screenIsMonochrome() {
        return false;
    }

    @SuppressWarnings("unused")
    @CalledByNative
    private static Point screenSize() {
        // FIXME: it should return physical size.
        return availableScreenSize();
    }

    @SuppressWarnings("unused")
    @CalledByNative
    private static Point availableScreenSize() {
        DisplayMetrics metrics = Resources.getSystem().getDisplayMetrics();
        return new Point(metrics.widthPixels, metrics.heightPixels);
    }

    // SSLHandle
    private static boolean mAllowHostDecision = false;
    private static HashSet<String> mAllowedHosts = getDefaultAllowedHosts();

    private static HashSet<String> getDefaultAllowedHosts() {
        HashSet<String> allowedHosts = new HashSet<String>();
        allowedHosts.add("webkit.org");
        allowedHosts.add("pbs.twimg.com");
        allowedHosts.add("www.apple.com");
        allowedHosts.add("www.google-analytics.com");
        return allowedHosts;
    }

    @SuppressWarnings("unused")
    @CalledByNative
    private static boolean requestAllowHostDecision(String host) {
        if (mAllowedHosts.contains(host))
            return true;

        Message msg = Message
                .obtain(null, WebKitProcess.REQUEST_DECISION, 0, 0);
        msg.getData().putCharSequence("text", "Allow " + host + "?");
        msg.getData().putCharSequence("host", host);
        WebKitProcess.getCurrent().sendSyncMessage(msg,
                WebKitProcess.RECEIVE_DECISION, new MessageCallback() {
                    @Override
                    public void run(Message msg) {
                        mAllowHostDecision = (msg.arg1 == WebKitProcess.DECISION_ALLOW);
                    }
                });
        if (mAllowHostDecision)
            mAllowedHosts.add(host);
        return mAllowHostDecision;
    }

    // SSLKeyGenerator
    @SuppressWarnings("unused")
    @CalledByNative
    private static String signedPublicKeyAndChallengeString(int index,
            String challenge, String url) {
        return "";
    }

    // UserAgent
    @SuppressWarnings("unused")
    @CalledByNative
    private static String platformVersionForUAString() {
        return "Android";
    }

    // WebInspector
    @SuppressWarnings("unused")
    @CalledByNative
    private static String inspectorBasePath() {
        return "";
    }

    // API::WebsiteDataStore
    @SuppressWarnings("unused")
    @CalledByNative
    private static String websiteDataDirectory() {
        return "";
    }

    // MediaPlayerPrivate
    @SuppressWarnings("unused")
    @CalledByNative
    private static Vector<String> getSupportedMIMETypes() {
        Vector mimeTypes = new Vector();
        mimeTypes.addElement("audio/3gpp");
        mimeTypes.addElement("audio/3gpp2");
        mimeTypes.addElement("audio/aac");
        mimeTypes.addElement("audio/amr");
        mimeTypes.addElement("audio/flac");
        mimeTypes.addElement("audio/midi");
        mimeTypes.addElement("audio/mp4");
        mimeTypes.addElement("audio/mpeg");
        mimeTypes.addElement("audio/ogg");
        mimeTypes.addElement("audio/x-matroska");
        mimeTypes.addElement("audio/x-wav");
        mimeTypes.addElement("video/3gpp");
        mimeTypes.addElement("video/3gpp2");
        mimeTypes.addElement("video/mp4");
        mimeTypes.addElement("video/mpeg");
        mimeTypes.addElement("video/webm");
        mimeTypes.addElement("video/x-matroska");
        return mimeTypes;
    }

    private WebCorePlatform() {
        // Utility class, do not instantiate.
    }
}
