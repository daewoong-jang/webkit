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

#include "config.h"
#include "WebKitProcessLauncher.h"

#include "MessageHost.h"
#include "ProcessLauncherBackend.h"
#include "WebKitProcess.h"
#include "WebView.h"
#include "WebViewPrivate.h"
#include <android/app/AlertDialog.h>
#include <android/os/Looper.h>
#include <android/os/ParcelFileDescriptor.h>
#include <android++/LogHelper.h>

namespace org {
namespace webkit {
namespace launcher {

class ConnectionHandler : public Handler {
public:
    void handleMessage(Message&) override;

    ConnectionHandler(WebKitProcessLauncher::Connection& that)
        : mThat(that) { }

    WebKitProcessLauncher::Connection& mThat;
};

void ConnectionHandler::handleMessage(Message& msg)
{
    switch (msg.what) {
    case WebKitProcess::REQUEST_DECISION: {
        AlertDialog::Builder builder(WebViewPrivate::getApplicationContext());
        builder.setMessage(msg.getData().getCharSequence(L"text"));
        builder.setNegativeButton(L"Deny", [=] (DialogInterface& dialog, int32_t which) {
            mThat.mMessageSender->send(Message::obtain(nullptr,
                    WebKitProcess::RECEIVE_DECISION,
                    WebKitProcess::DECISION_DENY, 0));
        });
        builder.setPositiveButton(L"Allow", [=] (DialogInterface& dialog, int32_t which) {
            mThat.mMessageSender->send(Message::obtain(nullptr,
                    WebKitProcess::RECEIVE_DECISION,
                    WebKitProcess::DECISION_ALLOW, 0));
        });
        builder.show();
        break;
    }
    default:
        if (msg.arg1 != 0) {
            auto& host = mThat.mMessageHosts[msg.arg1];
            if (host != nullptr)
                host->receiveMessage(mThat, msg);
        } else {
            MessageHost::receiveMessageWithoutUid(mThat, msg);
        }
        Handler::handleMessage(msg);
    }
}

WebKitProcessLauncher::Connection::Connection(WebKitProcessLauncher& that, int32_t pid, Intent& intent, int32_t processType, int32_t connector)
    : mThat(that)
    , mPid(pid)
    , mIntent(intent)
    , mProcessType(processType)
    , mConnector(connector)
    , mMessengerHandler(std::make_shared<ConnectionHandler>(*this))
    , mMessageReceiver(std::make_shared<Messenger>(mMessengerHandler))
{
}

WebKitProcessLauncher::Connection::~Connection()
{
}

int32_t WebKitProcessLauncher::Connection::getPid()
{
    return mPid;
}

Intent& WebKitProcessLauncher::Connection::getIntent()
{
    return mIntent;
}

void WebKitProcessLauncher::Connection::send(Message& message)
{
    mMessageSender->send(message);
}

void WebKitProcessLauncher::Connection::addMessageHost(std::passed_ptr<MessageHost> host)
{
    mMessageHosts[host->getUid()] = host;
}

void WebKitProcessLauncher::Connection::onServiceConnected(ComponentName& name, std::passed_ptr<IBinder> service)
{
    mIsOpen = true;
    mMessageSender = std::make_shared<Messenger>(service);
    Message message = Message::obtain(nullptr, WebKitProcess::START_PROCESS);
    message.replyTo = mMessageReceiver.get();
    Bundle bundle;
    bundle.putInt(L"processType", mProcessType);
    bundle.putParcelable(L"connector", ParcelFileDescriptor::adoptFd(mConnector));
    message.setData(WTFMove(bundle));
    mMessageSender->send(message);
    mThat.mDidFinishLaunchingProcess[mPid]->invoke(mPid);
}

void WebKitProcessLauncher::Connection::onServiceDisconnected(ComponentName& name)
{
    mIsOpen = false;
    mMessageSender = nullptr;
    mThat.disconnect(mPid);
}

WebKitProcessLauncher::WebKitProcessLauncher()
{
    Messages::registerMessages();
}

WebKitProcessLauncher::~WebKitProcessLauncher()
{
}

WebKitProcessLauncher& WebKitProcessLauncher::get()
{
    static WebKitProcessLauncher* launcher = new WebKitProcessLauncher;
    return *launcher;
}

int32_t WebKitProcessLauncher::getAvailableWebProcessConnection()
{
    int32_t slot = 1;
    while (slot <= MAX_PROCESS) {
        if (!mRunningProcess.count(slot))
            break;
        ++slot;
    }
    return slot;
}

static String getWebProcessClassName(int32_t pid)
{
    return classT<WebKitProcess>().getPackage().getName() + String(L".WebKitWebProcess") + std::to_wstring(pid);
}

std::passed_ptr<WebKitProcessLauncher::Connection> WebKitProcessLauncher::getConnection(int32_t pid)
{
    return mRunningProcess[pid];
}

bool WebKitProcessLauncher::isConnected(int32_t pid)
{
    return mRunningProcess.count(pid) && mRunningProcess[pid]->mIsOpen;
}

int32_t WebKitProcessLauncher::connect(int32_t processType, int32_t processStrategy, int32_t connector, std::passed_ptr<AWKFunction> didFinishLaunchingProcess)
{
    int32_t slot = getAvailableWebProcessConnection();
    if (slot > MAX_PROCESS) {
        LOGE("Can't create another process");
        return 0;
    }

    Intent intent;
    intent.setClassName(WebViewPrivate::getApplicationContext().getPackageName(), getWebProcessClassName(slot));

    assert(!mRunningProcess.count(slot));
    mRunningProcess[slot] = std::shared_ptr<Connection>(new Connection(*this, slot, intent, processType, connector));
    mDidFinishLaunchingProcess[slot] = didFinishLaunchingProcess;

    std::shared_ptr<Connection>& connection = mRunningProcess[slot];
    if (!WebViewPrivate::getApplicationContext().bindService(intent, connection, Context::BIND_AUTO_CREATE)) {
        LOGE("Couldn't connect to service");
        mRunningProcess.erase(slot);
        mDidFinishLaunchingProcess.erase(slot);
        return 0;
    }

    return slot;
}

bool WebKitProcessLauncher::disconnect(int32_t pid)
{
    if (!isConnected(pid))
        return true;

    WebViewPrivate::getApplicationContext().unbindService(mRunningProcess[pid]);
    mRunningProcess.erase(pid);
    return true;
}

} // namespace launcher
} // namespace webkit
} // namespace org
