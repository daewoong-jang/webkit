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
#include "CoordinatedCompositorHostProxy.h"

#if USE(COORDINATED_GRAPHICS_MULTIPROCESS)

#include "CoordinatedCompositorHostMessages.h"
#include "CoordinatedCompositorHostProxyMessages.h"
#include "DrawingArea.h"
#include "ThreadedCoordinatedLayerTreeHost.h"
#include "WebCoreArgumentCoders.h"
#include "WebPage.h"
#include "WebProcess.h"

using namespace WebCore;

namespace WebKit {

CoordinatedCompositorHostProxy::CoordinatedCompositorHostProxy(WebPage& webPage, ThreadedCompositor::Client& client, const IntSize& viewportSize, float scaleFactor)
    : m_webPage(webPage)
    , m_client(client)
{
    WebProcess::singleton().addMessageReceiver(Messages::CoordinatedCompositorHostProxy::messageReceiverName(), m_webPage.pageID(), *this);
    m_webPage.send(Messages::CoordinatedCompositorHost::Initialize(viewportSize, scaleFactor), m_webPage.pageID());
}

CoordinatedCompositorHostProxy::~CoordinatedCompositorHostProxy()
{
    WebProcess::singleton().removeMessageReceiver(Messages::CoordinatedCompositorHostProxy::messageReceiverName(), m_webPage.pageID());
}

void CoordinatedCompositorHostProxy::setNativeSurfaceHandleForCompositing(uint64_t handle)
{
    m_webPage.send(Messages::CoordinatedCompositorHost::SetNativeSurfaceHandleForCompositing(handle), m_webPage.pageID());
}

void CoordinatedCompositorHostProxy::setScaleFactor(float scale)
{
    m_webPage.send(Messages::CoordinatedCompositorHost::SetScaleFactor(scale), m_webPage.pageID());
}

void CoordinatedCompositorHostProxy::setScrollPosition(const IntPoint& scrollPosition, float scale)
{
    m_webPage.send(Messages::CoordinatedCompositorHost::SetScrollPosition(scrollPosition, scale), m_webPage.pageID());
}

void CoordinatedCompositorHostProxy::setViewportSize(const IntSize& viewportSize, float scale)
{
    m_webPage.send(Messages::CoordinatedCompositorHost::SetViewportSize(viewportSize, scale), m_webPage.pageID());
}

void CoordinatedCompositorHostProxy::setDrawsBackground(bool drawsBackground)
{
    m_webPage.send(Messages::CoordinatedCompositorHost::SetDrawsBackground(drawsBackground), m_webPage.pageID());
}

void CoordinatedCompositorHostProxy::updateSceneState(const CoordinatedGraphicsState& state)
{
    m_webPage.send(Messages::CoordinatedCompositorHost::UpdateSceneState(state), m_webPage.pageID());
}

void CoordinatedCompositorHostProxy::releaseUpdateAtlases(Vector<uint32_t> atlasesToRemove)
{
    m_webPage.send(Messages::CoordinatedCompositorHost::ReleaseUpdateAtlases(atlasesToRemove), m_webPage.pageID());
}

void CoordinatedCompositorHostProxy::invalidate()
{
    m_webPage.send(Messages::CoordinatedCompositorHost::Invalidate(), m_webPage.pageID());
}

void CoordinatedCompositorHostProxy::forceRepaint()
{
    m_webPage.send(Messages::CoordinatedCompositorHost::ForceRepaint(), m_webPage.pageID());
}

void CoordinatedCompositorHostProxy::frameComplete()
{
    m_webPage.send(Messages::CoordinatedCompositorHost::FrameComplete(), m_webPage.pageID());
}

void CoordinatedCompositorHostProxy::setVisibleContentsRect(const FloatRect& rect, const FloatPoint& trajectoryVector)
{
    m_webPage.drawingArea()->layerTreeHost()->scrollNonCompositedContents(IntRect(rect));
}

void CoordinatedCompositorHostProxy::renderNextFrame()
{
    m_client.renderNextFrame();
}

void CoordinatedCompositorHostProxy::commitScrollOffset(uint32_t layerID, const IntSize& offset)
{
    m_client.commitScrollOffset(layerID, offset);
}

void CoordinatedCompositorHostProxy::detach(uint64_t stateID)
{
    LayerTreeHost* layerTreeHost = m_webPage.drawingArea()->layerTreeHost();
    layerTreeHost->setLayerFlushSchedulingEnabled(false);
    layerTreeHost->reset();
}

void CoordinatedCompositorHostProxy::attach(uint64_t stateID)
{
    LayerTreeHost* layerTreeHost = m_webPage.drawingArea()->layerTreeHost();
    layerTreeHost->setLayerFlushSchedulingEnabled(true);

    m_webPage.send(Messages::CoordinatedCompositorHost::DidAttach(stateID));
}

}
#endif // USE(COORDINATED_GRAPHICS_MULTIPROCESS)
