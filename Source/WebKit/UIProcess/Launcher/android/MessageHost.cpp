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

#include "config.h"
#include "MessageHost.h"

#include <android++/LogHelper.h>

namespace org {
namespace webkit {
namespace launcher {

int32_t MessageHost::CHANNEL_OPENED;

int32_t MessageHost::sNextUid = 1;
std::unordered_map<int32_t, MessageHost::Constructor> MessageHost::mConstructors;

MessageHost::MessageHost()
    : mUid(sNextUid++)
{
}

int32_t MessageHost::getUid()
{
    return mUid;
}

void MessageHost::sendMessage(int32_t what, int32_t arg2)
{
    mConnection->send(Message::obtain(nullptr, what, mUid, arg2));
}

bool MessageHost::receiveMessage(WebKitProcessLauncher::Connection& connection, Message& msg)
{
    return true;
}

bool MessageHost::receiveMessageWithoutUid(WebKitProcessLauncher::Connection& connection, Message& msg)
{
    LOGA_IF(msg.arg1 == 0, "Message has uid which is not expected");
    auto host = constructHost(connection, msg);
    if (host == nullptr)
        return false;
    host->mConnection = &connection;
    connection.addMessageHost(host);
    host->sendMessage(CHANNEL_OPENED, 0);
    return true;
}

void MessageHost::registerHost(int32_t what, Constructor constructor)
{
    mConstructors[what] = constructor;
}

std::shared_ptr<MessageHost> MessageHost::constructHost(WebKitProcessLauncher::Connection& connection, Message& msg)
{
    if (mConstructors.count(msg.what) == 0)
        return nullptr;
    return mConstructors[msg.what](connection, msg);
}

void MessageHost::registerMessages()
{
    CHANNEL_OPENED = Messages::registerMessage();
}

} // namespace launcher
} // namespace webkit
} // namespace org
