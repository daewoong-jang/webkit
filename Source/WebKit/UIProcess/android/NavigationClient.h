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

#ifndef NavigationClient_h
#define NavigationClient_h

#include "APINavigationClient.h"
#include "AWKNavigationDelegateNatives.h"
#include <androidjni/GlobalRef.h>

namespace org {
namespace webkit {
class AWKWebContent;
}
}

namespace WebKit {

class NavigationClient final : public API::NavigationClient {
public:
    NavigationClient(org::webkit::AWKWebContent&, JNI::PassLocalRef<org::webkit::AWKNavigationDelegate>);
    ~NavigationClient() = default;

private:
    void didStartProvisionalNavigation(WebPageProxy&, API::Navigation*, API::Object*) override;
    void didReceiveServerRedirectForProvisionalNavigation(WebPageProxy&, API::Navigation*, API::Object*) override;
    void didFailProvisionalNavigationWithError(WebPageProxy&, WebFrameProxy&, API::Navigation*, const WebCore::ResourceError&, API::Object*) override;
    void didCommitNavigation(WebPageProxy&, API::Navigation*, API::Object*) override;
    void didFinishNavigation(WebPageProxy&, API::Navigation*, API::Object*) override;
    void didFailNavigationWithError(WebPageProxy&, WebFrameProxy&, API::Navigation*, const WebCore::ResourceError&, API::Object*) override;

    void processDidTerminate(WebKit::WebPageProxy&, WebKit::ProcessTerminationReason) override;

    void decidePolicyForNavigationAction(WebPageProxy&, Ref<API::NavigationAction>&&, Ref<WebFramePolicyListenerProxy>&& listener, API::Object*) override;
    void decidePolicyForNavigationResponse(WebPageProxy&, API::NavigationResponse&, Ref<WebFramePolicyListenerProxy>&& listener, API::Object*) override;

    org::webkit::Natives::AWKWebContent& m_webContent;
    JNI::GlobalRef<org::webkit::AWKNavigationDelegate> m_delegate;
};

} // namespace WebKit

#endif // NavigationClient_h
