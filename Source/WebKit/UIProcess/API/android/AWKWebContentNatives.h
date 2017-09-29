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

#ifndef AWKWebContent_h
#define AWKWebContent_h

#include <org/webkit/Natives/AWKWebContent.h>

#include "PageClientImpl.h"
#include "ViewController.h"
#include <androidjni/GlobalRef.h>

namespace WebKit {
class DownloadProxy;
class WebPageProxy;
}

namespace org {
namespace webkit {

class AWKWebContent final : public Natives::AWKWebContent {
    friend class Natives::AWKWebContent;
    typedef Natives::AWKWebContent Base;
public:
    ~AWKWebContent();

    JNI::PassLocalRef<Natives::AWKWebContentConfiguration> getConfiguration() override;

    std::string getTitle() override;
    JNI::PassLocalRef<Natives::AWKURL> getURL() override;
    std::string getCustomUserAgent() override;
    void setCustomUserAgent(const std::string& customUserAgent) override;

    void setNavigationDelegate(JNI::PassLocalRef<Natives::AWKNavigationDelegate> navigationDelegate) override;
    JNI::PassLocalRef<Natives::AWKNavigationDelegate> getNavigationDelegate() override;

    void setUIDelegate(JNI::PassLocalRef<Natives::AWKUIDelegate> uiDelegate) override;
    JNI::PassLocalRef<Natives::AWKUIDelegate> getUIDelegate() override;

    double getEstimatedProgress() override;
    bool hasOnlySecureContent() override;

    JNI::PassLocalRef<Natives::AWKNavigation> loadRequest(JNI::PassLocalRef<Natives::AWKURLRequest> request) override;
    JNI::PassLocalRef<Natives::AWKNavigation> loadHTMLString(const std::string& htmlString, JNI::PassLocalRef<Natives::AWKURL> baseURL) override;

    bool isLoading() override;
    JNI::PassLocalRef<Natives::AWKNavigation> reload() override;
    JNI::PassLocalRef<Natives::AWKNavigation> reloadFromOrigin() override;
    void stopLoading() override;

    JNI::PassLocalRef<Natives::AWKNavigation> loadData(JNI::PassLocalRef<Natives::AWKData> data, const std::string& mimeType, const std::string& characterEncodingName, JNI::PassLocalRef<Natives::AWKURL> baseURL) override;
    JNI::PassLocalRef<Natives::AWKNavigation> loadFileURL(JNI::PassLocalRef<Natives::AWKURL> url, JNI::PassLocalRef<Natives::AWKURL> readAccessUrl) override;

    JNI::PassLocalRef<Natives::AWKBackForwardList> getBackForwardList() override;
    bool canGoBack() override;
    bool canGoForward() override;
    JNI::PassLocalRef<Natives::AWKNavigation> goBack() override;
    JNI::PassLocalRef<Natives::AWKNavigation> goForward() override;
    JNI::PassLocalRef<Natives::AWKNavigation> goToBackForwardListItem(JNI::PassLocalRef<Natives::AWKBackForwardListItem> item) override;

    void evaluateJavaScript(const std::string& javaScriptString, JNI::PassLocalRef<Natives::AWKCallback> completionHandler) override;

    JNI::PassLocalRef<Natives::AWKView> getView() override;
    JNI::PassLocalRef<Natives::AWKView> peekView() override;

    WebKit::WebPageProxy& webPage() const { return *m_page; }
    WebKit::PageClientImpl& pageClient() { return m_pageClient; }
    WebKit::ViewController& view() { return m_viewController; }

private:
    AWKWebContent(JNI::PassLocalRef<Natives::Rect>, JNI::PassLocalRef<Natives::AWKWebContentConfiguration>);

    JNI::GlobalRef<Natives::AWKWebContentConfiguration> m_configuration;
    JNI::GlobalRef<Natives::AWKNavigationDelegate> m_navigationDelegate;
    JNI::GlobalRef<Natives::AWKUIDelegate> m_uiDelegate;
    JNI::GlobalRef<Natives::AWKBackForwardList> m_backForwardList;
    WebKit::PageClientImpl m_pageClient;
    WebKit::WebPageProxy* m_page;

    WebKit::ViewController m_viewController;
};

} // namespace webkit
} // namespace org

#endif // AWKWebContent_h
