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

package org.webkit.wtf;

import java.util.concurrent.atomic.AtomicBoolean;

import android.os.Handler;
import android.os.Message;

import org.webkit.CalledByNative;
import org.webkit.NativeConstructor;
import org.webkit.NativeDestructor;
import org.webkit.NativeExportMacro;
import org.webkit.NativeNamespace;
import org.webkit.NativeObjectField;

@NativeNamespace("WTF")
@NativeExportMacro("WTF_EXPORT_PRIVATE")
class ThreadLoopBackend {
    private static final boolean DEBUG = false;

    private AtomicBoolean mMessageFired = new AtomicBoolean(true);

    @CalledByNative
    private void dispatch() {
        if (!mMessageFired.getAndSet(false)) {
            // mMessageFired was already false.
            return;
        }
        mHandler.sendEmptyMessage(0);
    }

    @CalledByNative
    private void dispatchTimer(int timerID, double nextFireIntervalInMillis) {
        long nextFireInterval = (long) nextFireIntervalInMillis;
        mHandler.sendEmptyMessageDelayed(timerID, nextFireInterval);
    }

    @CalledByNative
    private void removeTimer(int timerID) {
        mHandler.removeMessages(timerID);
    }

    @CalledByNative
    private void stop() {
        mHandler.getLooper().quit();
    }

    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            if (msg.what == 0) {
                mMessageFired.set(true);
                nativePerformMessage();
            } else {
                nativePerformTimer(msg.what);
            }
        }
    };

    @NativeObjectField
    private int mNativePtr;

    @CalledByNative
    private ThreadLoopBackend() {
        nativeCreate();
    }

    @Override
    protected void finalize() {
        nativeDestroy();
    }

    @NativeConstructor
    private native void nativeCreate();

    @NativeDestructor
    private native void nativeDestroy();

    private native void nativePerformMessage();

    private native void nativePerformTimer(int timerID);
}
