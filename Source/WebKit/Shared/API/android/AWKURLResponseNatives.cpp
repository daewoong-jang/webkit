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
#include "AWKURLResponseNatives.h"

#include "APIURLResponse.h"
#include "AWKSharedNativesCast.h"
#include "StringUtilities.h"
#include <WebCore/ResourceResponse.h>

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKURLResponsePrivate : public AWKURLResponse::Private {
public:
    RefPtr<API::URLResponse> m_urlResponse;
};

template<> JNI::PassLocalRef<AWKURLResponse> AWKURLResponse::create(API::URLResponse* urlResponse)
{
    JNI::LocalRef<AWKURLResponse> ref = AWKURLResponse::create();
    ref->p<AWKURLResponsePrivate>().m_urlResponse = urlResponse;
    return ref.release();
}

template<> JNI::PassLocalRef<AWKURLResponse> AWKURLResponse::create(const WebCore::ResourceResponse& resourceResponse)
{
    JNI::LocalRef<AWKURLResponse> ref = AWKURLResponse::create();
    ref->p<AWKURLResponsePrivate>().m_urlResponse = API::URLResponse::create(resourceResponse);
    return ref.release();
}

template<> API::URLResponse& AWKURLResponse::castTo()
{
    return *p<AWKURLResponsePrivate>().m_urlResponse;
}

template<> const WebCore::ResourceResponse& AWKURLResponse::castTo()
{
    return core(this).resourceResponse();
}

int64_t AWKURLResponse::getExpectedContentLength()
{
    return core(this).resourceResponse().expectedContentLength();
}

std::string AWKURLResponse::getSuggestedFilename()
{
    return stdStringFromWebCoreString(core(this).resourceResponse().suggestedFilename());
}

std::string AWKURLResponse::getMIMEType()
{
    return stdStringFromWebCoreString(core(this).resourceResponse().mimeType());
}

std::string AWKURLResponse::getTextEncodingName()
{
    return stdStringFromWebCoreString(core(this).resourceResponse().textEncodingName());
}

JNI::PassLocalRef<AWKURL> AWKURLResponse::getURL()
{
    return JNI::wrap<AWKURL>(core(this).resourceResponse().url());
}

AWKURLResponse* AWKURLResponse::nativeCreate()
{
    std::unique_ptr<AWKURLResponse> ptr(new AWKURLResponse);
    ptr->m_private.reset(new AWKURLResponsePrivate);
    return ptr.release();
}

}
} // namespace webkit
} // namespace org
