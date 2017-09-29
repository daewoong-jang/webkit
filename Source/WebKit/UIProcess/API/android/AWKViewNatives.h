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

#ifndef AWKView_h
#define AWKView_h

#include <org/webkit/Natives/AWKView.h>

#include "PageClientImpl.h"

namespace org {
namespace webkit {

class AWKView final : public Natives::AWKView {
    friend class Natives::AWKView;
    typedef Natives::AWKView Base;
public:
    ~AWKView();

    void scrollBegin(float, float) override;
    bool scrollBy(float, float, bool) override;
    bool scrollTo(float, float) override;
    void scrollEnd(float, float) override;

    void pinchBegin(float, float) override;
    void adjustTransientZoom(float, float, float) override;
    void commitTransientZoom() override;

    bool isActive() override;
    void setActive(bool) override;

    bool isFocused() override;
    void setFocus(bool) override;

    bool isVisible() override;
    void setVisible(bool) override;

    void setPageScaleFactor(float, int32_t, int32_t) override;
    float getPageScaleFactor() override;

    void setIntrinsicDeviceScaleFactor(float) override;
    float getIntrinsicDeviceScaleFactor() override;

    void setOverrideDeviceScaleFactor(float overrideDeviceScaleFactor) override;
    float getOverrideDeviceScaleFactor() override;

    void setUseFixedLayout(bool) override;
    bool getUseFixedLayout() override;

    void setBackgroundColor(int32_t) override;
    int32_t getBackgroundColor() override;

    bool requestExitFullScreen() override;

    void sendSingleTap(float, float) override;
    void sendSingleTapUnconfirmed(float, float) override;
    void sendDoubleTap(float, float) override;
    void sendLongTap(float, float) override;

    void sendShowPressState(float, float) override;
    void sendShowPressCancel(float, float) override;
    void sendLongPress(float, float) override;

    void sendTouchEvent(int64_t, int32_t, int32_t, float, float, float, float, int32_t) override;
    void sendMouseEvent(int64_t, int32_t, int32_t, float, float, float, float, int32_t) override;
    void sendWheelEvent(int64_t, int32_t, int32_t, float, float, float, float, float, float) override;
    void sendKeyEvent(int64_t, int32_t, int32_t, int32_t, int32_t, const std::string&, const std::string&, bool, bool, bool, int64_t) override;

    RefPtr<WebKit::WebPopupMenuProxy> createPopupMenuProxy(WebKit::WebPageProxy&);
#if ENABLE(CONTEXT_MENUS)
    RefPtr<WebKit::WebContextMenuProxy> createContextMenuProxy(WebKit::WebPageProxy&, const WebKit::ContextMenuContextData&, const WebKit::UserData&);
#endif

    void doneWithKeyEvent(const WebKit::NativeWebKeyboardEvent&, bool);

    void setViewNeedsDisplay(const WebCore::IntRect&);

    void editorStateChanged();

    void setCursor(const WebCore::Cursor&);

    void webProcessDidExit(const String&);
    void didRelaunchWebProcess() override;

    AWKWebContent& webContent() const { return m_webContent; }

    void startGL();
    bool paintToCurrentGLContext();
    void contextLostGL();
    void stopGL();

private:
    void onLayout(int32_t, int32_t) override;

    void onPause() override;
    void onResume() override;

    void disablePainting(bool);

    void dispatchOnMainThread(std::function<void ()>&&);

    void setViewNeedsDisplay(int32_t x, int32_t y, int32_t width, int32_t height) override;
    void editorStateChanged(const std::string& text, int32_t selectionStart, int32_t selectionEnd, int32_t compositionStart, int32_t compositionEnd) override;
    void setCursor(int32_t cursorType) override;
    void webProcessDidExit(const std::string& url) override;

private:
    AWKView(AWKWebContent&);

    WebKit::WebPageProxy& webPage() const;
    WebKit::PageClientImpl& pageClient();
    const WebKit::PageClientImpl& pageClient() const;

    AWKWebContent& m_webContent;

    WebCore::Image* m_lastCursorImage { nullptr };
    bool m_hasInputMethod { false };
    bool m_waitingForKeyEventHandled { false };
    Vector<WebKit::NativeWebKeyboardEvent> m_pendingKeyEvents;

    WebCore::FloatPoint m_transientZoomOrigin;
    float m_transientZoomMagnification { 0.0f };

    std::atomic<bool> m_paintingDisabled { false };
    float m_overrideDeviceScaleFactor { 0.0f };
};

} // namespace webkit
} // namespace org

#endif // AWKView_h
