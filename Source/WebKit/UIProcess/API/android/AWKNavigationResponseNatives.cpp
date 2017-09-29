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
#include "AWKNavigationResponseNatives.h"

#include "APINavigationResponse.h"
#include "AWKNativesCast.h"
#include <WebCore/ResourceRequest.h>

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKNavigationResponsePrivate : public AWKNavigationResponse::Private {
public:
    RefPtr<API::NavigationResponse> m_navigationResponse;
};

template<> JNI::PassLocalRef<AWKNavigationResponse> AWKNavigationResponse::create(API::NavigationResponse* navigationResponse)
{
    JNI::LocalRef<AWKNavigationResponse> ref = AWKNavigationResponse::create();
    ref->p<AWKNavigationResponsePrivate>().m_navigationResponse = navigationResponse;
    return ref.release();
}

template<> API::NavigationResponse& AWKNavigationResponse::castTo()
{
    return *p<AWKNavigationResponsePrivate>().m_navigationResponse;
}

bool AWKNavigationResponse::isForMainFrame()
{
    return core(this).frame().isMainFrame();
}

JNI::PassLocalRef<AWKURLResponse> AWKNavigationResponse::getResponse()
{
    return JNI::wrap<AWKURLResponse>(core(this).response());
}

bool AWKNavigationResponse::canShowMIMEType()
{
    return core(this).canShowMIMEType();
}

JNI::PassLocalRef<AWKFrameInfo> AWKNavigationResponse::getFrame()
{
    return JNI::wrap<AWKFrameInfo>(&core(this).frame());
}

JNI::PassLocalRef<AWKURLRequest> AWKNavigationResponse::getRequest()
{
    return JNI::wrap<AWKURLRequest>(core(this).request());
}

AWKNavigationResponse* AWKNavigationResponse::nativeCreate()
{
    std::unique_ptr<AWKNavigationResponse> ptr(new AWKNavigationResponse);
    ptr->m_private.reset(new AWKNavigationResponsePrivate);
    return ptr.release();
}

}
} // namespace webkit
} // namespace org
