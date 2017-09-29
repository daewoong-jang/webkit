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
#include "AWKFrameInfoNatives.h"

#include "APIFrameInfo.h"
#include "AWKNativesCast.h"

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKFrameInfoPrivate : public AWKFrameInfo::Private {
public:
    RefPtr<API::FrameInfo> m_frameInfo;
};

template<> JNI::PassLocalRef<AWKFrameInfo> AWKFrameInfo::create(API::FrameInfo* frameInfo)
{
    JNI::LocalRef<AWKFrameInfo> ref = AWKFrameInfo::create();
    ref->p<AWKFrameInfoPrivate>().m_frameInfo = frameInfo;
    return ref.release();
}

template<> API::FrameInfo& AWKFrameInfo::castTo()
{
    return *p<AWKFrameInfoPrivate>().m_frameInfo;
}

bool AWKFrameInfo::isMainFrame()
{
    return core(this).isMainFrame();
}

JNI::PassLocalRef<AWKURLRequest> AWKFrameInfo::getRequest()
{
    return JNI::wrap<AWKURLRequest>(core(this).request());
}

JNI::PassLocalRef<AWKSecurityOrigin> AWKFrameInfo::getSecurityOrigin()
{
    return JNI::wrap<AWKSecurityOrigin>(&core(this).securityOrigin());
}

AWKFrameInfo* AWKFrameInfo::nativeCreate()
{
    std::unique_ptr<AWKFrameInfo> ptr(new AWKFrameInfo);
    ptr->m_private.reset(new AWKFrameInfoPrivate);
    return ptr.release();
}

}
} // namespace webkit
} // namespace org
