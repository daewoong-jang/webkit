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
#include "AWKUIDelegate.h"

namespace org {
namespace webkit {
namespace Managed {

std::shared_ptr<AWKWebContent> AWKUIDelegate::createWebViewWithConfiguration(const std::shared_ptr<AWKWebContentConfiguration>& configuration, const std::shared_ptr<AWKNavigationAction>& navigationAction, const std::shared_ptr<AWKWindowFeatures>& windowFeatures)
{
    return nullptr;
}

bool AWKUIDelegate::runJavaScriptAlertPanelWithMessage(const std::shared_ptr<AWKWebContent>& webContent, const std::string& message, const std::shared_ptr<AWKFrameInfo>& frame, const std::shared_ptr<AWKFunction>& completionHandler)
{
    return false;
}

bool AWKUIDelegate::runJavaScriptConfirmPanelWithMessage(const std::shared_ptr<AWKWebContent>& webContent, const std::string& message, const std::shared_ptr<AWKFrameInfo>& frame, const std::shared_ptr<AWKFunction>& completionHandler)
{
    return false;
}

bool AWKUIDelegate::runBeforeUnloadConfirmPanelWithMessage(const std::shared_ptr<AWKWebContent>& webContent, const std::string& message, const std::shared_ptr<AWKFrameInfo>& frame, const std::shared_ptr<AWKFunction>& completionHandler)
{
    return false;
}

bool AWKUIDelegate::runJavaScriptTextInputPanelWithPrompt(const std::shared_ptr<AWKWebContent>& webContent, const std::string& prompt, const std::string& defaultText, const std::shared_ptr<AWKFrameInfo>& frame, const std::shared_ptr<AWKFunction>& completionHandler)
{
    return false;
}

void AWKUIDelegate::webViewDidClose(const std::shared_ptr<AWKWebContent>& webContent)
{
}

bool AWKUIDelegate::runOpenPanelWithParameters(const std::shared_ptr<AWKOpenPanelParameters>& parameters, const std::shared_ptr<AWKFrameInfo>& frame, const std::shared_ptr<AWKFunction>& completionHandler)
{
    return false;
}

bool AWKUIDelegate::shouldPreviewElement(const std::shared_ptr<AWKPreviewElementInfo>& elementInfo)
{
    return false;
}

std::shared_ptr<AWKWebContent> AWKUIDelegate::previewingViewControllerForElement(const std::shared_ptr<AWKPreviewElementInfo>& elementInfo, const std::shared_ptr<Vector>& previewActions)
{
    return nullptr;
}

void AWKUIDelegate::commitPreviewingViewController(const std::shared_ptr<AWKWebContent>& previewingViewController)
{
}

void AWKUIDelegate::INIT()
{
}

} // namespace Managed
} // namespace webkit
} // namespace org
