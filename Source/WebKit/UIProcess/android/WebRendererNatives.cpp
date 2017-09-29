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
#include "WebRendererNatives.h"

#include "AWKViewNatives.h"
#include <WebCore/GLContextEGLAndroid.h>
#include <androidjni/GlobalRef.h>
#include <wtf/NeverDestroyed.h>

using namespace WebCore;
using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class WebRendererPrivate : public WebRenderer::Private {
public:
    JNI::GlobalRef<AWKView> m_view;
};

static org::webkit::AWKView& view(WebRenderer::Private& self)
{
    return *static_cast<WebRendererPrivate&>(self).m_view.getPtr<org::webkit::AWKView>();
}

class GLThreadData {
public:
    static GLThreadData& singleton();

    void createContext();
    void makeContextCurrent();

private:
    std::unique_ptr<GLContext> m_context;
};

GLThreadData& GLThreadData::singleton()
{
    static NeverDestroyed<GLThreadData> data;
    return data;
}

void GLThreadData::createContext()
{
    m_context = GLContextEGLAndroid::create(PlatformDisplay::sharedDisplay());
}

void GLThreadData::makeContextCurrent()
{
    m_context->makeContextCurrent();
}

void WebRenderer::surfaceCreated()
{
    GLThreadData::singleton().createContext();
}

void WebRenderer::paint()
{
    GLThreadData::singleton().makeContextCurrent();
    view(*m_private).paintToCurrentGLContext();
}

void WebRenderer::contextLost()
{
    view(*m_private).contextLostGL();
}

void WebRenderer::stop()
{
    view(*m_private).stopGL();
}

WebRenderer* WebRenderer::nativeCreate(JNI::PassLocalRef<AWKView> view)
{
    std::unique_ptr<WebRenderer> ptr(new WebRenderer);
    ptr->m_private.reset(new WebRendererPrivate);
    ptr->p<WebRendererPrivate>().m_view = view;
    return ptr.release();
}

} // namespace Natives
} // namespace webkit
} // namespace org
