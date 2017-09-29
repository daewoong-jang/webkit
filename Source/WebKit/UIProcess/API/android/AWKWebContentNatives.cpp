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
#include "AWKWebContentNatives.h"

#include "AWKCallbackNatives.h"
#include "AWKNativesCast.h"
#include "AWKUIDelegateNatives.h"
#include "AWKViewNatives.h"
#include "AWKWebContentConfigurationNatives.h"
#include "NavigationClient.h"
#include "StringUtilities.h"
#include "UIClient.h"
#include "WebBackForwardList.h"
#include <WebCore/Timer.h>
#include <android/graphics/Natives/Rect.h>

using namespace WebCore;
using namespace WebKit;

namespace org {
namespace webkit {

static String urlToString(JNI::PassLocalRef<Natives::AWKURL> url)
{
    return url ? String::fromUTF8(url->getAbsoluteString().c_str()) : String();
}

AWKWebContent::AWKWebContent(JNI::PassLocalRef<Natives::Rect> frame, JNI::PassLocalRef<Natives::AWKWebContentConfiguration> configuration)
    : m_configuration(configuration)
    , m_pageClient(*this)
    , m_page(m_pageClient.page())
    , m_viewController(*this)
{
    if (frame && frame->width() > 0 && frame->height() > 0)
        m_pageClient.setSize(IntSize(frame->width(), frame->height()));
}

AWKWebContent::~AWKWebContent()
{
}

JNI::PassLocalRef<Natives::AWKWebContentConfiguration> AWKWebContent::getConfiguration()
{
    return m_configuration;
}

std::string AWKWebContent::getTitle()
{
    return stdStringFromWebCoreString(webPage().pageLoadState().title());
}

JNI::PassLocalRef<Natives::AWKURL> AWKWebContent::getURL()
{
    return AWKURL::create(stdStringFromWebCoreString(webPage().pageLoadState().activeURL()));
}

std::string AWKWebContent::getCustomUserAgent()
{
    return stdStringFromWebCoreString(webPage().customUserAgent());
}

void AWKWebContent::setCustomUserAgent(const std::string& customUserAgent)
{
    webPage().setCustomUserAgent(String::fromUTF8(customUserAgent.c_str()));
}

void AWKWebContent::setNavigationDelegate(JNI::PassLocalRef<Natives::AWKNavigationDelegate> navigationDelegate)
{
    m_navigationDelegate = navigationDelegate;
    webPage().setNavigationClient(std::make_unique<NavigationClient>(*this, m_navigationDelegate));
}

JNI::PassLocalRef<Natives::AWKNavigationDelegate> AWKWebContent::getNavigationDelegate()
{
    return m_navigationDelegate;
}

void AWKWebContent::setUIDelegate(JNI::PassLocalRef<Natives::AWKUIDelegate> uiDelegate)
{
    m_uiDelegate = uiDelegate;
    webPage().setUIClient(std::make_unique<UIClient>(*this, uiDelegate));
}

JNI::PassLocalRef<Natives::AWKUIDelegate> AWKWebContent::getUIDelegate()
{
    return m_uiDelegate;
}

double AWKWebContent::getEstimatedProgress()
{
    return webPage().estimatedProgress();
}

bool AWKWebContent::hasOnlySecureContent()
{
    return webPage().pageLoadState().hasOnlySecureContent();
}

JNI::PassLocalRef<Natives::AWKNavigation> AWKWebContent::loadRequest(JNI::PassLocalRef<Natives::AWKURLRequest> request)
{
    if (!request)
        return nullptr;

    RefPtr<API::Navigation> navigation = webPage().loadRequest(ResourceRequest(core(request).resourceRequest()));
    if (!navigation)
        return nullptr;

    return AWKNavigation::create(navigation.get());
}

JNI::PassLocalRef<Natives::AWKNavigation> AWKWebContent::loadHTMLString(const std::string& htmlString, JNI::PassLocalRef<Natives::AWKURL> baseURL)
{
    RefPtr<API::Navigation> navigation = webPage().loadHTMLString(String::fromUTF8(htmlString.c_str()), urlToString(baseURL));
    if (!navigation)
        return nullptr;

    return AWKNavigation::create(navigation.get());
}

bool AWKWebContent::isLoading()
{
    return webPage().pageLoadState().isLoading();
}

JNI::PassLocalRef<Natives::AWKNavigation> AWKWebContent::reload()
{
    RefPtr<API::Navigation> navigation = webPage().reload({ });
    if (!navigation)
        return nullptr;

    return AWKNavigation::create(navigation.get());
}

JNI::PassLocalRef<Natives::AWKNavigation> AWKWebContent::reloadFromOrigin()
{
    RefPtr<API::Navigation> navigation = webPage().reload({ ReloadOption::FromOrigin });
    if (!navigation)
        return nullptr;

    return AWKNavigation::create(navigation.get());
}

void AWKWebContent::stopLoading()
{
    webPage().stopLoading();
}

JNI::PassLocalRef<Natives::AWKNavigation> AWKWebContent::loadData(JNI::PassLocalRef<Natives::AWKData> data, const std::string& mimeType, const std::string& characterEncodingName, JNI::PassLocalRef<Natives::AWKURL> baseURL)
{
    if (!data)
        return nullptr;

    API::Data& localData = core(data);
    RefPtr<API::Navigation> navigation = webPage().loadData(&localData, String::fromUTF8(mimeType.c_str()), String::fromUTF8(characterEncodingName.c_str()), urlToString(baseURL));
    if (!navigation)
        return nullptr;

    return AWKNavigation::create(navigation.get());
}

JNI::PassLocalRef<Natives::AWKNavigation> AWKWebContent::loadFileURL(JNI::PassLocalRef<Natives::AWKURL> url, JNI::PassLocalRef<Natives::AWKURL> readAccessUrl)
{
    if (!url)
        return nullptr;

    if (!url->isFileURL() || !readAccessUrl->isFileURL())
        return nullptr;

    RefPtr<API::Navigation> navigation = webPage().loadFile(urlToString(url), urlToString(readAccessUrl));
    if (!navigation)
        return nullptr;

    return AWKNavigation::create(navigation.get());
}

JNI::PassLocalRef<Natives::AWKBackForwardList> AWKWebContent::getBackForwardList()
{
    return m_backForwardList;
}

bool AWKWebContent::canGoBack()
{
    return webPage().backForwardList().backItem();
}

bool AWKWebContent::canGoForward()
{
    return webPage().backForwardList().forwardItem();
}

JNI::PassLocalRef<Natives::AWKNavigation> AWKWebContent::goBack()
{
    RefPtr<API::Navigation> navigation = webPage().goBack();
    return AWKNavigation::create(navigation.get());
}

JNI::PassLocalRef<Natives::AWKNavigation> AWKWebContent::goForward()
{
    RefPtr<API::Navigation> navigation = webPage().goForward();
    return AWKNavigation::create(navigation.get());
}

JNI::PassLocalRef<Natives::AWKNavigation> AWKWebContent::goToBackForwardListItem(JNI::PassLocalRef<Natives::AWKBackForwardListItem> item)
{
    if (!item)
        return nullptr;

    RefPtr<API::Navigation> navigation = webPage().goToBackForwardItem(&core(item));
    return AWKNavigation::create(navigation.get());
}

void AWKWebContent::evaluateJavaScript(const std::string& javaScriptString, JNI::PassLocalRef<Natives::AWKCallback> completionHandler)
{
    if (javaScriptString.empty())
        return;

    notImplemented();
}

JNI::PassLocalRef<Natives::AWKView> AWKWebContent::getView()
{
    return m_viewController.getOrCreateView(this);
}

JNI::PassLocalRef<Natives::AWKView> AWKWebContent::peekView()
{
    return m_viewController.view();
}

namespace Natives {

AWKWebContent* AWKWebContent::nativeCreate(JNI::PassLocalRef<Rect> frame, JNI::PassLocalRef<AWKWebContentConfiguration> configuration)
{
    return new org::webkit::AWKWebContent(frame, configuration);
}

} // namespace Natives
} // namespace webkit
} // namespace org
