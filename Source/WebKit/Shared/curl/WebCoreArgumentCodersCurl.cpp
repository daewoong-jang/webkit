/*
 * Copyright (C) 2013 NAVER Corp. All rights reserved.
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
#include "WebCoreArgumentCoders.h"

#if USE(CURL)

#include "NotImplemented.h"
#include <WebCore/CertificateInfo.h>
#include <WebCore/ResourceError.h>
#include <WebCore/ResourceRequest.h>
#include <WebCore/ResourceResponse.h>
#include <wtf/text/CString.h>

using namespace WebCore;

namespace IPC {

void ArgumentCoder<ResourceRequest>::encodePlatformData(Encoder& encoder, const ResourceRequest& resourceRequest)
{
    encoder << resourceRequest.url().string();
    encoder << resourceRequest.httpMethod();
    encoder << resourceRequest.httpHeaderFields();

    // FIXME: Do not encode HTTP message body.
    // 1. It can be large and thus costly to send across.
    // 2. It is misleading to provide a body with some requests, while others use body streams, which cannot be serialized at all.
    FormData* httpBody = resourceRequest.httpBody();
    encoder << static_cast<bool>(httpBody);
    if (httpBody)
        encoder << httpBody->flattenToString();

    encoder << resourceRequest.firstPartyForCookies().string();
}

bool ArgumentCoder<ResourceRequest>::decodePlatformData(Decoder& decoder, ResourceRequest& resourceRequest)
{
    String url;
    if (!decoder.decode(url))
        return false;
    resourceRequest.setURL(URL(URL(), url));

    String httpMethod;
    if (!decoder.decode(httpMethod))
        return false;
    resourceRequest.setHTTPMethod(httpMethod);

    HTTPHeaderMap headers;
    if (!decoder.decode(headers))
        return false;
    resourceRequest.setHTTPHeaderFields(WTFMove(headers));

    bool hasHTTPBody;
    if (!decoder.decode(hasHTTPBody))
        return false;
    if (hasHTTPBody) {
        String httpBody;
        if (!decoder.decode(httpBody))
            return false;
        resourceRequest.setHTTPBody(FormData::create(httpBody.utf8()));
    }

    String firstPartyForCookies;
    if (!decoder.decode(firstPartyForCookies))
        return false;
    resourceRequest.setFirstPartyForCookies(URL(URL(), firstPartyForCookies));

    return true;
}

void ArgumentCoder<CertificateInfo>::encode(Encoder& encoder, const CertificateInfo& certificateInfo)
{
    notImplemented();
}

bool ArgumentCoder<CertificateInfo>::decode(Decoder& decoder, CertificateInfo& certificateInfo)
{
    notImplemented();
    return true;
}

void ArgumentCoder<ResourceError>::encodePlatformData(Encoder& encoder, const ResourceError& resourceError)
{
    encoder.encodeEnum(resourceError.type());
    if (resourceError.isNull())
        return;

    encoder << resourceError.domain();
    encoder << resourceError.errorCode();
    encoder << resourceError.failingURL().string();
    encoder << resourceError.localizedDescription();
}

bool ArgumentCoder<ResourceError>::decodePlatformData(Decoder& decoder, ResourceError& resourceError)
{
    ResourceErrorBase::Type errorType;
    if (!decoder.decodeEnum(errorType))
        return false;
    if (errorType == ResourceErrorBase::Type::Null) {
        resourceError = { };
        return true;
    }

    String domain;
    if (!decoder.decode(domain))
        return false;

    int errorCode;
    if (!decoder.decode(errorCode))
        return false;

    String failingURL;
    if (!decoder.decode(failingURL))
        return false;

    String localizedDescription;
    if (!decoder.decode(localizedDescription))
        return false;

    resourceError = ResourceError(domain, errorCode, URL(URL(), failingURL), localizedDescription);
    resourceError.setType(errorType);
    return true;
}

void ArgumentCoder<ProtectionSpace>::encodePlatformData(Encoder&, const WebCore::ProtectionSpace&)
{
    notImplemented();
}

bool ArgumentCoder<ProtectionSpace>::decodePlatformData(Decoder&, WebCore::ProtectionSpace&)
{
    notImplemented();
    return true;
}

void ArgumentCoder<Credential>::encodePlatformData(Encoder&, const WebCore::Credential&)
{
    notImplemented();
}

bool ArgumentCoder<Credential>::decodePlatformData(Decoder&, WebCore::Credential&)
{
    notImplemented();
    return true;
}

}

#endif // USE(CURL)
