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
#include "WebKitProcess.h"

namespace org {
namespace webkit {
namespace launcher {

class MessageChannel {
public:
    using MessageCallback = WebKitProcess::MessageCallback;

    void setUid(int32_t uid);
    int32_t getUid();

    virtual bool receiveMessage(Message& msg);

protected:
    MessageChannel(int32_t openMessage);
    virtual ~MessageChannel();

    void close();

    void sendMessage(int32_t what, int32_t arg2);
    void sendMessage(int32_t what, int32_t arg2, String& data);
    void sendMessage(int32_t what, int32_t arg2, Bundle& data);

    void sendSyncMessage(int32_t what, int32_t arg2, int32_t want, MessageCallback);
    void sendSyncMessage(int32_t what, int32_t arg2, String& data, int32_t want, MessageCallback);
    void sendSyncMessage(int32_t what, int32_t arg2, Bundle& data, int32_t want, MessageCallback);

    virtual void onChannelOpen();

private:
    int32_t mUid = 0;
};

} // namespace launcher
} // namespace webkit
} // namespace org

using MessageChannel = org::webkit::launcher::MessageChannel;
