/*
 * Copyright (C) 2014-2016 NAVER Corp. All rights reserved.
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
#include "AWKSecurityOriginNatives.h"

#include "APISecurityOrigin.h"
#include "AWKSharedNativesCast.h"
#include "StringUtilities.h"

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKSecurityOriginPrivate : public AWKSecurityOrigin::Private {
public:
    RefPtr<API::SecurityOrigin> m_securityOrigin;
};

template<> JNI::PassLocalRef<AWKSecurityOrigin> AWKSecurityOrigin::create(API::SecurityOrigin* securityOrigin)
{
    JNI::LocalRef<AWKSecurityOrigin> ref = AWKSecurityOrigin::create();
    ref->p<AWKSecurityOriginPrivate>().m_securityOrigin = securityOrigin;
    return ref.release();
}

template<> API::SecurityOrigin& AWKSecurityOrigin::castTo()
{
    return *p<AWKSecurityOriginPrivate>().m_securityOrigin;
}

template<> const WebCore::SecurityOrigin& AWKSecurityOrigin::castTo()
{
    return core(this).securityOrigin();
}

std::string AWKSecurityOrigin::getProtocol()
{
    return stdStringFromWebCoreString(core(this).securityOrigin().protocol());
}

std::string AWKSecurityOrigin::getHost()
{
    return stdStringFromWebCoreString(core(this).securityOrigin().host());
}

int32_t AWKSecurityOrigin::getPort()
{
    if (auto port = core(this).securityOrigin().port())
        return *port;

    return -1;
}

AWKSecurityOrigin* AWKSecurityOrigin::nativeCreate()
{
    std::unique_ptr<AWKSecurityOrigin> ptr(new AWKSecurityOrigin);
    ptr->m_private.reset(new AWKSecurityOriginPrivate);
    return ptr.release();
}

} // namespace Natives
} // namespace webkit
} // namespace org
