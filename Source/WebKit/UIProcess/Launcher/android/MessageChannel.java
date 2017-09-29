/*
 * Copyright (C) 2017 Daewoong Jang.
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

package org.webkit.launcher;

import org.webkit.launcher.WebKitProcess.MessageCallback;

import android.os.Bundle;
import android.os.Message;
import static org.webkit.LogHelper.*;

public class MessageChannel {
    private int mUid = 0;

    protected MessageChannel(int openMessage) {
        sendSyncMessage(openMessage, 0, MessageHost.CHANNEL_OPENED,
                new MessageCallback() {
                    @Override
                    public void run(Message msg) {
                        setUid(msg.arg1);
                        WebKitProcess.getCurrent().addMessageChannel(
                                MessageChannel.this);
                        onChannelOpen();
                    }
                });
    }

    protected void close() {
        WebKitProcess.getCurrent().removeMessageChannel(mUid);
    }
    
    public void setUid(int uid) {
        mUid = uid;
    }

    public int getUid() {
        return mUid;
    }

    protected void sendMessage(int what, int arg2) {
        WebKitProcess.getCurrent().sendMessage(
                Message.obtain(null, what, mUid, arg2));
    }

    protected void sendMessage(int what, int arg2, String data) {
        WebKitProcess.getCurrent().sendMessage(
                Messages.obtain(what, mUid, arg2, data));
    }

    protected void sendMessage(int what, int arg2, Bundle data) {
        WebKitProcess.getCurrent().sendMessage(
                Messages.obtain(what, mUid, arg2, data));
    }

    protected void sendSyncMessage(int what, int arg2, int want,
            MessageCallback callback) {
        WebKitProcess.getCurrent().sendSyncMessage(
                Message.obtain(null, what, mUid, arg2), want, callback);
    }

    protected void sendSyncMessage(int what, int arg2, String data, int want,
            MessageCallback callback) {
        WebKitProcess.getCurrent().sendSyncMessage(
                Messages.obtain(what, mUid, arg2, data), want, callback);
    }

    protected void sendSyncMessage(int what, int arg2, Bundle data, int want,
            MessageCallback callback) {
        WebKitProcess.getCurrent().sendSyncMessage(
                Messages.obtain(what, mUid, arg2, data), want, callback);
    }

    protected void onChannelOpen() {
        LOGD("MessageChannel #" + mUid + " opened");
    }

    public boolean receiveMessage(Message msg) {
        return false;
    }
}
