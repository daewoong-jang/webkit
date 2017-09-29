/*
 * Copyright (C) 2014-2015 NAVER Corp. All rights reserved.
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
#include "AWKWindowFeaturesNatives.h"

#include "APIWindowFeatures.h"
#include "AWKNativesCast.h"

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKWindowFeaturesPrivate : public AWKWindowFeatures::Private {
public:
    RefPtr<API::WindowFeatures> m_windowFeatures;
};

template<> JNI::PassLocalRef<AWKWindowFeatures> AWKWindowFeatures::create(const WebCore::WindowFeatures& windowFeatures)
{
    JNI::LocalRef<AWKWindowFeatures> ref = AWKWindowFeatures::create();
    ref->p<AWKWindowFeaturesPrivate>().m_windowFeatures = API::WindowFeatures::create(windowFeatures);
    return ref.release();
}

template<> API::WindowFeatures& AWKWindowFeatures::castTo()
{
    return *p<AWKWindowFeaturesPrivate>().m_windowFeatures;
}

template<> const WebCore::WindowFeatures& AWKWindowFeatures::castTo()
{
    return core(this).windowFeatures();
}

bool AWKWindowFeatures::getMenuBarVisibility()
{
    return core(this).windowFeatures().menuBarVisible;
}

bool AWKWindowFeatures::getStatusBarVisibility()
{
    return core(this).windowFeatures().statusBarVisible;
}

bool AWKWindowFeatures::getToolBarsVisibility()
{
    return core(this).windowFeatures().toolBarVisible;
}

bool AWKWindowFeatures::getAllowsResizing()
{
    return core(this).windowFeatures().resizable;
}

int32_t AWKWindowFeatures::getX()
{
    if (auto x = core(this).windowFeatures().x)
        return *x;

    return -1;
}

int32_t AWKWindowFeatures::getY()
{
    if (auto y = core(this).windowFeatures().y)
        return *y;

    return -1;
}

int32_t AWKWindowFeatures::getWidth()
{
    if (auto width = core(this).windowFeatures().width)
        return *width;

    return -1;
}

int32_t AWKWindowFeatures::getHeight()
{
    if (auto height = core(this).windowFeatures().height)
        return *height;

    return -1;
}

AWKWindowFeatures* AWKWindowFeatures::nativeCreate()
{
    std::unique_ptr<AWKWindowFeatures> ptr(new AWKWindowFeatures);
    ptr->m_private.reset(new AWKWindowFeaturesPrivate);
    return ptr.release();
}

}
} // namespace webkit
} // namespace org
