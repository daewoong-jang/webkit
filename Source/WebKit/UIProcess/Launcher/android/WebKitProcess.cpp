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
#include "WebKitProcess.h"

#include <wtf/ExportMacros.h>

#include "MessageChannel.h"
#include "ProcessLauncherBackend.h"
#include "WTFMainThread.h"
#include <android/os/Looper.h>
#include <android/os/ParcelFileDescriptor.h>
#include <android/os/Process.h>
#include <android++/LogHelper.h>

namespace org {
namespace webkit {
namespace launcher {

class WebKitProcessHandler : public Handler {
public:
    void handleMessage(Message& msg) override
    {
        switch (msg.what) {
        case WebKitProcess::START_PROCESS: {
            mThat.mMessageSender.reset(msg.replyTo);
            msg.replyTo = nullptr;
            Bundle bundle = msg.getData();

            int32_t processType = bundle.getInt(L"processType", ProcessLauncherBackend::PROCESS_TYPE_WEB);
            auto fd = bundle.getParcelable(L"connector");
            if (!fd || fd->describeContents() != Parcelable::CONTENTS_FILE_DESCRIPTOR) {
                LOGA("No file descriptor to send IPC message through");
                return;
            }

            int32_t connector = std::static_pointer_cast<ParcelFileDescriptor>(fd)->detachFd();
            if (connector <= 0) {
                LOGA("File descriptor is invalid");
                return;
            }

            mThat.runWebKitMain(processType, connector);
            break;
        }
        default:
            if (!mThat.receiveReplyMessage(msg) && msg.arg1 != 0) {
                auto channel = mThat.mMessageChannels[msg.arg1];
                if (channel != nullptr)
                    channel->receiveMessage(msg);
            }
            Handler::handleMessage(msg);
        }
    }

    WebKitProcessHandler(WebKitProcess& that)
        : mThat(that)
    {
    }

    WebKitProcess& mThat;
};

static WebKitProcess* mWebKitProcess = nullptr;

WebKitProcess::WebKitProcess()
    : mMessengerHandler(std::make_shared<WebKitProcessHandler>(*this))
    , mMessageReceiver(std::make_shared<Messenger>(mMessengerHandler))
{
    Messages::registerMessages();
    WTFMainThread::initializeUIThread();
}

WebKitProcess::~WebKitProcess()
{
}

WebKitProcess* WebKitProcess::getCurrent()
{
    return mWebKitProcess;
}

Intent& WebKitProcess::getIntent()
{
    return mIntent;
}

void WebKitProcess::addMessageChannel(MessageChannel& channel)
{
    mMessageChannels[channel.getUid()] = &channel;
}

void WebKitProcess::removeMessageChannel(int32_t uid)
{
    mMessageChannels.erase(uid);
}

void WebKitProcess::runWebKitMain(int32_t processType, int32_t connector)
{
    mMainThread = std::make_unique<std::thread>([=] {
        Looper::prepare();
        WTFMainThread::initializeUIThread();
        ProcessLauncherBackend::runWebKitMain(processType, connector);
        Looper::loop();
    });
    mMainThread->detach();
}

void WebKitProcess::sendMessage(Message& msg)
{
    mMessageSender->send(msg);
}

void WebKitProcess::sendSyncMessage(Message& msg, int32_t want, MessageCallback& callback)
{
    synchronized (this) {
        mWant = want;
        mMessageSender->send(msg);
        this->wait();
        callback(Message::obtain(nullptr, mWant, mReply.first,
                mReply.second));
    }
}

bool WebKitProcess::receiveReplyMessage(Message& msg)
{
    synchronized (this) {
        if (msg.what == mWant) {
            mReply.first = msg.arg1;
            mReply.second = msg.arg2;
            this->notifyAll();
            return true;
        }
        return false;
    }
}

std::shared_ptr<IBinder> WebKitProcess::onBind(Intent& intent)
{
    mIntent = intent;
    return mMessageReceiver->getBinder();
}

void WebKitProcess::onConfigurationChanged(Configuration& newConfig)
{
    Service::onConfigurationChanged(newConfig);
}

void WebKitProcess::onCreate()
{
    mWebKitProcess = this;
}

void WebKitProcess::onDestroy()
{
    Process::killProcess(Process::myPid());
}

void WebKitProcess::onLowMemory()
{
    Service::onLowMemory();
}

void WebKitProcess::onRebind(Intent& intent)
{
    Service::onRebind(intent);
}

int32_t WebKitProcess::onStartCommand(Intent& intent, int32_t flags, int32_t startId)
{
    return Service::onStartCommand(intent, flags, startId);
}

void WebKitProcess::onTaskRemoved(Intent& rootIntent)
{
    Service::onTaskRemoved(rootIntent);
}

void WebKitProcess::onTrimMemory(int32_t level)
{
    Service::onTrimMemory(level);
}

bool WebKitProcess::onUnbind(Intent& intent)
{
    Service::onUnbind(intent);
    stopSelf();
    return false;
}

} // namespace launcher
} // namespace webkit
} // namespace org
