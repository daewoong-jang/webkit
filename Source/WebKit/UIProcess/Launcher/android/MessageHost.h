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

#pragma once

#include "Messages.h"
#include "WebKitProcessLauncher.h"

namespace org {
namespace webkit {
namespace launcher {

class MessageHost {
public:
    static int32_t CHANNEL_OPENED;

    using Constructor = std::function<std::shared_ptr<MessageHost> (WebKitProcessLauncher::Connection&, Message&)>;

    int32_t getUid();

    bool receiveMessage(WebKitProcessLauncher::Connection& connection, Message& msg);
    static bool receiveMessageWithoutUid(WebKitProcessLauncher::Connection& connection, Message& msg);

    static void registerHost(int32_t what, Constructor constructor);
    static std::shared_ptr<MessageHost> constructHost(WebKitProcessLauncher::Connection& connection, Message& msg);

    static void registerMessages();

protected:
    MessageHost();

    void sendMessage(int32_t what, int32_t arg2);

private:
    WebKitProcessLauncher::Connection* mConnection = nullptr;
    static int32_t sNextUid;
    int32_t mUid = 0;
    static std::unordered_map<int32_t, Constructor> mConstructors;
};

} // namespace launcher
} // namespace webkit
} // namespace org

using MessageHost = org::webkit::launcher::MessageHost;
