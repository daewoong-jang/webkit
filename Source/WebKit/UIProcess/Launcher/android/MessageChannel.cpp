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
#include "MessageChannel.h"

#include "MessageHost.h"
#include <android++/LogHelper.h>

namespace org {
namespace webkit {
namespace launcher {

MessageChannel::MessageChannel(int32_t openMessage)
{
    sendSyncMessage(openMessage, 0, MessageHost::CHANNEL_OPENED,
            [=] (Message& msg) {
                setUid(msg.arg1);
                WebKitProcess::getCurrent()->addMessageChannel(*this);
                onChannelOpen();
            });
}

MessageChannel::~MessageChannel()
{
    close();
}

void MessageChannel::close()
{
    WebKitProcess::getCurrent()->removeMessageChannel(mUid);
}

void MessageChannel::setUid(int32_t uid)
{
    mUid = uid;
}

int32_t MessageChannel::getUid()
{
    return mUid;
}

void MessageChannel::sendMessage(int32_t what, int32_t arg2)
{
    WebKitProcess::getCurrent()->sendMessage(
            Message::obtain(nullptr, what, mUid, arg2));
}

void MessageChannel::sendMessage(int32_t what, int32_t arg2, String& data)
{
    WebKitProcess::getCurrent()->sendMessage(
            Messages::obtain(what, mUid, arg2, data));
}

void MessageChannel::sendMessage(int32_t what, int32_t arg2, Bundle& data)
{
    WebKitProcess::getCurrent()->sendMessage(
            Messages::obtain(what, mUid, arg2, data));
}

void MessageChannel::sendSyncMessage(int32_t what, int32_t arg2, int32_t want, MessageCallback callback)
{
    WebKitProcess::getCurrent()->sendSyncMessage(
            Message::obtain(nullptr, what, mUid, arg2), want, callback);
}

void MessageChannel::sendSyncMessage(int32_t what, int32_t arg2, String& data, int32_t want, MessageCallback callback)
{
    WebKitProcess::getCurrent()->sendSyncMessage(
            Messages::obtain(what, mUid, arg2, data), want, callback);
}

void MessageChannel::sendSyncMessage(int32_t what, int32_t arg2, Bundle& data, int32_t want, MessageCallback callback)
{
    WebKitProcess::getCurrent()->sendSyncMessage(
            Messages::obtain(what, mUid, arg2, data), want, callback);
}

void MessageChannel::onChannelOpen()
{
    LOGD("MessageChannel #%d opened", mUid);
}

bool MessageChannel::receiveMessage(Message& msg)
{
    return false;
}

} // namespace launcher
} // namespace webkit
} // namespace org
