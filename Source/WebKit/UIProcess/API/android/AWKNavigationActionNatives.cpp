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
#include "AWKNavigationActionNatives.h"

#include "APINavigationAction.h"
#include "AWKNativesCast.h"

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKNavigationActionPrivate : public AWKNavigationAction::Private {
public:
    RefPtr<API::NavigationAction> m_navigation;
};

template<> JNI::PassLocalRef<AWKNavigationAction> AWKNavigationAction::create(API::NavigationAction* navigation)
{
    JNI::LocalRef<AWKNavigationAction> ref = AWKNavigationAction::create();
    ref->p<AWKNavigationActionPrivate>().m_navigation = navigation;
    return ref.release();
}

template<> API::NavigationAction& AWKNavigationAction::castTo()
{
    return *p<AWKNavigationActionPrivate>().m_navigation;
}

JNI::PassLocalRef<AWKFrameInfo> AWKNavigationAction::getSourceFrame()
{
    return JNI::wrap<AWKFrameInfo>(core(this).sourceFrame());
}

JNI::PassLocalRef<AWKFrameInfo> AWKNavigationAction::getTargetFrame()
{
    return JNI::wrap<AWKFrameInfo>(core(this).targetFrame());
}

static int32_t toAWK(WebCore::NavigationType navigationType)
{
    switch (navigationType) {
    case WebCore::NavigationType::LinkClicked:
        return AWKNavigationAction::NAVIGATION_TYPE_LINK_CLICKED;
    case WebCore::NavigationType::FormSubmitted:
        return AWKNavigationAction::NAVIGATION_TYPE_FORM_SUBMITTED;
    case WebCore::NavigationType::BackForward:
        return AWKNavigationAction::NAVIGATION_TYPE_BACK_FORWARD;
    case WebCore::NavigationType::Reload:
        return AWKNavigationAction::NAVIGATION_TYPE_RELOAD;
    case WebCore::NavigationType::FormResubmitted:
        return AWKNavigationAction::NAVIGATION_TYPE_FORM_RESUBMITTED;
    case WebCore::NavigationType::Other:
        return AWKNavigationAction::NAVIGATION_TYPE_OTHER;
    default:
        break;
    }

    ASSERT_NOT_REACHED();
    return AWKNavigationAction::NAVIGATION_TYPE_LINK_NONE;
}

int32_t AWKNavigationAction::getNavigationType()
{
    return toAWK(core(this).navigationType());
}

JNI::PassLocalRef<AWKURLRequest> AWKNavigationAction::getRequest()
{
    return JNI::wrap<AWKURLRequest>(core(this).request());
}

int32_t AWKNavigationAction::getModifierFlags()
{
    return core(this).modifiers();
}

int32_t AWKNavigationAction::getButtonNumber()
{
    return core(this).mouseButton();
}

AWKNavigationAction* AWKNavigationAction::nativeCreate()
{
    std::unique_ptr<AWKNavigationAction> ptr(new AWKNavigationAction);
    ptr->m_private.reset(new AWKNavigationActionPrivate);
    return ptr.release();
}

} // namespace Natives
} // namespace webkit
} // namespace org
