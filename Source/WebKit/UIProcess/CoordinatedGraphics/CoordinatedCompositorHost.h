/*
 * Copyright (C) 2017 Daewoong Jang.
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

#ifndef CoordinatedCompositorHost_h
#define CoordinatedCompositorHost_h

#if USE(COORDINATED_GRAPHICS_MULTIPROCESS)

#include "CoordinatedCompositor.h"
#include "CoordinatedGraphicsArgumentCoders.h"
#include "MessageReceiver.h"

namespace WebKit {

class WebPageProxy;

class CoordinatedCompositorHost final : public IPC::MessageReceiver, public CoordinatedCompositor::Client {
    WTF_MAKE_NONCOPYABLE(CoordinatedCompositorHost);
    WTF_MAKE_FAST_ALLOCATED;
public:
    CoordinatedCompositorHost(WebPageProxy&);
    ~CoordinatedCompositorHost();

    bool isActive() const;
    void setActive(bool);

    void setVisibleContentsRect(const WebCore::FloatRect&, const WebCore::FloatPoint& trajectoryVector);
    void setBackgroundColor(int32_t color);
    int32_t getBackgroundColor();

    void paintToCurrentGLContext();
    void purgeGLResources();

    bool isDetached() const { return m_isDetached; }
    void detach();
    void attach(std::function<void ()>&&);

private:
    void initialize(const WebCore::IntSize& viewportSize, float scaleFactor);

    void setNativeSurfaceHandleForCompositing(uint64_t);
    void setScaleFactor(float);
    void setScrollPosition(const WebCore::IntPoint&, float scale);
    void setViewportSize(const WebCore::IntSize&, float scale);
    void setDrawsBackground(bool);

    void updateSceneState(const WebCore::CoordinatedGraphicsState&);
    void releaseUpdateAtlases(const Vector<uint32_t>&);

    void invalidate();

    void forceRepaint();

    void frameComplete();

    void didAttach(uint64_t);

    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    // CoordinatedCompositor::Client
    void renderNextFrame() override;
    void commitScrollOffset(uint32_t layerID, const WebCore::IntSize& offset) override;
    void scheduleDisplay() override;

    WebPageProxy& m_webPage;
    RefPtr<CoordinatedCompositor> m_compositor;
    bool m_isDetached { false };
    WebCore::FloatRect m_lastSentVisibleRect;
    WebCore::FloatPoint m_lastSentTrajectoryVector;
    std::function<void ()> m_stateCallback;
    // The state ID corresponding to our current compositor host. Updated whenever we update
    // state of the compositor host.
    uint64_t m_expectedCompositorStateID { 0 };
};

} // namespace WebKit

#endif

#endif // CoordinatedCompositorHost_h
