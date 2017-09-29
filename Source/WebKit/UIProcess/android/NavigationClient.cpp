/*
 * Copyright (C) 2017 NAVER Corp. All rights reserved.
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
#include "NavigationClient.h"

#include "AWKErrorNatives.h"
#include "AWKFunctionNatives.h"
#include "AWKNavigationActionNatives.h"
#include "AWKNavigationNatives.h"
#include "AWKNavigationResponseNatives.h"
#include "AWKWebContentNatives.h"
#include "WebNavigationState.h"

using namespace WebCore;
using namespace org::webkit;

namespace WebKit {

static JNI::PassLocalRef<AWKNavigation> createNavigation(WebPageProxy& page, API::Navigation* navigation)
{
    return navigation ? AWKNavigation::create(navigation) : AWKNavigation::create(API::Navigation::create(page.navigationState()).ptr());
}

NavigationClient::NavigationClient(AWKWebContent& webContent, JNI::PassLocalRef<AWKNavigationDelegate> delegate)
    : m_webContent(webContent)
    , m_delegate(delegate)
{
}

void NavigationClient::didStartProvisionalNavigation(WebPageProxy& page, API::Navigation* navigation, API::Object*)
{
    if (!m_delegate)
        return;

    m_delegate->didStartProvisionalNavigation(&m_webContent, createNavigation(page, navigation));
}

void NavigationClient::didReceiveServerRedirectForProvisionalNavigation(WebPageProxy& page, API::Navigation* navigation, API::Object*)
{
    if (!m_delegate)
        return;

    m_delegate->didReceiveServerRedirectForProvisionalNavigation(&m_webContent, createNavigation(page, navigation));
}

void NavigationClient::didFailProvisionalNavigationWithError(WebPageProxy& page, WebFrameProxy&, API::Navigation* navigation, const ResourceError& resourceError, API::Object*)
{
    if (!m_delegate)
        return;

    auto error = API::Error::create(resourceError);
    m_delegate->didFailProvisionalNavigation(&m_webContent, createNavigation(page, navigation), AWKError::create(error.ptr()));
}

void NavigationClient::didCommitNavigation(WebPageProxy& page, API::Navigation* navigation, API::Object*)
{
    if (!m_delegate)
        return;

    m_delegate->didCommitNavigation(&m_webContent, createNavigation(page, navigation));
}

void NavigationClient::didFinishNavigation(WebPageProxy& page, API::Navigation* navigation, API::Object*)
{
    if (!m_delegate)
        return;

    m_delegate->didFinishNavigation(&m_webContent, createNavigation(page, navigation));
}

void NavigationClient::didFailNavigationWithError(WebPageProxy& page, WebFrameProxy&, API::Navigation* navigation, const ResourceError& resourceError, API::Object*)
{
    if (!m_delegate)
        return;

    auto error = API::Error::create(resourceError);
    m_delegate->didFailNavigation(&m_webContent, createNavigation(page, navigation), AWKError::create(error.ptr()));
}

void NavigationClient::processDidTerminate(WebKit::WebPageProxy&, WebKit::ProcessTerminationReason)
{
    if (!m_delegate)
        return;

    m_delegate->webViewWebContentProcessDidTerminate(&m_webContent);
}

void NavigationClient::decidePolicyForNavigationAction(WebPageProxy&, Ref<API::NavigationAction>&& navigationAction, Ref<WebFramePolicyListenerProxy>&& listener, API::Object*)
{
    if (!m_delegate)
        return;

    RefPtr<WebFramePolicyListenerProxy> localListener = WTFMove(listener);
    m_delegate->decidePolicyForNavigationAction(&m_webContent, AWKNavigationAction::create(navigationAction.ptr()),
        JNI::wrap<AWKFunction>(lambda([localListener] (int32_t policy) {
            switch (policy) {
            case AWKNavigationDelegate::NAVIGATION_ACTION_POLICY_CANCEL:
                localListener->ignore();
                break;
            case AWKNavigationDelegate::NAVIGATION_ACTION_POLICY_ALLOW:
                localListener->use(WebsitePolicies());
                break;
            default:
                ASSERT_NOT_REACHED();
                break;
            }
        }))
    );
}

void NavigationClient::decidePolicyForNavigationResponse(WebPageProxy&, API::NavigationResponse& navigationResponse, Ref<WebFramePolicyListenerProxy>&& listener, API::Object*)
{
    if (!m_delegate)
        return;

    RefPtr<WebFramePolicyListenerProxy> localListener = WTFMove(listener);
    m_delegate->decidePolicyForNavigationResponse(&m_webContent, AWKNavigationResponse::create(&navigationResponse),
        JNI::wrap<AWKFunction>(lambda([localListener] (int32_t policy) {
            switch (policy) {
            case AWKNavigationDelegate::NAVIGATION_RESPONSE_POLICY_CANCEL:
                localListener->ignore();
                break;
            case AWKNavigationDelegate::NAVIGATION_RESPONSE_POLICY_ALLOW:
                localListener->use(WebsitePolicies());
                break;
            default:
                ASSERT_NOT_REACHED();
                break;
            }
        }))
    );
}

} // namespace WebKit
