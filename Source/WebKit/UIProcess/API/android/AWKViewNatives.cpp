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
#include "AWKViewNatives.h"

#include "AWKWebContentNatives.h"
#include "AcceleratedDrawingAreaProxy.h"
#include "CoordinatedCompositorHost.h"
#include "NativeWebEvent.h"
#include "StringUtilities.h"
#include "WebContextMenuProxyAndroid.h"
#include "WebEventEnumerationsNatives.h"
#include "WebPopupMenuProxyAndroid.h"
#include <WebCore/GLContextEGLAndroid.h>

using namespace WebCore;
using namespace WebKit;

namespace org {
namespace webkit {

static const float ALMOST_INTEGER_THRESHOLD = 0.1f;

static float snapAlmostIntegerDeviceScaleFactorToInteger(float deviceScaleFactor)
{
    float rounded = round(deviceScaleFactor);
    float diff = abs(rounded - deviceScaleFactor);
    if (diff < ALMOST_INTEGER_THRESHOLD)
        return rounded;
    return deviceScaleFactor;
}

AWKView::AWKView(AWKWebContent& webContent)
    : m_webContent(webContent)
{
}

AWKView::~AWKView()
{
}

void AWKView::scrollBegin(float x, float y)
{
    pageClient().scrollBegin(FloatPoint(x, y));
}

bool AWKView::scrollBy(float deltaX, float deltaY, bool isOverScrollAllowed)
{
    return pageClient().scrollBy(FloatSize(deltaX, deltaY), isOverScrollAllowed);
}

bool AWKView::scrollTo(float x, float y)
{
    notImplemented();
    return false;
}

void AWKView::scrollEnd(float x, float y)
{
    pageClient().scrollEnd();
}

void AWKView::pinchBegin(float x, float y)
{
    notImplemented();
}

void AWKView::adjustTransientZoom(float x, float y, float magnification)
{
    if (getUseFixedLayout()) {
        pageClient().setContentScaleFactor(magnification);
        pageClient().setContentPosition(FloatPoint(x, y));
        Base::setViewNeedsDisplay(0, 0, 0, 0);
        return;
    }

    if (AcceleratedDrawingAreaProxy* drawingArea = static_cast<AcceleratedDrawingAreaProxy*>(webPage().drawingArea())) {
        drawingArea->adjustTransientZoom(magnification, FloatPoint(x, y));
    }

    m_transientZoomOrigin = FloatPoint(x, y);
    m_transientZoomMagnification = magnification;
}

void AWKView::commitTransientZoom()
{
    if (getUseFixedLayout()) {
        pageClient().setContentScaleFactor(m_transientZoomMagnification);
        pageClient().setContentPosition(m_transientZoomOrigin);
        return;
    }

    if (AcceleratedDrawingAreaProxy* drawingArea = static_cast<AcceleratedDrawingAreaProxy*>(webPage().drawingArea())) {
        drawingArea->commitTransientZoom(m_transientZoomMagnification, m_transientZoomOrigin);
    }
}

bool AWKView::isActive()
{
    return pageClient().isActive();
}

void AWKView::setActive(bool active)
{
    pageClient().setActive(active);
}

bool AWKView::isFocused()
{
    return pageClient().isFocused();
}

void AWKView::setFocus(bool focus)
{
    pageClient().setFocused(focus);
}

bool AWKView::isVisible()
{
    return pageClient().isVisible();
}

void AWKView::setVisible(bool visible)
{
    pageClient().setVisible(visible);
    disablePainting(!visible);
}

void AWKView::setPageScaleFactor(float scale, int32_t x, int32_t y)
{
    webPage().scalePage(scale, IntPoint(x, y));
}

float AWKView::getPageScaleFactor()
{
    return webPage().pageScaleFactor();
}

void AWKView::setIntrinsicDeviceScaleFactor(float scale)
{
    webPage().setIntrinsicDeviceScaleFactor(snapAlmostIntegerDeviceScaleFactorToInteger(scale));
}

float AWKView::getIntrinsicDeviceScaleFactor()
{
    if (m_overrideDeviceScaleFactor != 0.0f)
        return m_overrideDeviceScaleFactor;

    return webPage().deviceScaleFactor();
}

void AWKView::setOverrideDeviceScaleFactor(float overrideDeviceScaleFactor)
{
    // Default value is 0. A value of 0 means the window's backing scale factor
    // will be used and automatically update when the window moves screens.
    m_overrideDeviceScaleFactor = overrideDeviceScaleFactor;
}

float AWKView::getOverrideDeviceScaleFactor()
{
    return m_overrideDeviceScaleFactor;
}

void AWKView::setUseFixedLayout(bool use)
{
    webPage().setUseFixedLayout(use);
}

bool AWKView::getUseFixedLayout()
{
    return webPage().useFixedLayout();
}

void AWKView::setBackgroundColor(int32_t color)
{
    pageClient().compositorHost().setBackgroundColor(color);
}

int32_t AWKView::getBackgroundColor()
{
    return pageClient().compositorHost().getBackgroundColor();
}

bool AWKView::requestExitFullScreen()
{
#if ENABLE(FULLSCREEN_API)
    return pageClient().requestExitFullScreen();
#else
    return false;
#endif
}

void AWKView::sendSingleTap(float x, float y)
{
    NativeWebMouseEvent fakeMouseDown = NativeWebEvent::createMouseEvent(currentTimeMS(),
        NativeWebEvent::toMouseAction(WebEventEnumerations::EVENT_MOUSE_PRESSED),
        NativeWebEvent::toButton(WebEventEnumerations::EVENT_MOUSE_LEFT_BUTTON),
        NativeWebEvent::toModifiers(0),
        x, y, x, y, 1, pageClient().transformFromScene());
    webPage().handleMouseEvent(fakeMouseDown);

    NativeWebMouseEvent fakeMouseMove = NativeWebEvent::createMouseEvent(currentTimeMS(),
        NativeWebEvent::toMouseAction(WebEventEnumerations::EVENT_MOUSE_MOVED),
        NativeWebEvent::toButton(0),
        NativeWebEvent::toModifiers(0),
        x, y, x, y, 1, pageClient().transformFromScene());
    webPage().handleMouseEvent(fakeMouseMove);

    NativeWebMouseEvent fakeMouseUp = NativeWebEvent::createMouseEvent(currentTimeMS(),
        NativeWebEvent::toMouseAction(WebEventEnumerations::EVENT_MOUSE_RELEASED),
        NativeWebEvent::toButton(WebEventEnumerations::EVENT_MOUSE_LEFT_BUTTON),
        NativeWebEvent::toModifiers(0),
        x, y, x, y, 1, pageClient().transformFromScene());
    webPage().handleMouseEvent(fakeMouseUp);
}

void AWKView::sendSingleTapUnconfirmed(float x, float y)
{
    notImplemented();
}

void AWKView::sendDoubleTap(float x, float y)
{
    notImplemented();
}

void AWKView::sendLongTap(float x, float y)
{
    notImplemented();
}

void AWKView::sendShowPressState(float x, float y)
{
    notImplemented();
}

void AWKView::sendShowPressCancel(float x, float y)
{
    notImplemented();
}

void AWKView::sendLongPress(float x, float y)
{
    NativeWebMouseEvent fakeMouseDown = NativeWebEvent::createMouseEvent(currentTimeMS(),
        NativeWebEvent::toMouseAction(WebEventEnumerations::EVENT_MOUSE_PRESSED),
        NativeWebEvent::toButton(WebEventEnumerations::EVENT_MOUSE_RIGHT_BUTTON),
        NativeWebEvent::toModifiers(0),
        x, y, x, y, 1, pageClient().transformFromScene());
    webPage().handleMouseEvent(fakeMouseDown);

    NativeWebMouseEvent fakeMouseMove = NativeWebEvent::createMouseEvent(currentTimeMS(),
        NativeWebEvent::toMouseAction(WebEventEnumerations::EVENT_MOUSE_MOVED),
        NativeWebEvent::toButton(0),
        NativeWebEvent::toModifiers(0),
        x, y, x, y, 1, pageClient().transformFromScene());
    webPage().handleMouseEvent(fakeMouseMove);

    NativeWebMouseEvent fakeMouseUp = NativeWebEvent::createMouseEvent(currentTimeMS(),
        NativeWebEvent::toMouseAction(WebEventEnumerations::EVENT_MOUSE_RELEASED),
        NativeWebEvent::toButton(WebEventEnumerations::EVENT_MOUSE_RIGHT_BUTTON),
        NativeWebEvent::toModifiers(0),
        x, y, x, y, 1, pageClient().transformFromScene());
    webPage().handleMouseEvent(fakeMouseUp);
}

void AWKView::sendTouchEvent(int64_t timestamp, int32_t action, int32_t modifiers, float x, float y, float globalX, float globalY, int32_t id)
{
#if ENABLE(TOUCH_EVENTS)
    NativeWebTouchEvent touchEvent = NativeWebEvent::createTouchEvent(timestamp,
        NativeWebEvent::toTouchAction(action),
        NativeWebEvent::toModifiers(modifiers),
        x, y, globalX, globalY, id,
        NativeWebEvent::toTouchPointState(action), pageClient().transformFromScene());
    webPage().handleTouchEvent(touchEvent);
#endif
}

void AWKView::sendMouseEvent(int64_t timestamp, int32_t action, int32_t modifiers, float x, float y, float globalX, float globalY, int32_t clickCount)
{
    NativeWebMouseEvent mouseEvent = NativeWebEvent::createMouseEvent(timestamp,
        NativeWebEvent::toMouseAction(action),
        NativeWebEvent::toButton(modifiers),
        NativeWebEvent::toModifiers(modifiers),
        x, y, globalX, globalY, clickCount, pageClient().transformFromScene());
    webPage().handleMouseEvent(mouseEvent);
}

void AWKView::sendWheelEvent(int64_t timestamp, int32_t action, int32_t modifiers, float deltaX, float deltaY, float x, float y, float globalX, float globalY)
{
    NativeWebWheelEvent wheelEvent = NativeWebEvent::createWheelEvent(timestamp, modifiers,
        deltaX, deltaY, x, y, globalX, globalY,
        pageClient().transformFromScene(), getIntrinsicDeviceScaleFactor());
    webPage().handleWheelEvent(wheelEvent);
}

void AWKView::sendKeyEvent(int64_t timestamp, int32_t action, int32_t modifiers, int32_t windowsVirtualKeyCode, int32_t nativeVirtualKeyCode, const std::string& text, const std::string& unmodifiedText, bool isAutoRepeat, bool isSystemKey, bool isKeypadKey, int64_t downTime)
{
    NativeWebKeyboardEvent keyEvent = NativeWebEvent::createKeyboardEvent(timestamp,
        NativeWebEvent::toKeyAction(action),
        NativeWebEvent::toModifiers(modifiers),
        windowsVirtualKeyCode, nativeVirtualKeyCode, String::fromUTF8(text.c_str()), String::fromUTF8(unmodifiedText.c_str()),
        isAutoRepeat, isSystemKey, isKeypadKey, downTime);
    if (m_waitingForKeyEventHandled) {
        m_pendingKeyEvents.append(keyEvent);
        return;
    }

    ASSERT(m_pendingKeyEvents.isEmpty());

    webPage().handleKeyboardEvent(keyEvent);
    m_waitingForKeyEventHandled = true;
}

RefPtr<WebPopupMenuProxy> AWKView::createPopupMenuProxy(WebPageProxy& page)
{
    return WebPopupMenuProxyAndroid::create(this, page);
}

#if ENABLE(CONTEXT_MENUS)
RefPtr<WebContextMenuProxy> AWKView::createContextMenuProxy(WebPageProxy& page, const ContextMenuContextData& contextMenuContextData, const UserData& userData)
{
    return WebContextMenuProxyAndroid::create(this, page, contextMenuContextData, userData);
}
#endif

void AWKView::doneWithKeyEvent(const NativeWebKeyboardEvent& event, bool wasEventHandled)
{
    m_waitingForKeyEventHandled = false;

    if (m_pendingKeyEvents.isEmpty())
        return;

    if (wasEventHandled) {
        m_pendingKeyEvents.removeFirstMatching([&](const NativeWebKeyboardEvent& pendingEvent) {
            if (event.downTime() == pendingEvent.downTime())
                return true;
            return false;
        });

        if (m_pendingKeyEvents.isEmpty())
            return;
    }

    webPage().handleKeyboardEvent(m_pendingKeyEvents.at(0));
    m_pendingKeyEvents.remove(0);

    m_waitingForKeyEventHandled = true;
}

void AWKView::setViewNeedsDisplay(const IntRect& dirtyRect)
{
    Base::setViewNeedsDisplay(dirtyRect.x(), dirtyRect.y(), dirtyRect.width(), dirtyRect.height());
}

void AWKView::editorStateChanged()
{
    const EditorState& editorState = webPage().editorState();

    if (m_hasInputMethod) {
        if (editorState.selectionIsNone) {
            Base::endInputMethod();
            m_hasInputMethod = false;
        }
    } else {
        if (editorState.isContentEditable) {
            Base::startInputMethod(editorState.isInPasswordField);
            m_hasInputMethod = true;
        }
    }

    if (m_hasInputMethod)
        Base::editorStateChanged(editorState.editableText.utf8().data(), editorState.selectionRangeStart, editorState.selectionRangeEnd,
            editorState.compositionRangeStart, editorState.compositionRangeEnd);
}

void AWKView::setCursor(const Cursor&)
{
    notImplemented();
}

void AWKView::webProcessDidExit(const String& url)
{
    pageClient().compositorHost().detach();
    Base::webProcessDidExit(stdStringFromWebCoreString(url));
}

void AWKView::didRelaunchWebProcess()
{
    // CoordinatedGraphics WebView has been deactivated automatically when
    // the web process is crashed.
    // So the WebView should be activated in order to be able to receive any
    // scene state from the newly created web process.
    setActive(true);
    Base::didRelaunchWebProcess();
}

void AWKView::startGL()
{
    dispatchOnMainThread([=] {
        pageClient().compositorHost().attach([=] {
            if (isVisible()) {
                disablePainting(false);
                setViewNeedsDisplay(IntRect());
            }
        });
    });
}

bool AWKView::paintToCurrentGLContext()
{
    if (m_paintingDisabled)
        return false;

    pageClient().paintToCurrentGLContext();
    return true;
}

void AWKView::contextLostGL()
{
    GLContextEGLAndroid::setContextLost(true);
    stopGL();
    GLContextEGLAndroid::setContextLost(false);
    startGL();
}

void AWKView::stopGL()
{
    ASSERT(pageClient().compositorHost().isDetached());
    pageClient().compositorHost().purgeGLResources();
}

void AWKView::onLayout(int32_t width, int32_t height)
{
    IntRect layoutRect(0, 0, width, height);
    pageClient().windowAndViewFramesChanged(layoutRect, FloatPoint());
    pageClient().setSize(layoutRect.size());
}

void AWKView::onPause()
{
    disablePainting(true);
    pageClient().compositorHost().detach();
}

void AWKView::onResume()
{
}

void AWKView::disablePainting(bool disable)
{
    if (disable == m_paintingDisabled)
        return;

    setActive(!disable);
    m_paintingDisabled = disable || !isActive();
}

void AWKView::dispatchOnMainThread(std::function<void()>&& function)
{
    if (isMainThread())
        function();
    else
        callOnMainThread(WTFMove(function));
}

inline void AWKView::setViewNeedsDisplay(int32_t x, int32_t y, int32_t width, int32_t height)
{
    Base::setViewNeedsDisplay(x, y, width, height);
}

inline void AWKView::editorStateChanged(const std::string& text, int32_t selectionStart, int32_t selectionEnd, int32_t compositionStart, int32_t compositionEnd)
{
    Base::editorStateChanged(text, selectionStart, selectionEnd, compositionStart, compositionEnd);
}

inline void AWKView::setCursor(int32_t cursorType)
{
    Base::setCursor(cursorType);
}

inline void AWKView::webProcessDidExit(const std::string& url)
{
    Base::webProcessDidExit(url);
}

inline WebPageProxy& AWKView::webPage() const
{
    return m_webContent.webPage();
}

inline PageClientImpl& AWKView::pageClient()
{
    return m_webContent.pageClient();
}

const PageClientImpl& AWKView::pageClient() const
{
    return m_webContent.pageClient();
}

namespace Natives {

template<> JNI::PassLocalRef<AWKView> AWKView::create(org::webkit::AWKWebContent* webContent)
{
    return AWKView::create(JNI::PassLocalRef<AWKWebContent>(webContent));
}

AWKView* AWKView::nativeCreate(JNI::PassLocalRef<AWKWebContent> webContent)
{
    return new org::webkit::AWKView(*webContent.as<org::webkit::AWKWebContent>());
}

}
} // namespace webkit
} // namespace org
