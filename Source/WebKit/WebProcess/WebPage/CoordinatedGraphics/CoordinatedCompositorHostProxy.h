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

#ifndef CoordinatedCompositorHostProxy_h
#define CoordinatedCompositorHostProxy_h

#if USE(COORDINATED_GRAPHICS_MULTIPROCESS)

#include "CoordinatedGraphicsArgumentCoders.h"
#include "MessageReceiver.h"
#include "ThreadedCompositor.h"

namespace WebKit {

class ThreadedCoordinatedLayerTreeHost;
class WebPage;

class CoordinatedCompositorHostProxy final : public IPC::MessageReceiver {
    WTF_MAKE_NONCOPYABLE(CoordinatedCompositorHostProxy);
    WTF_MAKE_FAST_ALLOCATED;
public:
    CoordinatedCompositorHostProxy(WebPage&, ThreadedCompositor::Client&, const WebCore::IntSize& viewportSize, float scaleFactor);
    ~CoordinatedCompositorHostProxy();

    void setNativeSurfaceHandleForCompositing(uint64_t);
    void setScaleFactor(float);
    void setScrollPosition(const WebCore::IntPoint&, float scale);
    void setViewportSize(const WebCore::IntSize&, float scale);
    void setDrawsBackground(bool);

    void updateSceneState(const WebCore::CoordinatedGraphicsState&);
    void releaseUpdateAtlases(Vector<uint32_t>);

    void invalidate();

    void forceRepaint();

    void frameComplete();

private:
    // IPC::MessageReceiver
    void didReceiveMessage(IPC::Connection&, IPC::Decoder&) final;

    void setVisibleContentsRect(const WebCore::FloatRect&, const WebCore::FloatPoint& trajectoryVector);
    void renderNextFrame();
    void commitScrollOffset(uint32_t layerID, const WebCore::IntSize& offset);

    void detach(uint64_t);
    void attach(uint64_t);

    WebPage& m_webPage;
    ThreadedCompositor::Client& m_client;
};

} // namespace WebKit

#endif

#endif // CoordinatedCompositorHostProxy_h
