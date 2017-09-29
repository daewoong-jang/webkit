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

#ifndef WebContextMenuProxyAndroid_h
#define WebContextMenuProxyAndroid_h

#if ENABLE(CONTEXT_MENUS)

#include "WebContextMenuProxy.h"

namespace org {
namespace webkit {
class AWKView;
}
}

namespace WebKit {

class WebPageProxy;

class WebContextMenuProxyAndroid final : public WebContextMenuProxy {
public:
    static RefPtr<WebContextMenuProxy> create(org::webkit::AWKView* webview, WebPageProxy& page, const ContextMenuContextData& contextMenuContextData, const UserData& userData)
    {
        return adoptRef(new WebContextMenuProxyAndroid(webview, page, contextMenuContextData, userData));
    }

    ~WebContextMenuProxyAndroid() = default;

    void show() override;

    void showContextMenuWithItems(const Vector<WebContextMenuItemData>& items) override;

private:
    WebContextMenuProxyAndroid(org::webkit::AWKView* webview, WebPageProxy& page, const ContextMenuContextData&, const UserData&);

    org::webkit::AWKView* m_view;
    WebPageProxy& m_page;
};

} // namespace WebKit

#endif // ENABLE(CONTEXT_MENUS)

#endif // WebContextMenuProxyAndroid_h
