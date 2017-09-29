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

#include "config.h"
#include "WebViewPrivate.h"

#include "WTFMainThread.h"
#include "WebKitProcess.h"
#include "WebProcessContext.h"
#include "WebView.h"
#include "WebViewContext.h"
#include <GLES2/gl2.h>
#include <android/content/Context.h>
#include <android/content/res/Configuration.h>
#include <android/content/res/Resources.h>
#include <android/view/InputDevice.h>
#include <android/view/MotionEvent.h>
#include <android/view/inputmethod/InputMethodManager.h>
#include <android++/Functional.h>

#include <algorithm>

namespace org {
namespace webkit {

class WebViewPrivate::ViewObserver {
public:
    ViewObserver(WebViewPrivate&);
    ~ViewObserver();

private:
    WebViewPrivate& mThat;
    WebViewContext* mForegroundView;
};

WebViewPrivate::ViewObserver::ViewObserver(WebViewPrivate& that)
    : mThat(that)
    , mForegroundView(mThat.getForegroundView().get())
{
}

WebViewPrivate::ViewObserver::~ViewObserver()
{
    WebViewContext* foregroundView = mThat.getForegroundView().get();
    if (foregroundView == mForegroundView)
        return;

    WebViewContext* backgroundView = mForegroundView;
    if (mThat.isAttachedToWindow() && mThat.getVisibility() == View::VISIBLE) {
        if (backgroundView) {
            backgroundView->setActive(false);
            backgroundView->setVisible(false);
        }
        if (foregroundView) {
            foregroundView->setActive(true);
            foregroundView->setVisible(true);
        }
    }

    if (mThat.hasFocus()) {
        if (backgroundView)
            backgroundView->setFocus(false);
        if (foregroundView)
            foregroundView->setFocus(true);
    }

    mThat.mMotionDetector.reset();
    mThat.rebuildRenderList();
}

static Context* mApplicationContext = nullptr;

WebViewPrivate::WebViewPrivate(Context& context, WebView& webView)
    : ContainerView(context)
    , mWebView(webView)
    , mProcessContext(std::make_shared<WebProcessContext>())
    , mConfiguration(AWKWebContentConfiguration::create())
    , mMotionDetector(*this)
    , mHandler(std::make_shared<Handler>())
{
    setFocusable(true);

    mApplicationContext = &context;
    mConfiguration->setProcessPool(mProcessContext->getProcessPool());
}

WebViewPrivate::~WebViewPrivate()
{
    if (mSurfaceView)
        mSurfaceView->setRenderer(nullptr);
}

Context& WebViewPrivate::getApplicationContext()
{
    if (WebKitProcess::getCurrent() != nullptr)
        return *WebKitProcess::getCurrent();
    return *mApplicationContext;
}

const std::shared_ptr<GLSurfaceView>& WebViewPrivate::getSurfaceView()
{
    if (!mSurfaceView) {
        mSurfaceView.reset(new GLSurfaceView(getContext()));
        mSurfaceView->setEGLContextClientVersion(2);
        mSurfaceView->setRenderer(this);
        mSurfaceView->setRenderMode(GLSurfaceView::RENDERMODE_WHEN_DIRTY);
        mSurfaceView->queueEvent([] { WTFMainThread::initializeUIWorkerThread(); });
    }

    return mSurfaceView;
}

bool WebViewPrivate::isVisible()
{
    return mIsVisible;
}

void WebViewPrivate::addView(const std::shared_ptr<WebViewContext>& view)
{
    ViewObserver observer(*this);

    mViewList.push_back(view);

    if (isAttachedToWindow()) {
        view->setVisible(isVisible());
        view->onLayout(getWidth(), getHeight());
    }
}

void WebViewPrivate::removeView(const std::shared_ptr<WebViewContext>& view)
{
    ViewObserver observer(*this);

    auto target = std::find(mViewList.begin(), mViewList.end(), view);
    if (target == mViewList.end())
        return;

    view->setVisible(false);
    view->onPause();
    deleteView(view);
    mViewList.erase(target);
}

void WebViewPrivate::moveAfter(const std::shared_ptr<WebViewContext>& view, const std::shared_ptr<WebViewContext>& after)
{
    ViewObserver observer(*this);

    auto anchor = std::find(mViewList.begin(), mViewList.end(), after);
    if (anchor == mViewList.end())
        return;

    auto target = std::find(mViewList.begin(), mViewList.end(), view);
    if (target == mViewList.end() || anchor + 1 == target)
        return;

    std::swap(*(anchor + 1), *(target));
}

void WebViewPrivate::bringChildToFront(const std::shared_ptr<WebViewContext>& view)
{
    ViewObserver observer(*this);

    auto target = std::find(mViewList.begin(), mViewList.end(), view);
    if (target == mViewList.begin() || target == mViewList.end())
        return;

    std::rotate(mViewList.begin(), target, target + 1);
}

void WebViewPrivate::sendToBack(const std::shared_ptr<WebViewContext>& view)
{
    ViewObserver observer(*this);

    auto target = std::find(mViewList.begin(), mViewList.end(), view);
    if (*target == mViewList.back() || target == mViewList.end())
        return;

    std::move(target, target + 1, mViewList.end());
}

std::shared_ptr<WebViewContext> WebViewPrivate::createView()
{
    auto view = std::make_shared<WebViewContext>(*this, mConfiguration);
    DisplayMetrics& metrics = Resources::getSystem().getDisplayMetrics();
    view->getView()->setIntrinsicDeviceScaleFactor(metrics.density);
    view->getView()->setUseFixedLayout(true);
    addView(view);
    return view;
}

const std::shared_ptr<WebViewContext>& WebViewPrivate::getForegroundView()
{
    static std::shared_ptr<WebViewContext> nullPtr;
    if (mViewList.empty())
        return nullPtr;

    return mViewList[0];
}

float WebViewPrivate::getDeviceScaleFactor()
{
    return getContext().getResources().getDisplayMetrics().density;
}

void WebViewPrivate::onPause()
{
    mSurfaceView->onPause();
    propagate(mViewList, &WebViewContext::onPause);
}

void WebViewPrivate::onResume()
{
    mSurfaceView->onResume();
    propagate(mViewList, &WebViewContext::onResume);
    rebuildRenderList();
}

static inline bool isClickEvent(InputEvent& event)
{
    if (event.getSource() != InputDevice::SOURCE_MOUSE)
        return false;

    MotionEvent& mouseEvent = static_cast<MotionEvent&>(event);
    if (mouseEvent.getActionMasked() != MotionEvent::ACTION_UP)
        return false;
    if (mouseEvent.getSize() != 1)
        return false;
    if (mouseEvent.getButtonState() & MotionEvent::BUTTON_PRIMARY)
        return true;

    return true;
}

bool WebViewPrivate::sendGenericEvent(InputEvent& event)
{
    if (!mMotionDetector.dispatchGenericEvent(event)) {
        if (hasOnClickListeners() && isClickEvent(event)) {
            callOnClick();
            return true;
        }

        return false;
    }

    return true;
}

bool WebViewPrivate::sendUnhandledTouchEvent(InputEvent& event)
{
    return mMotionDetector.dispatchUnhandledTouchEvent(event);
}

void WebViewPrivate::resetMotionEvent()
{
    mMotionDetector.reset();
}

std::shared_ptr<WebEditableContext> WebViewPrivate::getEditableContext()
{
    if (!getForegroundView())
        return nullptr;

    return getForegroundView()->getEditable();
}

void WebViewPrivate::invalidate()
{
    mWebView.invalidate();
}

void WebViewPrivate::invalidate(Rect&)
{
    mWebView.invalidate();
}

void WebViewPrivate::deleteView(const std::shared_ptr<WebViewContext>& view)
{
    std::shared_ptr<WebViewContext> localView = view;
    std::shared_ptr<WebRenderContext> renderer = localView->getRenderer();
    mSurfaceView->queueEvent([=] {
        renderer->stop();
        mHandler->post([localView] () mutable {
            localView.reset();
        });
    });
}

void WebViewPrivate::rebuildRenderList()
{
    if (!isVisible())
        return;

    if (mViewList.size() == 0)
        return;

    std::vector<std::shared_ptr<WebRenderContext>> renderers;
    for (auto& view : mViewList)
        renderers.push_back(view->getRenderer());

    mPendingRenderList = std::make_shared<WebRenderContextList>(std::move(renderers));
    mSurfaceView->requestRender();
}

void WebViewPrivate::destroyRenderListOnMainThread()
{
    if (mCurrentRenderList) {
        std::shared_ptr<WebRenderContextList> protect(std::move(mCurrentRenderList));
        mHandler->post([protect] {});
    }
}

void WebViewPrivate::onVisibilityChanged()
{
    bool visible = mViewVisibility == View::VISIBLE && mWindowVisibility == View::VISIBLE;
    if (visible == mIsVisible)
        return;

    mIsVisible = visible;

    if (!getForegroundView())
        return;

    getForegroundView()->setVisible(isVisible());

    if (visible) {
        rebuildRenderList();
        mSurfaceView->requestRender();
    } else
        mPendingRenderList.reset();
}

void WebViewPrivate::doneWithTouchEvent(bool wasEventHandled)
{
    MotionEvent event = mTouchEvents.front();
    mTouchEvents.erase(mTouchEvents.begin());
    if (!wasEventHandled) {
        int32_t location[2];
        getLocationOnScreen(location);
        event.offsetLocation(-location[0], -location[1]);
        sendUnhandledTouchEvent(event);
        event.offsetLocation(location[0], location[1]);
    }
}

void WebViewPrivate::editorStateChanged(String text, int32_t selectionStart, int32_t selectionEnd, int32_t compositionStart, int32_t compositionEnd)
{
    if (!mInputConnection)
        return;
    mInputConnection->updateInputState(text, selectionStart, selectionEnd, compositionStart, compositionEnd);
}

bool WebViewPrivate::onGenericMotionEvent(MotionEvent& event)
{
    if (mMotionDetector.dispatchGenericEvent(event))
        return true;
    return View::onGenericMotionEvent(event);
}

void WebViewPrivate::onWindowFocusChanged(bool hasWindowFocus)
{
    View::onWindowFocusChanged(hasWindowFocus);

    if (!getForegroundView())
        return;

    getForegroundView()->getView()->onFocusChanged(hasWindowFocus);
    getForegroundView()->setFocus(hasWindowFocus);
    rebuildRenderList();
}

void WebViewPrivate::onAttachedToWindow()
{
    View::onAttachedToWindow();
}

void WebViewPrivate::onDetachedFromWindow()
{
    View::onDetachedFromWindow();
    getForegroundView()->setVisible(false);
}

std::shared_ptr<InputConnection> WebViewPrivate::onCreateInputConnection(EditorInfo& outAttrs)
{
    if (!mInputConnection)
        mInputConnection = std::make_shared<WebEditableInputConnection>(getEditableContext(), *this, true);
    return mInputConnection;
}

void WebViewPrivate::onVisibilityChanged(View* changedView, int32_t visibility)
{
    View::onVisibilityChanged(changedView, visibility);
    if (visibility == mViewVisibility)
        return;

    mViewVisibility = visibility;
    onVisibilityChanged();
}

void WebViewPrivate::onWindowVisibilityChanged(int32_t visibility)
{
    View::onWindowVisibilityChanged(visibility);
    if (visibility == mWindowVisibility)
        return;

    mWindowVisibility = visibility;
    onVisibilityChanged();
}

void WebViewPrivate::onMeasure(int32_t widthMeasureSpec, int32_t heightMeasureSpec)
{
    View::onMeasure(widthMeasureSpec, heightMeasureSpec);
}

void WebViewPrivate::onLayout(bool changed, int32_t left, int32_t top, int32_t right, int32_t bottom)
{
    View::onLayout(changed, left, top, right, bottom);

    if (!changed)
        return;

    propagate(mViewList, &WebViewContext::onLayout, getWidth(), getHeight());
    rebuildRenderList();
}

void WebViewPrivate::onConfigurationChanged(Configuration& newConfig)
{
    View::onConfigurationChanged(newConfig);
    propagate(mViewList, &WebViewContext::onConfigurationChanged, newConfig);
}

void WebViewPrivate::onSurfaceCreated(GL10 gl)
{
    if (mSurfaceCreatedOnce) {
        if (mCurrentRenderList)
            mCurrentRenderList->contextLost();
    } else {
        mSurfaceCreatedOnce = true;
    }

    WebRenderer::surfaceCreated();
}

void WebViewPrivate::onSurfaceChanged(GL10 gl, int32_t width, int32_t height)
{
}

void WebViewPrivate::onDrawFrame(GL10 gl)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if (!mIsVisible) {
        destroyRenderListOnMainThread();
        mCurrentRenderList.reset();
        mPendingRenderList.reset();
    }

    if (mPendingRenderList) {
        destroyRenderListOnMainThread();
        mCurrentRenderList = std::move(mPendingRenderList);
    }

    if (!mCurrentRenderList)
        return;

    mCurrentRenderList->paint();
}

MotionDetector::Settings WebViewPrivate::clientSettings()
{
    if (!getForegroundView())
        return MotionDetector::Settings();

    return getForegroundView()->getMotionSettings();
}

float WebViewPrivate::clientWidth()
{
    return getWidth();
}

float WebViewPrivate::clientHeight()
{
    return getHeight();
}

float WebViewPrivate::clientScaleFactor()
{
    if (!getForegroundView())
        return 1.0f;

    return getForegroundView()->getCurrentScale();
}

float WebViewPrivate::clientMinimumScaleFactor()
{
    if (!getForegroundView())
        return 1.0f;

    return getForegroundView()->getMinimumScale();
}

float WebViewPrivate::clientMaximumScaleFactor()
{
    if (!getForegroundView())
        return 1.0f;

    return getForegroundView()->getMaximumScale();
}

float WebViewPrivate::clientContentWidth()
{
    if (!getForegroundView())
        return 0;

    return getForegroundView()->getContentWidth();
}

float WebViewPrivate::clientContentHeight()
{
    if (!getForegroundView())
        return 0;

    return getForegroundView()->getContentHeight();
}

RectF WebViewPrivate::clientContentViewport()
{
    if (!getForegroundView())
        return RectF();

    return getForegroundView()->getViewport();
}

void WebViewPrivate::handleSingleTap(float x, float y)
{
    if (!getForegroundView())
        return;

    return getForegroundView()->sendSingleTap(x, y);
}

void WebViewPrivate::handleSingleTapUnconfirmed(float x, float y)
{
}

void WebViewPrivate::handleDoubleTap(float x, float y)
{
    if (!getForegroundView())
        return;

    return getForegroundView()->sendDoubleTap(x, y);
}

void WebViewPrivate::handleLongTap(float x, float y)
{
}

void WebViewPrivate::handleShowPressState(float x, float y)
{
}

void WebViewPrivate::handleShowPressCancel(float x, float y)
{
}

void WebViewPrivate::handleLongPress(float x, float y)
{
}

void WebViewPrivate::handleScrollBegin(float x, float y)
{
}

void WebViewPrivate::handleScrollBy(float x, float y, float deltaX, float deltaY)
{
    if (!getForegroundView())
        return;

    getForegroundView()->scrollBy(deltaX, deltaY);

    invalidate();
}

void WebViewPrivate::handleScrollEnd(float x, float y)
{
}

void WebViewPrivate::handleFlingStart(float x, float y, int32_t vx, int32_t vy)
{
}

void WebViewPrivate::handleFlingCancel()
{
}

void WebViewPrivate::handlePinchBegin(float x, float y)
{
    if (!getForegroundView())
        return;

    getForegroundView()->pinchBegin(x, y);
}

void WebViewPrivate::handlePinchTo(RectF& rect)
{
    if (!getForegroundView())
        return;

    getForegroundView()->pinchTo(rect);

    invalidate();
}

void WebViewPrivate::handlePinchBy(float x, float y, float magnification)
{
    if (!getForegroundView())
        return;

    getForegroundView()->pinchBy(x, y, magnification);

    invalidate();
}

void WebViewPrivate::handlePinchEnd()
{
    if (!getForegroundView())
        return;

    getForegroundView()->pinchEnd();
}

static int32_t mTouchEventId = 0;

bool WebViewPrivate::handleTouchEvent(MotionEvent& event)
{
    if (!getForegroundView())
        return false;

    if (event.getActionMasked() == MotionEvent::ACTION_DOWN)
        ++mTouchEventId;
    mTouchEvents.push_back(event);
    return getForegroundView()->sendTouchEvent(event, mTouchEventId);
}

bool WebViewPrivate::handleUnconfirmed(InputEvent& event)
{
    if (!getForegroundView())
        return false;

    return getForegroundView()->sendGenericEvent(event);
}

} // namespace webkit
} // namespace org
