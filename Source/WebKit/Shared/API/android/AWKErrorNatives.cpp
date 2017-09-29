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
#include "AWKErrorNatives.h"

#include "APIError.h"
#include "AWKSharedNativesCast.h"
#include "StringUtilities.h"
#include <WebCore/ResourceError.h>

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKErrorPrivate : public AWKError::Private {
public:
    RefPtr<API::Error> m_error;
};

template<> JNI::PassLocalRef<AWKError> AWKError::create(API::Error* error)
{
    JNI::LocalRef<AWKError> ref = AWKError::create();
    ref->p<AWKErrorPrivate>().m_error = error;
    return ref.release();
}

template<> API::Error& AWKError::castTo()
{
    return *p<AWKErrorPrivate>().m_error;
}

template<> const WebCore::ResourceError& AWKError::castTo()
{
    return core(this).platformError();
}

std::string AWKError::getDomain()
{
    return stdStringFromWebCoreString(core(this).domain());
}

int32_t AWKError::getErrorCode()
{
    return core(this).errorCode();
}

std::string AWKError::getFailingURL()
{
    return stdStringFromWebCoreString(core(this).failingURL());
}

std::string AWKError::getLocalizedDescription()
{
    return stdStringFromWebCoreString(core(this).localizedDescription());
}

AWKError* AWKError::nativeCreate()
{
    std::unique_ptr<AWKError> ptr(new AWKError);
    ptr->m_private.reset(new AWKErrorPrivate);
    return ptr.release();
}

}
} // namespace webkit
} // namespace org
