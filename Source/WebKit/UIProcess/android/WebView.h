/*
 * Copyright (C) 2016 NAVER Corp. All rights reserved.
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

#pragma once

#include "WebRenderContextList.h"
#include "WebViewPrivate.h"
#include <android/view/FrameLayout.h>

namespace org {
namespace webkit {

class WebView : public FrameLayout {
public:
    ANDROID_EXTERN WebView(Context&);
    ANDROID_EXTERN virtual ~WebView();

    ANDROID_EXTERN virtual bool canGoBack();
    ANDROID_EXTERN virtual bool canGoForward();
    ANDROID_EXTERN virtual void goBack();
    ANDROID_EXTERN virtual void goForward();

    ANDROID_EXTERN virtual void loadUrl(StringRef url);

    ANDROID_EXTERN virtual void onPause();
    ANDROID_EXTERN virtual void onResume();

    // View
    ANDROID_EXTERN virtual void invalidate() override;
    ANDROID_EXTERN virtual void invalidate(Rect&) override;

private:
    std::shared_ptr<WebViewPrivate> mPrivate;
    std::shared_ptr<GLSurfaceView> mSurfaceView;
};

} // namespace webkit
} // namespace org

using WebView = org::webkit::WebView;
