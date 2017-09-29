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
#include "CoordinatedCompositorHost.h"

#if USE(COORDINATED_GRAPHICS_MULTIPROCESS)

#include "CoordinatedCompositorHostMessages.h"
#include "CoordinatedCompositorHostProxyMessages.h"
#include "WebPageProxy.h"
#include "WebProcessProxy.h"

using namespace WebCore;

namespace WebKit {

CoordinatedCompositorHost::CoordinatedCompositorHost(WebPageProxy& webPage)
    : m_webPage(webPage)
    , m_compositor(CoordinatedCompositor::create(*this))
{
    m_webPage.process().addMessageReceiver(Messages::CoordinatedCompositorHost::messageReceiverName(), m_webPage.pageID(), *this);
}

CoordinatedCompositorHost::~CoordinatedCompositorHost()
{
    m_webPage.process().removeMessageReceiver(Messages::CoordinatedCompositorHost::messageReceiverName(), m_webPage.pageID());
}

bool CoordinatedCompositorHost::isActive() const
{
    RefPtr<CoordinatedGraphicsScene> scene(m_compositor->scene());
    if (!scene)
        return false;

    return scene->isActive();
}

void CoordinatedCompositorHost::setActive(bool active)
{
    RefPtr<CoordinatedGraphicsScene> scene(m_compositor->scene());
    if (!scene || scene->isActive() == active)
        return;

    scene->setActive(active);
}

void CoordinatedCompositorHost::setVisibleContentsRect(const FloatRect& rect, const FloatPoint& trajectoryVector)
{
    if (rect == m_lastSentVisibleRect && trajectoryVector == m_lastSentTrajectoryVector)
        return;

    m_webPage.process().send(Messages::CoordinatedCompositorHostProxy::SetVisibleContentsRect(rect, trajectoryVector), m_webPage.pageID());
    m_lastSentVisibleRect = rect;
    m_lastSentTrajectoryVector = trajectoryVector;
}

void CoordinatedCompositorHost::setBackgroundColor(int32_t color)
{
    RefPtr<CoordinatedGraphicsScene> scene(m_compositor->scene());
    if (!scene)
        return;

    scene->setViewBackgroundColor(color);
}

int32_t CoordinatedCompositorHost::getBackgroundColor()
{
    RefPtr<CoordinatedGraphicsScene> scene(m_compositor->scene());
    if (!scene)
        return Color::white;

    return scene->viewBackgroundColor().rgb();
}

void CoordinatedCompositorHost::paintToCurrentGLContext()
{
    m_compositor->renderLayerTree();
}

void CoordinatedCompositorHost::purgeGLResources()
{
    m_compositor->purgeGLResources();
}

void CoordinatedCompositorHost::detach()
{
    ASSERT(isMainThread());

    if (m_isDetached)
        return;

    m_isDetached = true;

    m_compositor->detach();
    m_webPage.process().send(Messages::CoordinatedCompositorHostProxy::Detach(++m_expectedCompositorStateID), m_webPage.pageID());
}

void CoordinatedCompositorHost::attach(std::function<void ()>&& stateCallback)
{
    ASSERT(isMainThread());

    if (!m_isDetached)
        return;

    m_isDetached = false;

    m_stateCallback = WTFMove(stateCallback);
    m_webPage.process().send(Messages::CoordinatedCompositorHostProxy::Attach(++m_expectedCompositorStateID), m_webPage.pageID());
}

void CoordinatedCompositorHost::didAttach(uint64_t stateID)
{
    ASSERT(isMainThread());

    if (stateID != m_expectedCompositorStateID)
        return;

    m_compositor->attach();
    auto stateCallback = WTFMove(m_stateCallback);
    stateCallback();
}

void CoordinatedCompositorHost::initialize(const IntSize& viewportSize, float scaleFactor)
{
    m_compositor->initialize(viewportSize, scaleFactor);
}

void CoordinatedCompositorHost::setNativeSurfaceHandleForCompositing(uint64_t handle)
{
    m_compositor->setNativeSurfaceHandleForCompositing(handle);
}

void CoordinatedCompositorHost::setScaleFactor(float scale)
{
    m_compositor->setScaleFactor(scale);
}

void CoordinatedCompositorHost::setScrollPosition(const IntPoint& scrollPosition, float scale)
{
    m_compositor->setScrollPosition(scrollPosition, scale);
}

void CoordinatedCompositorHost::setViewportSize(const IntSize& viewportSize, float scale)
{
    m_compositor->setViewportSize(viewportSize, scale);
}

void CoordinatedCompositorHost::setDrawsBackground(bool drawsBackground)
{
    m_compositor->setDrawsBackground(drawsBackground);
}

void CoordinatedCompositorHost::updateSceneState(const CoordinatedGraphicsState& state)
{
    m_compositor->updateSceneState(state);
}

void CoordinatedCompositorHost::releaseUpdateAtlases(const Vector<uint32_t>& atlasesToRemove)
{
    m_compositor->releaseUpdateAtlases(atlasesToRemove);
}

void CoordinatedCompositorHost::invalidate()
{
    m_compositor->invalidate();
}

void CoordinatedCompositorHost::forceRepaint()
{
    m_compositor->forceRepaint();
}

void CoordinatedCompositorHost::frameComplete()
{
    m_compositor->frameComplete();
}

void CoordinatedCompositorHost::renderNextFrame()
{
    m_webPage.send(Messages::CoordinatedCompositorHostProxy::RenderNextFrame(), m_webPage.pageID());
}

void CoordinatedCompositorHost::commitScrollOffset(uint32_t layerID, const IntSize& offset)
{
    m_webPage.send(Messages::CoordinatedCompositorHostProxy::CommitScrollOffset(layerID, offset), m_webPage.pageID());
}

void CoordinatedCompositorHost::scheduleDisplay()
{
    m_webPage.setViewNeedsDisplay(IntRect(IntPoint::zero(), m_webPage.viewSize()));
}

}
#endif // USE(COORDINATED_GRAPHICS_MULTIPROCESS)
