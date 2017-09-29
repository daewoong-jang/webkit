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

package org.webkit.launcher;

import org.webkit.wtf.WTFMainThread;

import android.app.Service;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.Messenger;
import android.os.ParcelFileDescriptor;
import android.os.Parcelable;
import android.os.Process;
import android.os.RemoteException;
import android.util.SparseArray;
import static org.webkit.LogHelper.*;

public class WebKitProcess extends Service {
    public static final int START_PROCESS = 1000;

    public static final int REQUEST_DECISION = 100000;
    public static final int RECEIVE_DECISION = 100001;

    public static final int DECISION_DENY = 0;
    public static final int DECISION_ALLOW = 1;

    private class IntPair {
        public int first = 0;
        public int second = 0;
    }

    private class WebKitProcessHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
            case WebKitProcess.START_PROCESS: {
                mMessageSender = msg.replyTo;
                msg.replyTo = null;
                Bundle bundle = msg.getData();

                int processType = bundle.getInt("processType",
                        ProcessLauncherBackend.PROCESS_TYPE_WEB);
                Parcelable fd = bundle.getParcelable("connector");
                if (fd == null
                        || fd.describeContents() != Parcelable.CONTENTS_FILE_DESCRIPTOR) {
                    LOGA("No file descriptor to send IPC message through");
                    return;
                }

                int connector = ((ParcelFileDescriptor) fd).detachFd();
                if (connector <= 0) {
                    LOGA("File descriptor is invalid");
                    return;
                }

                runWebKitMain(processType, connector);
                break;
            }
            default:
                if (!receiveReplyMessage(msg) && msg.arg1 != 0) {
                    MessageChannel channel = mMessageChannels.get(msg.arg1);
                    if (channel != null)
                        channel.receiveMessage(msg);
                }
                super.handleMessage(msg);
            }
        }
    };

    private static WebKitProcess mWebKitProcess = null;

    protected WebKitProcess() {
        mMessengerHandler = new WebKitProcessHandler();
        mMessageReceiver = new Messenger(mMessengerHandler);
        Messages.registerMessages();
        WTFMainThread.initializeUIThread();
    }

    private Handler mMessengerHandler;
    private Messenger mMessageReceiver;
    private Messenger mMessageSender;
    private Intent mIntent;
    private Thread mMainThread;
    private int mWant = 0;
    private IntPair mReply = new IntPair();
    private SparseArray<MessageChannel> mMessageChannels = new SparseArray<MessageChannel>();

    public static WebKitProcess getCurrent() {
        return mWebKitProcess;
    }

    public Intent getIntent() {
        return mIntent;
    }

    protected void addMessageChannel(MessageChannel channel) {
        mMessageChannels.append(channel.getUid(), channel);
    }

    protected void removeMessageChannel(int uid) {
        mMessageChannels.remove(uid);
    }

    protected void runWebKitMain(final int processType, final int connector) {
        mMainThread = new Thread() {
            @Override
            public void run() {
                Looper.prepare();
                WTFMainThread.initializeUIThread();
                ProcessLauncherBackend.runWebKitMain(processType, connector);
                Looper.loop();
            }
        };
        mMainThread.start();
    }

    public static interface MessageCallback {
        public void run(Message msg);
    }

    public void sendMessage(Message msg) {
        try {
            mMessageSender.send(msg);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    public void sendSyncMessage(Message msg, int want, MessageCallback callback) {
        try {
            synchronized (this) {
                mWant = want;
                mMessageSender.send(msg);
                this.wait();
                callback.run(Message.obtain(null, mWant, mReply.first,
                        mReply.second));
            }
        } catch (RemoteException e) {
            e.printStackTrace();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    private boolean receiveReplyMessage(Message msg) {
        synchronized (this) {
            if (msg.what == mWant) {
                mReply.first = msg.arg1;
                mReply.second = msg.arg2;
                this.notifyAll();
                return true;
            }
            return false;
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        mIntent = intent;
        return mMessageReceiver.getBinder();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
    }

    @Override
    public void onCreate() {
        mWebKitProcess = this;
    }

    @Override
    public void onDestroy() {
        Process.killProcess(Process.myPid());
    }

    @Override
    public void onLowMemory() {
        super.onLowMemory();
    }

    @Override
    public void onRebind(Intent intent) {
        super.onRebind(intent);
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        super.onStartCommand(intent, flags, startId);
        return START_NOT_STICKY;
    }

    @Override
    public void onTaskRemoved(Intent rootIntent) {
        super.onTaskRemoved(rootIntent);
    }

    @Override
    public void onTrimMemory(int level) {
        super.onTrimMemory(level);
    }

    @Override
    public boolean onUnbind(Intent intent) {
        super.onUnbind(intent);
        stopSelf();
        return false;
    }

    static {
        System.loadLibrary("WebKit2");
    }
}
