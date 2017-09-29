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

#include "config.h"
#include "CoordinatedCompositor.h"

#if USE(COORDINATED_GRAPHICS_MULTIPROCESS)

#if USE(OPENGL_ES_2)
#include <GLES2/gl2.h>
#else
#include <GL/gl.h>
#endif

using namespace WebCore;

namespace WebKit {

Ref<CoordinatedCompositor> CoordinatedCompositor::create(Client& client)
{
    return adoptRef(*new CoordinatedCompositor(client));
}

CoordinatedCompositor::CoordinatedCompositor(Client& client)
    : m_client(client)
    , m_scene(adoptRef(new CoordinatedGraphicsScene(this)))
{
    m_scene->setActive(true);
}

CoordinatedCompositor::~CoordinatedCompositor()
{
    if (m_scene)
        m_scene->detach();
}

void CoordinatedCompositor::initialize(const IntSize& viewportSize, float scaleFactor)
{
    LockHolder locker(m_lock);
    m_viewportSize = viewportSize;
    m_scaleFactor = scaleFactor;
    m_needsResize = !viewportSize.isEmpty();
    scheduleDisplay();
}

void CoordinatedCompositor::setNativeSurfaceHandleForCompositing(uint64_t handle)
{
    ASSERT_NOT_REACHED();
}

void CoordinatedCompositor::setScaleFactor(float scale)
{
    LockHolder locker(m_lock);
    m_scaleFactor = scale;
    scheduleDisplay();
}

void CoordinatedCompositor::setScrollPosition(const IntPoint& scrollPosition, float scale)
{
    LockHolder locker(m_lock);
    m_scrollPosition = scrollPosition;
    m_scaleFactor = scale;
    scheduleDisplay();
}

void CoordinatedCompositor::setViewportSize(const IntSize& viewportSize, float scale)
{
    LockHolder locker(m_lock);
    m_viewportSize = viewportSize;
    m_scaleFactor = scale;
    m_needsResize = true;
    scheduleDisplay();
}

void CoordinatedCompositor::setDrawsBackground(bool drawsBackground)
{
    LockHolder locker(m_lock);
    m_drawsBackground = drawsBackground;
    scheduleDisplay();
}

void CoordinatedCompositor::updateSceneState(const CoordinatedGraphicsState& state)
{
    ASSERT(isMainThread());

    LockHolder locker(m_lock);
    if (!m_scene || !m_scene->isActive())
        return;
    m_states.append(state);
    scheduleDisplay();
}

void CoordinatedCompositor::releaseUpdateAtlases(const Vector<uint32_t>& atlasesToRemove)
{
    LockHolder locker(m_lock);
    m_atlasesToRemove.appendVector(atlasesToRemove);
    scheduleDisplay();
}

void CoordinatedCompositor::invalidate()
{
    LockHolder locker(m_lock);
    m_scene->detach();
    m_purge = true;
    scheduleDisplay();
}

void CoordinatedCompositor::forceRepaint()
{
    scheduleDisplay();
}

void CoordinatedCompositor::frameComplete()
{
}

void CoordinatedCompositor::renderLayerTree()
{
    if (!m_scene || !m_scene->isActive())
        return;

    WebCore::IntSize viewportSize;
    WebCore::IntPoint scrollPosition;
    float scaleFactor;
    bool drawsBackground;
    bool needsResize;
    bool purge;
    Vector<WebCore::CoordinatedGraphicsState> states;
    Vector<uint32_t> atlasesToRemove;

    {
        LockHolder locker(m_lock);
        viewportSize = m_viewportSize;
        scrollPosition = m_scrollPosition;
        scaleFactor = m_scaleFactor;
        drawsBackground = m_drawsBackground;
        needsResize = m_needsResize;
        purge = m_purge;

        states = WTFMove(m_states);
        atlasesToRemove = WTFMove(m_atlasesToRemove);

        if (!states.isEmpty()) {
            callOnMainThread([this] {
                renderNextFrame();
            });
        }

        m_needsResize = false;
        m_purge = false;
    }

    if (purge)
        purgeGLResources();

    if (needsResize)
        glViewport(0, 0, viewportSize.width(), viewportSize.height());

    TransformationMatrix viewportTransform;
    viewportTransform.scale(scaleFactor);
    viewportTransform.translate(-scrollPosition.x(), -scrollPosition.y());

    if (!m_drawsBackground) {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    m_scene->applyStateChanges(states);
    m_scene->releaseUpdateAtlases(atlasesToRemove);
    m_scene->paintToCurrentGLContext(viewportTransform, 1, FloatRect { FloatPoint { }, viewportSize },
        Color::transparent, !drawsBackground, scrollPosition, 0);
}

void CoordinatedCompositor::purgeGLResources()
{
    ASSERT(!isMainThread());

    RefPtr<CoordinatedGraphicsScene> scene(WTFMove(m_scene));
    if (!scene)
        return;

    scene->purgeGLResources();
}

void CoordinatedCompositor::detach()
{
    ASSERT(isMainThread());

    LockHolder locker(m_lock);
    RefPtr<CoordinatedGraphicsScene> scene(m_scene);
    scene->detach();
}

void CoordinatedCompositor::attach()
{
    ASSERT(isMainThread());

    LockHolder locker(m_lock);
    m_scene = adoptRef(new CoordinatedGraphicsScene(this));
    m_scene->setActive(true);
}

void CoordinatedCompositor::scheduleDisplay()
{
    ASSERT(isMainThread());
    m_client.scheduleDisplay();
}

void CoordinatedCompositor::renderNextFrame()
{
    ASSERT(isMainThread());
    m_client.renderNextFrame();
}

void CoordinatedCompositor::updateViewport()
{
    callOnMainThread([this, protectedThis = makeRefPtr(this)] {
        scheduleDisplay();
    });
}

void CoordinatedCompositor::commitScrollOffset(uint32_t layerID, const IntSize& offset)
{
    ASSERT(isMainThread());
    m_client.commitScrollOffset(layerID, offset);
}

}
#endif // USE(COORDINATED_GRAPHICS_MULTIPROCESS)
