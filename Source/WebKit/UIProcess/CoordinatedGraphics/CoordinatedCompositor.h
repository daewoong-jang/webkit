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

#ifndef CoordinatedCompositor_h
#define CoordinatedCompositor_h

#if USE(COORDINATED_GRAPHICS_MULTIPROCESS)

#include "CoordinatedGraphicsScene.h"
#include <WebCore/IntSize.h>
#include <WebCore/TextureMapper.h>
#include <wtf/FastMalloc.h>
#include <wtf/Noncopyable.h>
#include <wtf/ThreadSafeRefCounted.h>

namespace WebKit {

class CoordinatedCompositor : public CoordinatedGraphicsSceneClient, public ThreadSafeRefCounted<CoordinatedCompositor> {
    WTF_MAKE_NONCOPYABLE(CoordinatedCompositor);
    WTF_MAKE_FAST_ALLOCATED;
public:
    class Client {
    public:
        virtual void renderNextFrame() = 0;
        virtual void commitScrollOffset(uint32_t layerID, const WebCore::IntSize& offset) = 0;
        virtual void scheduleDisplay() = 0;
    };

    static Ref<CoordinatedCompositor> create(Client&);
    virtual ~CoordinatedCompositor();

    CoordinatedGraphicsScene* scene() { return m_scene.get(); }

    void initialize(const WebCore::IntSize&, float scaleFactor);

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

    void renderLayerTree();

    void purgeGLResources();
    void detach();
    void attach();

private:
    CoordinatedCompositor(Client&);

    void scheduleDisplay();

    // CoordinatedGraphicsSceneClient
    void renderNextFrame() override;
    void updateViewport() override;
    void commitScrollOffset(uint32_t layerID, const WebCore::IntSize& offset) override;

    Client& m_client;
    RefPtr<CoordinatedGraphicsScene> m_scene;

    Lock m_lock;
    WebCore::IntSize m_viewportSize;
    WebCore::IntPoint m_scrollPosition;
    float m_scaleFactor { 1 };
    bool m_drawsBackground { true };
    bool m_needsResize { false };
    bool m_purge { false };
    Vector<WebCore::CoordinatedGraphicsState> m_states;
    Vector<uint32_t> m_atlasesToRemove;
};

} // namespace WebKit

#endif

#endif // CoordinatedCompositor_h
