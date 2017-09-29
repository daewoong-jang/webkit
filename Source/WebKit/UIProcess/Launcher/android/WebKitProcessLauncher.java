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

import org.webkit.AWKFunction;
import org.webkit.WebViewPrivate;

import android.app.AlertDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.ParcelFileDescriptor;
import android.os.RemoteException;
import android.util.SparseArray;
import static org.webkit.LogHelper.*;

public class WebKitProcessLauncher {
    public static interface MessageCallback {
        public void run(Message msg);
    }

    public class Connection implements ServiceConnection {
        private class ConnectionHandler extends Handler {
            @Override
            public void handleMessage(Message msg) {
                switch (msg.what) {
                case WebKitProcess.REQUEST_DECISION: {
                    AlertDialog.Builder builder = new AlertDialog.Builder(
                            WebViewPrivate.getApplicationContext());
                    builder.setMessage(msg.getData().getCharSequence("text"));
                    builder.setNegativeButton("Deny", new OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            try {
                                mMessageSender.send(Message.obtain(null,
                                        WebKitProcess.RECEIVE_DECISION,
                                        WebKitProcess.DECISION_DENY, 0));
                            } catch (RemoteException e) {
                                e.printStackTrace();
                            }
                        }
                    });
                    builder.setPositiveButton("Allow", new OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            try {
                                mMessageSender.send(Message.obtain(null,
                                        WebKitProcess.RECEIVE_DECISION,
                                        WebKitProcess.DECISION_ALLOW, 0));
                            } catch (RemoteException e) {
                                e.printStackTrace();
                            }
                        }
                    });
                    builder.show();
                    break;
                }
                default:
                    if (msg.arg1 != 0) {
                        MessageHost host = mMessageHosts.get(msg.arg1);
                        if (host != null)
                            host.receiveMessage(Connection.this, msg);
                    } else {
                        MessageHost.receiveMessageWithoutUid(Connection.this,
                                msg);
                    }
                    super.handleMessage(msg);
                }
            }
        };

        private Connection(int pid, Intent intent, int processType,
                int connector) {
            mPid = pid;
            mIntent = intent;
            mProcessType = processType;
            mConnector = connector;
            mMessengerHandler = new ConnectionHandler();
            mMessageReceiver = new Messenger(mMessengerHandler);
        }

        private int mPid;
        private Intent mIntent;
        private int mProcessType;
        private int mConnector;
        private boolean mIsOpen = false;
        private Handler mMessengerHandler;
        private Messenger mMessageReceiver;
        private Messenger mMessageSender;
        private SparseArray<MessageHost> mMessageHosts = new SparseArray<MessageHost>();

        public int getPid() {
            return mPid;
        }

        public Intent getIntent() {
            return mIntent;
        }

        public void send(Message message) {
            try {
                mMessageSender.send(message);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }

        public void addMessageHost(MessageHost host) {
            mMessageHosts.append(host.getUid(), host);
        }

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            mIsOpen = true;
            mMessageSender = new Messenger(service);
            Message message = Message.obtain(null, WebKitProcess.START_PROCESS);
            message.replyTo = mMessageReceiver;
            Bundle bundle = new Bundle();
            bundle.putInt("processType", mProcessType);
            bundle.putParcelable("connector",
                    ParcelFileDescriptor.adoptFd(mConnector));
            message.setData(bundle);
            try {
                mMessageSender.send(message);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
            mDidFinishLaunchingProcess.get(mPid).invoke(mPid);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            mIsOpen = false;
            mMessageSender = null;
            disconnect(mPid);
        }
    }

    static final int MAX_PROCESS = 8;

    private WebKitProcessLauncher() {
        Messages.registerMessages();
    }

    private static WebKitProcessLauncher launcher = new WebKitProcessLauncher();

    public static WebKitProcessLauncher get() {
        return launcher;
    }

    private int getAvailableWebProcessConnection() {
        int slot = 1;
        while (slot <= MAX_PROCESS) {
            if (mRunningProcess.indexOfKey(slot) < 0)
                break;
            ++slot;
        }
        return slot;
    }

    private static String getWebProcessClassName(int pid) {
        return WebKitProcess.class.getPackage().getName() + ".WebKitWebProcess"
                + Integer.toString(pid);
    }

    public Connection getConnection(int pid) {
        return mRunningProcess.get(pid);
    }

    public boolean isConnected(int pid) {
        return mRunningProcess.indexOfKey(pid) >= 0
                && mRunningProcess.get(pid).mIsOpen;
    }

    public int connect(int processType, int processStrategy, int connector,
            AWKFunction didFinishLaunchingProcess) {
        int slot = getAvailableWebProcessConnection();
        if (slot > MAX_PROCESS) {
            LOGE("Can't create another process");
            return 0;
        }

        Intent intent = new Intent();
        intent.setClassName(WebViewPrivate.getApplicationContext()
                .getPackageName(), getWebProcessClassName(slot));

        LOGA_IF(mRunningProcess.indexOfKey(slot) < 0, "Invalid connection");
        mRunningProcess.append(slot, new Connection(slot, intent, processType,
                connector));
        mDidFinishLaunchingProcess.append(slot, didFinishLaunchingProcess);

        Connection connection = mRunningProcess.get(slot);
        if (!WebViewPrivate.getApplicationContext().bindService(intent,
                connection, Context.BIND_AUTO_CREATE)) {
            LOGE("Couldn't connect to service");
            mRunningProcess.remove(slot);
            mDidFinishLaunchingProcess.remove(slot);
            return 0;
        }

        return slot;
    }

    public boolean disconnect(int pid) {
        if (!isConnected(pid))
            return true;

        WebViewPrivate.getApplicationContext().unbindService(
                mRunningProcess.get(pid));
        mRunningProcess.remove(pid);
        return true;
    }

    private SparseArray<Connection> mRunningProcess = new SparseArray<Connection>();
    private SparseArray<AWKFunction> mDidFinishLaunchingProcess = new SparseArray<AWKFunction>();
}
