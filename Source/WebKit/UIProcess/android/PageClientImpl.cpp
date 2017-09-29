/*
 * Copyright (C) 2013-2015 NAVER Corp. All rights reserved.
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
#include "PageClientImpl.h"

#include "APIPageConfiguration.h"
#include "AWKNativesCast.h"
#include "AWKWebContentConfigurationNatives.h"
#include "AWKWebContentNatives.h"
#include "ActivityState.h"
#include "CoordinatedCompositorHost.h"
#include "CoordinatedGraphicsScene.h"
#include "DrawingAreaProxyImpl.h"
#include "WebBackForwardList.h"
#include "WebBackForwardListItem.h"
#include "WebContextMenuProxy.h"
#include "WebPageProxy.h"
#include "WebProcessPool.h"
#include <WebCore/NotImplemented.h>

#if ENABLE(CONTEXT_MENUS)
#include "WebContextMenuProxyAndroid.h"
#endif

using namespace WebCore;
using namespace org::webkit;

namespace WebKit {

static Ref<API::PageConfiguration> createPageConfiguration(JNI::PassLocalRef<AWKWebContentConfiguration> configuration)
{
    Ref<API::PageConfiguration> pageConfiguration = API::PageConfiguration::create();
    pageConfiguration->setPreferences(&core(configuration->getPreferences()));
    pageConfiguration->setUserContentController(&core(configuration->getUserContentController()));
    pageConfiguration->setWebsiteDataStore(&core(configuration->getWebsiteDataStore()));
    return pageConfiguration;
}

PageClientImpl::PageClientImpl(AWKWebContent& webContent)
    : m_webContent(webContent)
    , m_focused(false)
    , m_visible(false)
#if ENABLE(FULLSCREEN_API)
    , m_hasRequestedFullScreen(false)
#endif
    , m_opacity(1.0)
{
    JNI::LocalRef<AWKWebContentConfiguration> configuration = webContent.getConfiguration();
    WebProcessPool& processPool = core(configuration->getProcessPool());
    m_page = processPool.createWebPage(*this, createPageConfiguration(configuration));
    m_page->initializeWebPage();
    m_compositorHost = std::make_unique<CoordinatedCompositorHost>(*m_page);
}

PageClientImpl::~PageClientImpl()
{
    if (m_page->isClosed())
        return;

    m_page->close();
}

void PageClientImpl::setSize(const IntSize& size)
{
    if (m_size == size)
        return;

    m_size = size;

    updateViewportSize();
}

bool PageClientImpl::isActive()
{
    if (!m_page)
        return false;

    return m_compositorHost->isActive();
}

void PageClientImpl::setActive(bool active)
{
    m_compositorHost->setActive(active);
    m_page->activityStateDidChange(ActivityState::WindowIsActive);
}

void PageClientImpl::setFocused(bool focused)
{
    if (m_focused == focused)
        return;

    m_focused = focused;
    m_page->activityStateDidChange(ActivityState::IsFocused | ActivityState::WindowIsActive);
}

void PageClientImpl::setVisible(bool visible)
{
    if (m_visible == visible)
        return;

    m_visible = visible;
    m_page->activityStateDidChange(ActivityState::IsVisible | ActivityState::IsInWindow);

    if (AcceleratedDrawingAreaProxy* drawingArea = static_cast<AcceleratedDrawingAreaProxy*>(page()->drawingArea()))
        drawingArea->visibilityDidChange();
}

void PageClientImpl::setContentScaleFactor(float scaleFactor)
{
    m_page->scalePage(scaleFactor, roundedIntPoint(contentPosition()));
    updateViewportSize();
}

void PageClientImpl::paintToCurrentGLContext()
{
    m_compositorHost->paintToCurrentGLContext();
}

void PageClientImpl::setDrawsBackground(bool drawsBackground)
{
    m_page->setDrawsBackground(drawsBackground);
}

bool PageClientImpl::drawsBackground() const
{
    return m_page->drawsBackground();
}

void PageClientImpl::suspendActiveDOMObjectsAndAnimations()
{
    m_page->suspendActiveDOMObjectsAndAnimations();
}

void PageClientImpl::resumeActiveDOMObjectsAndAnimations()
{
    m_page->resumeActiveDOMObjectsAndAnimations();
}

#if ENABLE(FULLSCREEN_API)
bool PageClientImpl::requestExitFullScreen()
{
    if (!isFullScreen())
        return false;

    m_page->fullScreenManager()->requestExitFullScreen();
    return true;
}
#endif

FloatSize PageClientImpl::visibleContentsSize() const
{
    FloatSize visibleContentsSize(dipSize());
    if (m_page->useFixedLayout())
        visibleContentsSize.scale(1 / contentScaleFactor());

    return visibleContentsSize;
}

AffineTransform PageClientImpl::transformFromScene() const
{
    auto transform = transformToScene().inverse();
    return transform ? *transform : AffineTransform();
}

AffineTransform PageClientImpl::transformToScene() const
{
    FloatPoint position = -m_contentPosition;
    float effectiveScale = m_page->deviceScaleFactor();
    if (m_page->useFixedLayout())
        effectiveScale *= contentScaleFactor();
    position.scale(effectiveScale, effectiveScale);

    TransformationMatrix transform;
    transform.translate(position.x(), position.y());
    transform.scale(effectiveScale);

    return transform.toAffineTransform();
}

void PageClientImpl::scrollBegin(const FloatPoint& devicePoint)
{
}

bool PageClientImpl::scrollBy(const FloatSize& deviceDelta, bool isOverScrollAllowed)
{
    float effectiveScale = contentScaleFactor() * m_page->deviceScaleFactor();
    FloatSize finalDelta = deviceDelta;
    finalDelta.scale(1 / effectiveScale);

    FloatPoint scrollPosition;
    scrollPosition.setX(clampTo(m_contentPosition.x() + finalDelta.width(), 0.f, m_contentsSize.width() - floorf(m_size.width() / effectiveScale)));
    scrollPosition.setY(clampTo(m_contentPosition.y() + finalDelta.height(), 0.f, m_contentsSize.height() - floorf(m_size.height() / effectiveScale)));

    if (scrollPosition == m_contentPosition)
        return false;

    setContentPosition(scrollPosition);
    m_compositorHost->setVisibleContentsRect(FloatRect(scrollPosition, m_size), FloatPoint());
    return true;
}

void PageClientImpl::scrollEnd()
{
}

void PageClientImpl::windowAndViewFramesChanged(const FloatRect& viewFrameInWindowCoordinates, const FloatPoint& accessibilityViewCoordinates)
{
    notImplemented();
}

inline ViewController& PageClientImpl::view()
{
    return m_webContent.view();
}

void PageClientImpl::updateViewportSize()
{
    if (AcceleratedDrawingAreaProxy* drawingArea = static_cast<AcceleratedDrawingAreaProxy*>(page()->drawingArea())) {
        drawingArea->setSize(roundedIntSize(dipSize()), IntSize(), IntSize());
        FloatRect visibleContentsRect(contentPosition(), visibleContentsSize());
        visibleContentsRect.intersect(FloatRect(FloatPoint(), contentsSize()));
        m_compositorHost->setVisibleContentsRect(visibleContentsRect, FloatPoint());
    }
}

inline FloatSize PageClientImpl::dipSize() const
{
    FloatSize dipSize(size());
    dipSize.scale(1 / m_page->deviceScaleFactor());

    return dipSize;
}

std::unique_ptr<DrawingAreaProxy> PageClientImpl::createDrawingAreaProxy()
{
    return std::make_unique<DrawingAreaProxyImpl>(*m_page);
}

void PageClientImpl::setViewNeedsDisplay(const Region& area)
{
    m_webContent.view().setViewNeedsDisplay(area.bounds());
}

void PageClientImpl::requestScroll(const FloatPoint&, const IntPoint&, bool)
{
    notImplemented();
}

FloatPoint PageClientImpl::viewScrollPosition()
{
    return { };
}

IntSize PageClientImpl::viewSize()
{
    return roundedIntSize(dipSize());
}

bool PageClientImpl::isViewWindowActive()
{
    return isActive();
}

bool PageClientImpl::isViewFocused()
{
    return isFocused();
}

bool PageClientImpl::isViewVisible()
{
    return isVisible();
}

bool PageClientImpl::isViewInWindow()
{
    return isVisible();
}

void PageClientImpl::processDidExit()
{
    m_webContent.view().webProcessDidExit(m_page->backForwardList().currentItem() ? m_page->backForwardList().currentItem()->url() : "");
}

void PageClientImpl::didRelaunchProcess()
{
    m_webContent.view().didRelaunchWebProcess();
}

void PageClientImpl::pageClosed()
{
    notImplemented();
}

void PageClientImpl::preferencesDidChange()
{
    notImplemented();
}

void PageClientImpl::toolTipChanged(const String&, const String& newToolTip)
{
    notImplemented();
}

void PageClientImpl::didCommitLoadForMainFrame(const String&, bool)
{
    setContentPosition(FloatPoint());
    m_contentsSize = IntSize();
    view().didCommitLoad();
}

void PageClientImpl::handleDownloadRequest(DownloadProxy* download)
{
    notImplemented();
}

void PageClientImpl::didChangeContentSize(const IntSize& size)
{
    if (m_contentsSize == size)
        return;

    m_contentsSize = size;
    m_webContent.view().didChangeContentsSize(size);

    updateViewportSize();
}

void PageClientImpl::setCursor(const Cursor& cursor)
{
    view().setCursor(cursor);
}

void PageClientImpl::setCursorHiddenUntilMouseMoves(bool)
{
    notImplemented();
}

void PageClientImpl::didChangeViewportProperties(const ViewportAttributes& attr)
{
    m_webContent.view().didChangeViewportAttributes(attr);
}

void PageClientImpl::registerEditCommand(Ref<WebEditCommandProxy>&& command, WebPageProxy::UndoOrRedo undoOrRedo)
{
    m_undoController.registerEditCommand(WTFMove(command), undoOrRedo);
}

void PageClientImpl::clearAllEditCommands()
{
    m_undoController.clearAllEditCommands();
}

bool PageClientImpl::canUndoRedo(WebPageProxy::UndoOrRedo undoOrRedo)
{
    return m_undoController.canUndoRedo(undoOrRedo);
}

void PageClientImpl::executeUndoRedo(WebPageProxy::UndoOrRedo undoOrRedo)
{
    m_undoController.executeUndoRedo(undoOrRedo);
}

void PageClientImpl::wheelEventWasNotHandledByWebCore(const NativeWebWheelEvent&)
{
    notImplemented();
}

void PageClientImpl::updateTextInputState()
{
    view().editorStateChanged();
}

FloatRect PageClientImpl::convertToDeviceSpace(const FloatRect& userRect)
{
    if (m_page->useFixedLayout()) {
        FloatRect result = userRect;
        result.scale(m_page->deviceScaleFactor());
        return result;
    }

    notImplemented();
    return userRect;
}

FloatRect PageClientImpl::convertToUserSpace(const FloatRect& deviceRect)
{
    if (m_page->useFixedLayout()) {
        FloatRect result = deviceRect;
        result.scale(1 / m_page->deviceScaleFactor());
        return result;
    }

    notImplemented();
    return deviceRect;
}

IntPoint PageClientImpl::screenToRootView(const IntPoint& point)
{
    notImplemented();
    return point;
}

IntRect PageClientImpl::rootViewToScreen(const IntRect&)
{
    notImplemented();
    return IntRect();
}

void PageClientImpl::doneWithKeyEvent(const NativeWebKeyboardEvent& event, bool wasEventHandled)
{
    view().doneWithKeyEvent(event, wasEventHandled);
}

#if ENABLE(TOUCH_EVENTS)
void PageClientImpl::doneWithTouchEvent(const NativeWebTouchEvent& event, bool wasEventHandled)
{
    m_webContent.view().doneWithTouchEvent(wasEventHandled);
}
#endif

RefPtr<WebPopupMenuProxy> PageClientImpl::createPopupMenuProxy(WebPageProxy& page)
{
    return view().createPopupMenuProxy(page);
}

#if ENABLE(CONTEXT_MENUS)
RefPtr<WebContextMenuProxy> PageClientImpl::createContextMenuProxy(WebPageProxy& page, const ContextMenuContextData& contextMenuContextData, const UserData& userData)
{
    return view().createContextMenuProxy(page, contextMenuContextData, userData);
}
#endif

#if ENABLE(INPUT_TYPE_COLOR)
RefPtr<WebColorPicker> PageClientImpl::createColorPicker(WebPageProxy*, const Color&, const IntRect&)
{
    notImplemented();
    return nullptr;
}
#endif

void PageClientImpl::enterAcceleratedCompositingMode(const LayerTreeContext&)
{
    setActive(true);
}

void PageClientImpl::exitAcceleratedCompositingMode()
{
    setActive(false);
}

void PageClientImpl::updateAcceleratedCompositingMode(const LayerTreeContext&)
{
    notImplemented();
}

#if ENABLE(FULLSCREEN_API)
WebFullScreenManagerProxyClient& PageClientImpl::fullScreenManagerProxyClient()
{
    return *this;
}

void PageClientImpl::closeFullScreenManager()
{
}

bool PageClientImpl::isFullScreen()
{
    return m_hasRequestedFullScreen;
}

void PageClientImpl::enterFullScreen()
{
    if (m_hasRequestedFullScreen)
        return;

    WebFullScreenManagerProxy* manager = m_page->fullScreenManager();
    if (!manager)
        return;

    m_hasRequestedFullScreen = true;

    manager->willEnterFullScreen();
    view().enterFullScreen();
    manager->didEnterFullScreen();
}

void PageClientImpl::exitFullScreen()
{
    if (!m_hasRequestedFullScreen)
        return;

    WebFullScreenManagerProxy* manager = m_page->fullScreenManager();
    if (!manager)
        return;

    m_hasRequestedFullScreen = false;

    manager->willExitFullScreen();
    view().exitFullScreen();
    manager->didExitFullScreen();
}

void PageClientImpl::beganEnterFullScreen(const IntRect&, const IntRect&)
{
    notImplemented();
}

void PageClientImpl::beganExitFullScreen(const IntRect&, const IntRect&)
{
    notImplemented();
}
#endif

void PageClientImpl::didFinishLoadingDataForCustomContentProvider(const String& suggestedFilename, const IPC::DataReference&)
{
    notImplemented();
}

void PageClientImpl::didFirstVisuallyNonEmptyLayoutForMainFrame()
{
    notImplemented();
}

void PageClientImpl::didFinishLoadForMainFrame()
{
    notImplemented();
}

void PageClientImpl::didSameDocumentNavigationForMainFrame(SameDocumentNavigationType)
{
    notImplemented();
}

void PageClientImpl::didRestoreScrollPosition()
{
    notImplemented();
}

UserInterfaceLayoutDirection PageClientImpl::userInterfaceLayoutDirection()
{
    notImplemented();
    return UserInterfaceLayoutDirection::LTR;
}

void PageClientImpl::refView()
{
    notImplemented();
}

void PageClientImpl::derefView()
{
    notImplemented();
}

}
