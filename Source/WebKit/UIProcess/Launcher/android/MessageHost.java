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

import android.os.Message;
import android.util.SparseArray;
import static org.webkit.LogHelper.*;

public class MessageHost {
    public static int CHANNEL_OPENED;

    public static interface Constructor {
        public MessageHost run(WebKitProcessLauncher.Connection connection,
                Message msg);
    }

    private WebKitProcessLauncher.Connection mConnection = null;
    private static int sNextUid = 1;
    private int mUid = 0;

    protected MessageHost() {
        mUid = sNextUid++;
    }

    public int getUid() {
        return mUid;
    }

    protected void sendMessage(int what, int arg2) {
        mConnection.send(Message.obtain(null, what, mUid, arg2));
    }

    public boolean receiveMessage(WebKitProcessLauncher.Connection connection,
            Message msg) {
        return true;
    }

    public static boolean receiveMessageWithoutUid(
            WebKitProcessLauncher.Connection connection, Message msg) {
        LOGA_IF(msg.arg1 == 0, "Message has uid which is not expected");
        MessageHost host = constructHost(connection, msg);
        if (host == null)
            return false;
        host.mConnection = connection;
        connection.addMessageHost(host);
        host.sendMessage(CHANNEL_OPENED, 0);
        return true;
    }

    private static SparseArray<Constructor> mConstructors = new SparseArray<Constructor>();

    public static void registerHost(int what, Constructor constructor) {
        mConstructors.append(what, constructor);
    }

    public static MessageHost constructHost(
            WebKitProcessLauncher.Connection connection, Message msg) {
        if (mConstructors.indexOfKey(msg.what) < 0)
            return null;
        return mConstructors.get(msg.what).run(connection, msg);
    }

    public static void registerMessages() {
        CHANNEL_OPENED = Messages.registerMessage();
    }
}
