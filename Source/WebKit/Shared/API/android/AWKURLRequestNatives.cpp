/*
 * Copyright (C) 2014-2015 NAVER Corp. All rights reserved.
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
#include "AWKURLRequestNatives.h"

#include "APIURLRequest.h"
#include "AWKSharedNativesCast.h"
#include "StringUtilities.h"

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKURLRequestPrivate : public AWKURLRequest::Private {
public:
    RefPtr<API::URLRequest> m_urlRequest;
};

template<> JNI::PassLocalRef<AWKURLRequest> AWKURLRequest::create(API::URLRequest* urlRequest)
{
    JNI::LocalRef<AWKURLRequest> ref = AWKURLRequest::create();
    ref->p<AWKURLRequestPrivate>().m_urlRequest = urlRequest;
    return ref.release();
}

template<> JNI::PassLocalRef<AWKURLRequest> AWKURLRequest::create(const WebCore::ResourceRequest& resourceRequest)
{
    JNI::LocalRef<AWKURLRequest> ref = AWKURLRequest::create();
    ref->p<AWKURLRequestPrivate>().m_urlRequest = API::URLRequest::create(resourceRequest);
    return ref.release();
}

template<> API::URLRequest& AWKURLRequest::castTo()
{
    return *p<AWKURLRequestPrivate>().m_urlRequest;
}

template<> const WebCore::ResourceRequest& AWKURLRequest::castTo()
{
    return core(this).resourceRequest();
}

JNI::PassLocalRef<AWKURL> AWKURLRequest::getURL()
{
    return JNI::wrap<AWKURL>(core(this).resourceRequest().url());
}

JNI::PassLocalRef<AWKURL> AWKURLRequest::getFirstPartyForCookies()
{
    return JNI::wrap<AWKURL>(core(this).resourceRequest().firstPartyForCookies());
}

std::string AWKURLRequest::getHTTPMethod()
{
    return stdStringFromWebCoreString(core(this).resourceRequest().httpMethod());
}

AWKURLRequest* AWKURLRequest::nativeCreate()
{
    std::unique_ptr<AWKURLRequest> ptr(new AWKURLRequest);
    ptr->m_private.reset(new AWKURLRequestPrivate);
    return ptr.release();
}

AWKURLRequest* AWKURLRequest::nativeCreateWithURL(JNI::PassLocalRef<AWKURL> url)
{
    std::unique_ptr<AWKURLRequest> ptr(new AWKURLRequest);
    ptr->m_private.reset(new AWKURLRequestPrivate);
    ptr->p<AWKURLRequestPrivate>().m_urlRequest = API::URLRequest::create(WebCore::ResourceRequest(core(url)));
    return ptr.release();
}

}
} // namespace webkit
} // namespace org
