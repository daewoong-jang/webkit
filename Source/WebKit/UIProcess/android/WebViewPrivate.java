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
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Rect;
import android.graphics.RectF;
import android.opengl.GLSurfaceView;
import android.os.Handler;
import android.util.DisplayMetrics;
import android.view.InputDevice;
import android.view.InputEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;

import java.util.Collections;
import java.util.Vector;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import org.webkit.launcher.WebKitProcess;
import org.webkit.wtf.WTFMainThread;

public final class WebViewPrivate extends ContainerView implements
        GLSurfaceView.Renderer, MotionDetector.Client {
    class ViewObserver {
        private WebViewContext mForegroundView;

        public ViewObserver() {
            mForegroundView = getForegroundView();
        }

        public void exit() {
            WebViewContext foregroundView = getForegroundView();
            if (foregroundView == mForegroundView)
                return;

            WebViewContext backgroundView = mForegroundView;
            if (isAttachedToWindow() && getVisibility() == View.VISIBLE) {
                if (backgroundView != null) {
                    backgroundView.setActive(false);
                    backgroundView.setVisible(false);
                }
                if (foregroundView != null) {
                    foregroundView.setActive(true);
                    foregroundView.setVisible(true);
                }
            }

            if (hasFocus()) {
                if (backgroundView != null)
                    backgroundView.setFocus(false);
                if (foregroundView != null)
                    foregroundView.setFocus(true);
            }

            mMotionDetector.reset();
            rebuildRenderList();
        }
    }

    private static Context mApplicationContext = null;

    private WebView mWebView;

    private GLSurfaceView mSurfaceView;
    private int mViewVisibility = View.INVISIBLE;
    private int mWindowVisibility = View.INVISIBLE;
    private boolean mIsVisible = false;
    private boolean mSurfaceCreatedOnce = false;

    private WebProcessContext mProcessContext;
    private AWKWebContentConfiguration mConfiguration;
    private MotionDetector mMotionDetector;
    private Handler mHandler;

    protected WebViewPrivate(Context context, WebView webView) {
        super(context);

        mWebView = webView;
        mProcessContext = new WebProcessContext();
        mConfiguration = new AWKWebContentConfiguration();
        mMotionDetector = new MotionDetector(this);
        mHandler = new Handler();

        setClickable(true);
        setFocusableInTouchMode(true);

        mApplicationContext = context;
        mConfiguration.setProcessPool(mProcessContext.getProcessPool());
    }

    public static Context getApplicationContext() {
        if (WebKitProcess.getCurrent() != null)
            return WebKitProcess.getCurrent();
        return mApplicationContext;
    }

    public GLSurfaceView getSurfaceView() {
        if (mSurfaceView == null) {
            mSurfaceView = new GLSurfaceView(getContext());
            mSurfaceView.setEGLContextClientVersion(2);
            mSurfaceView.setRenderer(this);
            mSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
            mSurfaceView.queueEvent(new Runnable() {
                @Override
                public void run() {
                    WTFMainThread.initializeUIWorkerThread();
                }
            });
        }

        return mSurfaceView;
    }

    public boolean isVisible() {
        return mIsVisible;
    }

    private WebRenderContextList mCurrentRenderList = null;
    private WebRenderContextList mPendingRenderList = null;
    private Vector<WebViewContext> mViewList = new Vector<WebViewContext>();

    public void addView(WebViewContext view) {
        ViewObserver observer = new ViewObserver();

        mViewList.add(view);

        if (isAttachedToWindow()) {
            view.setVisible(isVisible());
            view.onLayout(getWidth(), getHeight());
        }

        observer.exit();
    }

    public void removeView(WebViewContext view) {
        ViewObserver observer = new ViewObserver();

        WebViewContext target = mViewList.get(mViewList.indexOf(view));
        if (target == null)
            return;

        view.setVisible(false);
        view.onPause();
        deleteView(view);
        mViewList.remove(view);
        observer.exit();
    }

    public void moveAfter(WebViewContext view, WebViewContext after) {
        ViewObserver observer = new ViewObserver();

        int anchor = mViewList.indexOf(after);
        if (anchor == -1)
            return;

        int target = mViewList.indexOf(view);
        if (target == -1 || anchor + 1 == target)
            return;

        Collections.swap(mViewList, anchor + 1, target);
        observer.exit();
    }

    public void bringChildToFront(WebViewContext view) {
        ViewObserver observer = new ViewObserver();

        int target = mViewList.indexOf(view);
        if (target == 0 || target == -1)
            return;

        Collections.rotate(mViewList, -target);
        observer.exit();
    }

    public void sendToBack(WebViewContext view) {
        ViewObserver observer = new ViewObserver();

        int target = mViewList.indexOf(view);
        if (target == mViewList.size() - 1 || target == -1)
            return;

        mViewList.addElement(mViewList.remove(target));
        observer.exit();
    }

    public WebViewContext createView() {
        WebViewContext view = new WebViewContext(this, mConfiguration);
        DisplayMetrics metrics = Resources.getSystem().getDisplayMetrics();
        view.getView().setIntrinsicDeviceScaleFactor(metrics.density);
        view.getView().setUseFixedLayout(true);
        addView(view);
        return view;
    }

    public WebViewContext getForegroundView() {
        if (mViewList.isEmpty())
            return null;

        return mViewList.elementAt(0);
    }

    public float getDeviceScaleFactor() {
        return getContext().getResources().getDisplayMetrics().density;
    }

    protected void onPause() {
        mSurfaceView.onPause();
        for (WebViewContext view : mViewList)
            view.onPause();
    }

    protected void onResume() {
        mSurfaceView.onResume();
        for (WebViewContext view : mViewList)
            view.onResume();
        rebuildRenderList();
    }

    private static boolean isClickEvent(InputEvent event) {
        if (event.getSource() != InputDevice.SOURCE_MOUSE)
            return false;

        MotionEvent mouseEvent = (MotionEvent) event;
        if (mouseEvent.getActionMasked() != MotionEvent.ACTION_UP)
            return false;
        if (mouseEvent.getSize() != 1)
            return false;
        if ((mouseEvent.getButtonState() & MotionEvent.BUTTON_PRIMARY) != 0)
            return true;

        return true;
    }

    public boolean sendGenericEvent(final InputEvent event) {
        if (!mMotionDetector.dispatchGenericEvent(event)) {
            if (hasOnClickListeners() && isClickEvent(event)) {
                callOnClick();
                return true;
            }

            return false;
        }

        return true;
    }

    public boolean sendUnhandledTouchEvent(InputEvent event) {
        return mMotionDetector.dispatchUnhandledTouchEvent(event);
    }

    public void resetMotionEvent() {
        mMotionDetector.reset();
    }

    public WebEditableContext getEditableContext() {
        if (getForegroundView() == null)
            return null;

        return getForegroundView().getEditable();
    }

    public void invalidate() {
        mWebView.invalidate();
    }

    public void invalidate(Rect rect) {
        mWebView.invalidate();
    }

    public void deleteView(WebViewContext view) {
        final WebViewContext localView = view;
        final WebRenderContext renderer = localView.getRenderer();
        mSurfaceView.queueEvent(new Runnable() {
            @Override
            public void run() {
                renderer.stop(null);
                mHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        localView.hashCode();
                    }
                });
            }
        });
    }

    private void rebuildRenderList() {
        if (!isVisible())
            return;

        if (mViewList.size() == 0)
            return;

        Vector<WebRenderContext> renderers = new Vector<WebRenderContext>();
        for (WebViewContext view : mViewList)
            renderers.add(view.getRenderer());

        mPendingRenderList = new WebRenderContextList(renderers);
        mSurfaceView.requestRender();
    }

    private void destroyRenderListOnMainThread() {
        if (mCurrentRenderList != null) {
            final WebRenderContextList protect = mCurrentRenderList;
            mCurrentRenderList = null;
            mHandler.post(new Runnable() {
                @Override
                public void run() {
                    protect.hashCode();
                }
            });
        }
    }

    private void onVisibilityChanged() {
        boolean visible = mViewVisibility == View.VISIBLE
                && mWindowVisibility == View.VISIBLE;
        if (visible == mIsVisible)
            return;

        mIsVisible = visible;

        if (getForegroundView() == null)
            return;

        getForegroundView().setVisible(isVisible());

        if (visible) {
            rebuildRenderList();
            mSurfaceView.requestRender();
        } else
            mPendingRenderList = null;
    }

    private boolean mIsAttachedToWindow = false;

    protected boolean isAttachedToWindow() {
        return mIsAttachedToWindow;
    }

    private final Vector<MotionEvent> mTouchEvents = new Vector<MotionEvent>();
    private static int mTouchEventId = 0;
    private WebEditableInputConnection mInputConnection = null;

    @Override
    protected void doneWithTouchEvent(boolean wasEventHandled) {
        MotionEvent event = mTouchEvents.get(0);
        mTouchEvents.remove(0);
        if (!wasEventHandled) {
            int[] location = new int[2];
            getLocationOnScreen(location);
            event.offsetLocation(-location[0], -location[1]);
            sendUnhandledTouchEvent(event);
            event.offsetLocation(location[0], location[1]);
        }
    }

    @Override
    protected void editorStateChanged(String text, int selectionStart,
            int selectionEnd, int compositionStart, int compositionEnd) {
        if (mInputConnection == null)
            return;
        mInputConnection.updateInputState(text, selectionStart, selectionEnd,
                compositionStart, compositionEnd);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (mMotionDetector.dispatchGenericEvent(event))
            return true;
        return super.onTouchEvent(event);
    }

    @Override
    public void onWindowFocusChanged(boolean hasWindowFocus) {
        super.onWindowFocusChanged(hasWindowFocus);

        if (getForegroundView() == null)
            return;

        getForegroundView().getView().onFocusChanged(hasWindowFocus);
        getForegroundView().setFocus(hasWindowFocus);
        rebuildRenderList();
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        mIsAttachedToWindow = true;
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        mIsAttachedToWindow = false;
        getForegroundView().setVisible(false);
    }

    @Override
    public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
        if (mInputConnection == null)
            mInputConnection = new WebEditableInputConnection(
                    getEditableContext(), this, true);
        return mInputConnection;
    }

    @Override
    protected void onVisibilityChanged(View changedView, int visibility) {
        super.onVisibilityChanged(changedView, visibility);
        if (visibility == mViewVisibility)
            return;

        mViewVisibility = visibility;
        onVisibilityChanged();
    }

    @Override
    protected void onWindowVisibilityChanged(int visibility) {
        super.onWindowVisibilityChanged(visibility);
        if (visibility == mWindowVisibility)
            return;

        mWindowVisibility = visibility;
        onVisibilityChanged();
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right,
            int bottom) {
        super.onLayout(changed, left, top, right, bottom);

        if (!changed)
            return;

        for (WebViewContext view : mViewList)
            view.onLayout(right - left, bottom - top);
        rebuildRenderList();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        for (WebViewContext view : mViewList)
            view.onConfigurationChanged(newConfig);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        if (mSurfaceCreatedOnce) {
            if (mCurrentRenderList != null)
                mCurrentRenderList.contextLost();
        } else {
            mSurfaceCreatedOnce = true;
        }

        WebRenderer.surfaceCreated();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        gl.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT
                | GL10.GL_STENCIL_BUFFER_BIT);

        if (!mIsVisible) {
            destroyRenderListOnMainThread();
            mCurrentRenderList = null;
            mPendingRenderList = null;
        }

        if (mPendingRenderList != null) {
            destroyRenderListOnMainThread();
            mCurrentRenderList = mPendingRenderList;
            mPendingRenderList = null;
        }

        if (mCurrentRenderList == null)
            return;

        mCurrentRenderList.paint();
    }

    @Override
    public MotionDetector.Settings clientSettings() {
        if (getForegroundView() == null)
            return new MotionDetector.Settings();

        return getForegroundView().getMotionSettings();
    }

    @Override
    public float clientWidth() {
        return getWidth();
    }

    @Override
    public float clientHeight() {
        return getHeight();
    }

    @Override
    public float clientScaleFactor() {
        if (getForegroundView() == null)
            return 1.0f;

        return getForegroundView().getCurrentScale();
    }

    @Override
    public float clientMinimumScaleFactor() {
        if (getForegroundView() == null)
            return 1.0f;

        return getForegroundView().getMinimumScale();
    }

    @Override
    public float clientMaximumScaleFactor() {
        if (getForegroundView() == null)
            return 1.0f;

        return getForegroundView().getMaximumScale();
    }

    @Override
    public float clientContentWidth() {
        if (getForegroundView() == null)
            return 0.0f;

        return getForegroundView().getContentWidth();
    }

    @Override
    public float clientContentHeight() {
        if (getForegroundView() == null)
            return 0.0f;

        return getForegroundView().getContentHeight();
    }

    @Override
    public RectF clientContentViewport() {
        if (getForegroundView() == null)
            return new RectF();

        return getForegroundView().getViewport();
    }

    @Override
    public void handleSingleTap(float x, float y) {
        if (getForegroundView() == null)
            return;

        getForegroundView().sendSingleTap(x, y);
    }

    @Override
    public void handleSingleTapUnconfirmed(float x, float y) {
    }

    @Override
    public void handleDoubleTap(float x, float y) {
        if (getForegroundView() == null)
            return;

        getForegroundView().sendDoubleTap(x, y);
    }

    @Override
    public void handleLongTap(float x, float y) {
    }

    @Override
    public void handleShowPressState(float x, float y) {
    }

    @Override
    public void handleShowPressCancel(float x, float y) {
    }

    @Override
    public void handleLongPress(float x, float y) {
    }

    @Override
    public void handleScrollBegin(float x, float y) {
    }

    @Override
    public void handleScrollBy(float x, float y, float deltaX, float deltaY) {
        if (getForegroundView() == null)
            return;

        getForegroundView().scrollBy(deltaX, deltaY);

        invalidate();
    }

    @Override
    public void handleScrollEnd(float x, float y) {
    }

    @Override
    public void handleFlingStart(float x, float y, int vx, int vy) {
    }

    @Override
    public void handleFlingCancel() {
    }

    @Override
    public void handlePinchBegin(float x, float y) {
        if (getForegroundView() == null)
            return;

        getForegroundView().pinchBegin(x, y);
    }

    @Override
    public void handlePinchTo(RectF rect) {
        if (getForegroundView() == null)
            return;

        getForegroundView().pinchTo(rect);

        invalidate();
    }

    @Override
    public void handlePinchBy(float x, float y, float magnification) {
        if (getForegroundView() == null)
            return;

        getForegroundView().pinchBy(x, y, magnification);

        invalidate();
    }

    @Override
    public void handlePinchEnd() {
        if (getForegroundView() == null)
            return;

        getForegroundView().pinchEnd();
    }

    @Override
    public boolean handleTouchEvent(MotionEvent event) {
        if (getForegroundView() == null)
            return false;

        if (event.getActionMasked() == MotionEvent.ACTION_DOWN)
            ++mTouchEventId;
        mTouchEvents.addElement(event);
        return getForegroundView().sendTouchEvent(event, mTouchEventId);
    }

    @Override
    public boolean handleUnconfirmed(InputEvent event) {
        if (getForegroundView() == null)
            return false;

        return getForegroundView().sendGenericEvent(event);
    }
}
