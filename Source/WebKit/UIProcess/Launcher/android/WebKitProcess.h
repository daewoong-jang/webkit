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

#pragma once

#include <android/app/Service.h>
#include <android/os/Handler.h>
#include <android/os/Message.h>
#include <android/os/Messenger.h>

namespace org {
namespace webkit {
namespace launcher {

class MessageChannel;

class WebKitProcess : public Service {
    NONCOPYABLE(WebKitProcess);
    friend class WebKitProcessHandler;
public:
    static const int32_t START_PROCESS = 1;

    static const int32_t REQUEST_DECISION = 100000;
    static const int32_t RECEIVE_DECISION = 100001;

    static const int32_t DECISION_DENY = 0;
    static const int32_t DECISION_ALLOW = 1;

    using IntPair = std::pair<int32_t, int32_t>;

    WebKitProcess();
    virtual ~WebKitProcess();

    static WebKitProcess* getCurrent();
    Intent& getIntent();

    void addMessageChannel(MessageChannel& channel);
    void removeMessageChannel(int32_t uid);

    void runWebKitMain(int32_t processType, int32_t connector);

    using MessageCallback = std::function<void (Message&)>;

    void sendMessage(Message& msg);
    void sendSyncMessage(Message& msg, int32_t want, MessageCallback& callback);
    bool receiveReplyMessage(Message& msg);

    std::shared_ptr<IBinder> onBind(Intent& intent) override;
    void onConfigurationChanged(Configuration& newConfig) override;
    void onCreate() override;
    void onDestroy() override;
    void onLowMemory() override;
    void onRebind(Intent& intent) override;
    int32_t onStartCommand(Intent& intent, int32_t flags, int32_t startId) override;
    void onTaskRemoved(Intent& rootIntent) override;
    void onTrimMemory(int32_t level) override;
    bool onUnbind(Intent& intent) override;

protected:
    std::shared_ptr<Handler> mMessengerHandler;
    std::shared_ptr<Messenger> mMessageReceiver;
    std::shared_ptr<Messenger> mMessageSender;
    Intent mIntent;
    std::unique_ptr<std::thread> mMainThread;
    int32_t mWant { 0 };
    IntPair mReply;
    std::unordered_map<int32_t, MessageChannel*> mMessageChannels;
};

} // namespace launcher
} // namespace webkit
} // namespace org

using WebKitProcess = org::webkit::launcher::WebKitProcess;
