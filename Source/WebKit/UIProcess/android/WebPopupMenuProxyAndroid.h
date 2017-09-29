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

#ifndef WebPopupMenuProxyAndroid_h
#define WebPopupMenuProxyAndroid_h

#include "WebPopupMenuProxy.h"
#include <wtf/Ref.h>

namespace org {
namespace webkit {
class AWKView;
}
}

namespace WebKit {

class WebMenu;

class WebPopupMenuProxyAndroid : public WebPopupMenuProxy {
public:
    static Ref<WebPopupMenuProxyAndroid> create(org::webkit::AWKView* view, WebPopupMenuProxy::Client& client)
    {
        return adoptRef(*new WebPopupMenuProxyAndroid(view, client));
    }
    ~WebPopupMenuProxyAndroid();

    virtual void showPopupMenu(const WebCore::IntRect& rect, WebCore::TextDirection, double pageScaleFactor, const Vector<WebPopupItem>& items, const PlatformPopupMenuData&, int32_t selectedIndex) override;
    virtual void hidePopupMenu() override;

private:
    WebPopupMenuProxyAndroid(org::webkit::AWKView*, WebPopupMenuProxy::Client&);

    org::webkit::AWKView* m_view;
};

}

#endif /* WebPopupMenuProxyAndroid_h */
