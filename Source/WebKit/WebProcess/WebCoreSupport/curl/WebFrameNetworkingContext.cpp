/*
 * Copyright (C) 2015 NAVER Corp. All rights reserved.
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
#include "WebFrameNetworkingContext.h"

#include "SessionTracker.h"
#include "WebFrame.h"
#include "WebPage.h"
#include <WebCore/FrameLoader.h>
#include <WebCore/NetworkStorageSession.h>
#include <WebCore/NotImplemented.h>

using namespace PAL;
using namespace WebCore;

namespace WebKit {

WebFrameNetworkingContext::WebFrameNetworkingContext(WebKit::WebFrame* frame)
    : FrameNetworkingContext(frame->coreFrame())
{
}

void WebFrameNetworkingContext::ensurePrivateBrowsingSession(SessionID sessionID)
{
    ASSERT(isMainThread());
    ASSERT(sessionID.isEphemeral());

    if (NetworkStorageSession::storageSession(sessionID))
        return;

    NetworkStorageSession::ensurePrivateBrowsingSession(sessionID, String::number(sessionID.sessionID()));
#if USE(NETWORK_SESSION)
    SessionTracker::setSession(sessionID, NetworkSession::create(sessionID));
#endif
}

void WebFrameNetworkingContext::ensureWebsiteDataStoreSession(WebsiteDataStoreParameters&&)
{
    notImplemented();
}

void WebFrameNetworkingContext::setCookieAcceptPolicyForAllContexts(HTTPCookieAcceptPolicy policy)
{
    notImplemented();
}

WebFrameLoaderClient* WebFrameNetworkingContext::webFrameLoaderClient()
{
    if (!frame())
        return nullptr;

    return toWebFrameLoaderClient(frame()->loader().client());
}

NetworkStorageSession& WebFrameNetworkingContext::storageSession() const
{
    if (frame() && frame()->page()->usesEphemeralSession())
        return *NetworkStorageSession::storageSession(SessionID::legacyPrivateSessionID());

    return NetworkStorageSession::defaultStorageSession();
}

}
