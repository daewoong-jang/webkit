/*
 * Copyright (C) 2017 NAVER Corp. All rights reserved.
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

#ifndef GLContextEGLAndroid_h
#define GLContextEGLAndroid_h

#include "GLContext.h"
#include <EGL/egl.h>

namespace WebCore {

class GLContextEGLAndroid final : public GLContext {
    WTF_MAKE_NONCOPYABLE(GLContextEGLAndroid);
public:
    static std::unique_ptr<GLContextEGLAndroid> create(PlatformDisplay&);
    virtual ~GLContextEGLAndroid();

    static bool isContextLost();
    static void setContextLost(bool);

    bool makeContextCurrent() override;
    void swapBuffers() override { }
    void waitNative() override { }
    bool canRenderToDefaultFramebuffer() override { return false; }
    IntSize defaultFrameBufferSize() override { return IntSize(); }
    void swapInterval(int) override { }

    bool isEGLContext() const override { return true; }

    cairo_device_t* cairoDevice() override { return 0; }
    PlatformGraphicsContext3D platformContext() override { return m_context; }

private:
    static bool initialize();

    GLContextEGLAndroid(PlatformDisplay&);

    EGLDisplay m_display;
    EGLSurface m_surface;
    EGLContext m_context;
};

} // namespace WebCore

#endif // GLContextEGLAndroid_h
