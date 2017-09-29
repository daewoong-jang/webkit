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

package org.webkit;

import android.content.Context;
import android.graphics.Rect;
import android.opengl.GLSurfaceView;
import android.widget.FrameLayout;

public class WebView extends FrameLayout {
    public WebView(Context context) {
        super(context);
        mPrivate = new WebViewPrivate(context, this);
        mSurfaceView = mPrivate.getSurfaceView();

        addView(mSurfaceView);
        addView(mPrivate);
        mPrivate.createView();
    }

    private WebViewPrivate mPrivate;
    private GLSurfaceView mSurfaceView;

    public boolean canGoBack() {
        return mPrivate.getForegroundView().getWebContent().canGoBack();
    }

    public boolean canGoForward() {
        return mPrivate.getForegroundView().getWebContent().canGoForward();
    }

    public void goBack() {
        mPrivate.getForegroundView().getWebContent().goBack();
    }

    public void goForward() {
        mPrivate.getForegroundView().getWebContent().goForward();
    }

    public void loadUrl(String url) {
        AWKURL urlObject = new AWKURL(url);
        AWKURLRequest request = new AWKURLRequest(urlObject);
        mPrivate.getForegroundView().getWebContent().loadRequest(request);
    }

    public void onPause() {
        mPrivate.onPause();
    }

    public void onResume() {
        mPrivate.onResume();
    }

    @Override
    public void invalidate() {
        if (!mPrivate.isVisible())
            return;

        mSurfaceView.requestRender();
    }

    @Override
    public void invalidate(Rect rect) {
        invalidate();
    }
}
