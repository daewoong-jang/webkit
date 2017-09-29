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
#include "SurfaceTextureFactory.h"

namespace org {
namespace webkit {

class SurfaceTextureFactory::Channel : public MessageChannel {
    friend class SurfaceTextureFactory;
public:
    Channel(SurfaceTextureFactory& that);
    ~Channel() = default;

    bool receiveMessage(Message& msg);

private:
    SurfaceTextureFactory& mThat;
};

int32_t SurfaceTextureFactory::OPEN_CHANNEL;

SurfaceTextureFactory::Channel::Channel(SurfaceTextureFactory& that)
    : MessageChannel(OPEN_CHANNEL)
    , mThat(that)
{
}

bool SurfaceTextureFactory::Channel::receiveMessage(Message& msg)
{
    return false;
}

SurfaceTextureFactory::SurfaceTextureFactory()
    : mChannel(std::make_unique<Channel>(*this))
{
}

SurfaceTextureFactory::~SurfaceTextureFactory()
{
}

void SurfaceTextureFactory::registerMessages()
{
    OPEN_CHANNEL = Messages::registerMessage();
}

} // namespace webkit
} // namespace org
