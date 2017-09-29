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
#include "UIClient.h"

#include "AWKFrameInfoNatives.h"
#include "AWKFunctionNatives.h"
#include "AWKOpenPanelParametersNatives.h"
#include "AWKWebContentNatives.h"
#include "StringUtilities.h"
#include "WebOpenPanelResultListenerProxy.h"

using namespace WebCore;
using namespace org::webkit;

namespace WebKit {

UIClient::UIClient(AWKWebContent& webContent, JNI::PassLocalRef<Natives::AWKUIDelegate> delegate)
    : m_webContent(webContent)
    , m_delegate(delegate)
{
}

void UIClient::createNewPage(WebPageProxy&, Ref<API::FrameInfo>&&, ResourceRequest&&, WindowFeatures&&, NavigationActionData&&, Function<void(RefPtr<WebPageProxy>&&)>&&)
{
    if (!m_delegate)
        return;

    notImplemented();
}

void UIClient::close(WebPageProxy*)
{
    if (!m_delegate)
        return;

    m_delegate->webViewDidClose(&m_webContent);
}

void UIClient::runJavaScriptAlert(WebPageProxy*, const String& message, WebFrameProxy*, const SecurityOriginData&, Function<void ()>&& completionHandler)
{
    if (!m_delegate)
        return;

    JNI::LocalRef<AWKFunction> completionFunction = JNI::wrap<AWKFunction>(WTFMove(completionHandler));
    if (!m_delegate->runJavaScriptAlertPanelWithMessage(&m_webContent, stdStringFromWebCoreString(message), AWKFrameInfo::create(), completionFunction))
        completionFunction->invoke();
}

void UIClient::runJavaScriptConfirm(WebPageProxy*, const String& message, WebFrameProxy*, const SecurityOriginData&, Function<void (bool)>&& completionHandler)
{
    if (!m_delegate)
        return;

    JNI::LocalRef<AWKFunction> completionFunction = JNI::wrap<AWKFunction>(WTFMove(completionHandler));
    if (!m_delegate->runJavaScriptConfirmPanelWithMessage(&m_webContent, stdStringFromWebCoreString(message), AWKFrameInfo::create(), completionFunction))
        completionFunction->invoke(false);
}

void UIClient::runJavaScriptPrompt(WebPageProxy*, const String& message, const String& defaultValue, WebFrameProxy*, const SecurityOriginData&, Function<void (const String&)>&& completionHandler)
{
    if (!m_delegate)
        return;

    JNI::LocalRef<AWKFunction> completionFunction = JNI::wrap<AWKFunction>(WTFMove(completionHandler));
    if (!m_delegate->runJavaScriptTextInputPanelWithPrompt(&m_webContent, stdStringFromWebCoreString(message), stdStringFromWebCoreString(defaultValue), AWKFrameInfo::create(), completionFunction))
        completionFunction->invoke(std::string(""));
}

bool UIClient::canRunBeforeUnloadConfirmPanel() const
{
    if (!m_delegate)
        return false;

    notImplemented();
    return true;
}

void UIClient::runBeforeUnloadConfirmPanel(WebPageProxy*, const String& message, WebFrameProxy*, const SecurityOriginData&, Function<void (bool)>&& completionHandler)
{
    if (!m_delegate)
        return;

    JNI::LocalRef<AWKFunction> completionFunction = JNI::wrap<AWKFunction>(WTFMove(completionHandler));
    if (!m_delegate->runBeforeUnloadConfirmPanelWithMessage(&m_webContent, stdStringFromWebCoreString(message), AWKFrameInfo::create(), completionFunction))
        completionFunction->invoke(false);
}

bool UIClient::runOpenPanel(WebPageProxy*, WebFrameProxy*, const SecurityOriginData&, API::OpenPanelParameters* parameters, WebOpenPanelResultListenerProxy* listener)
{
    if (!m_delegate)
        return false;

    RefPtr<WebOpenPanelResultListenerProxy> localListener = WTFMove(listener);
    m_delegate->runOpenPanelWithParameters(AWKOpenPanelParameters::create(parameters), AWKFrameInfo::create(),
        JNI::wrap<AWKFunction>(lambda([localListener] (JNI::PassLocalRef<JNI::AnyObject> urlVector) {
            notImplemented();
        }))
    );

    return true;
}

} // namespace WebKit
