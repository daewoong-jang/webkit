/*
 * Copyright (C) 2014 NAVER Corp. All rights reserved.
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
#include "AWKNavigationNatives.h"

#include "APINavigation.h"
#include "AWKNativesCast.h"
#include "StringUtilities.h"
#include <WebCore/ResourceRequest.h>

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKNavigationPrivate : public AWKNavigation::Private {
public:
    RefPtr<API::Navigation> m_navigation;
};

template<> JNI::PassLocalRef<AWKNavigation> AWKNavigation::create(API::Navigation* navigation)
{
    JNI::LocalRef<AWKNavigation> ref = AWKNavigation::create();
    ref->p<AWKNavigationPrivate>().m_navigation = navigation;
    return ref.release();
}

template<> API::Navigation& AWKNavigation::castTo()
{
    return *p<AWKNavigationPrivate>().m_navigation;
}

template<> const WebCore::ResourceRequest& AWKNavigation::castTo()
{
    return core(this).request();
}

JNI::PassLocalRef<AWKURLRequest> AWKNavigation::getRequest()
{
    return JNI::wrap<AWKURLRequest>(core(this).request());
}

AWKNavigation* AWKNavigation::nativeCreate()
{
    std::unique_ptr<AWKNavigation> ptr(new AWKNavigation);
    ptr->m_private.reset(new AWKNavigationPrivate);
    return ptr.release();
}

}
} // namespace webkit
} // namespace org
