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

#include <android/content/Intent.h>
#include <android/content/ServiceConnection.h>
#include <android/os/Message.h>
#include <android/os/Messenger.h>
#include <org/webkit/Managed/AWKFunction.h>

#include <unordered_set>

namespace org {
namespace webkit {

using AWKFunction = Managed::AWKFunction;

namespace launcher {

class MessageHost;

class WebKitProcessLauncher final {
public:
    ~WebKitProcessLauncher();

    using MessageCallback = std::function<void (Message&)>;

    class Connection : public ServiceConnection {
        friend class ConnectionHandler;
        friend class WebKitProcessLauncher;
    public:
        ~Connection();

        int32_t getPid();
        Intent& getIntent();

        void send(Message&);
        void addMessageHost(std::passed_ptr<MessageHost> host);

        // ServiceConnection
        void onServiceConnected(ComponentName& name, std::passed_ptr<IBinder> service) override;
        void onServiceDisconnected(ComponentName& name) override;

    protected:
        Connection(WebKitProcessLauncher&, int32_t, Intent&, int32_t, int32_t);

        WebKitProcessLauncher& mThat;
        int32_t mPid;
        Intent mIntent;
        int32_t mProcessType;
        int32_t mConnector;
        bool mIsOpen { false };
        std::shared_ptr<Handler> mMessengerHandler;
        std::shared_ptr<Messenger> mMessageReceiver;
        std::shared_ptr<Messenger> mMessageSender;
        std::unordered_map<int32_t, std::shared_ptr<MessageHost>> mMessageHosts;
    };
    friend class Connection;

    static const int32_t MAX_PROCESS = 8;

    static WebKitProcessLauncher& get();

    std::passed_ptr<Connection> getConnection(int32_t);
    bool isConnected(int32_t);
    int32_t connect(int32_t processType, int32_t processStrategy, int32_t connector, std::passed_ptr<AWKFunction> didFinishLaunchingProcess);
    bool disconnect(int32_t);

private:
    WebKitProcessLauncher();

    int32_t getAvailableWebProcessConnection();

    std::unordered_map<int32_t, std::shared_ptr<Connection>> mRunningProcess;
    std::unordered_map<int32_t, std::shared_ptr<AWKFunction>> mDidFinishLaunchingProcess;
};

} // namespace launcher
} // namespace webkit
} // namespace org

using WebKitProcessLauncher = org::webkit::launcher::WebKitProcessLauncher;
