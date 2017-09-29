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
#include "AWKNavigationDelegate.h"

namespace org {
namespace webkit {
namespace Managed {

void AWKNavigationDelegate::didCommitNavigation(const std::shared_ptr<AWKWebContent>& webContent, const std::shared_ptr<AWKNavigation>& navigation)
{
}

void AWKNavigationDelegate::didStartProvisionalNavigation(const std::shared_ptr<AWKWebContent>& webContent, const std::shared_ptr<AWKNavigation>& navigation)
{
}

void AWKNavigationDelegate::didReceiveServerRedirectForProvisionalNavigation(const std::shared_ptr<AWKWebContent>& webContent, const std::shared_ptr<AWKNavigation>& navigation)
{
}

void AWKNavigationDelegate::didFailNavigation(const std::shared_ptr<AWKWebContent>& webContent, const std::shared_ptr<AWKNavigation>& navigation, const std::shared_ptr<AWKError>& error)
{
}

void AWKNavigationDelegate::didFailProvisionalNavigation(const std::shared_ptr<AWKWebContent>& webContent, const std::shared_ptr<AWKNavigation>& navigation, const std::shared_ptr<AWKError>& error)
{
}

void AWKNavigationDelegate::didFinishNavigation(const std::shared_ptr<AWKWebContent>& webContent, const std::shared_ptr<AWKNavigation>& navigation)
{
}

void AWKNavigationDelegate::webViewWebContentProcessDidTerminate(const std::shared_ptr<AWKWebContent>& webContent)
{
}

void AWKNavigationDelegate::decidePolicyForNavigationAction(const std::shared_ptr<AWKWebContent>& webContent, const std::shared_ptr<AWKNavigationAction>& navigationAction, const std::shared_ptr<AWKFunction>& decisionHandler)
{
}

void AWKNavigationDelegate::decidePolicyForNavigationResponse(const std::shared_ptr<AWKWebContent>& webContent, const std::shared_ptr<AWKNavigationResponse>& navigationResponse, const std::shared_ptr<AWKFunction>&  decisionHandler)
{
}

void AWKNavigationDelegate::INIT()
{
}

}
} // namespace webkit
} // namespace org
