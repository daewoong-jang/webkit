/*
 * Copyright (C) 2014 NAVER Corp. All rights reserved.
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
#include "AWKProcessPoolConfigurationNatives.h"

#include "APIProcessPoolConfiguration.h"
#include "AWKNativesCast.h"
#include "StringUtilities.h"
#include <java/util/Natives/Vector.h>
#include <WebCore/NotImplemented.h>
#include <wtf/MainThread.h>

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKProcessPoolConfigurationPrivate : public AWKProcessPoolConfiguration::Private {
public:
    RefPtr<API::ProcessPoolConfiguration> m_configuration;
};

template<> JNI::PassLocalRef<AWKProcessPoolConfiguration> AWKProcessPoolConfiguration::create(API::ProcessPoolConfiguration* configuration)
{
    JNI::LocalRef<AWKProcessPoolConfiguration> ref = AWKProcessPoolConfiguration::create();
    ref->p<AWKProcessPoolConfigurationPrivate>().m_configuration = configuration;
    return ref.release();
}

template<> API::ProcessPoolConfiguration& AWKProcessPoolConfiguration::castTo()
{
    return *p<AWKProcessPoolConfigurationPrivate>().m_configuration;
}

int32_t AWKProcessPoolConfiguration::getMaximumProcessCount()
{
    return core(this).maximumProcessCount();
}

void AWKProcessPoolConfiguration::setMaximumProcessCount(int64_t maximumProcessCount)
{
    core(this).setMaximumProcessCount(maximumProcessCount);
}

std::string AWKProcessPoolConfiguration::getInjectedBundlePath()
{
    return stdStringFromWebCoreString(core(this).injectedBundlePath());
}

void AWKProcessPoolConfiguration::setInjectedBundlePath(const std::string& injectedBundlePath)
{
    core(this).setInjectedBundlePath(String::fromUTF8(injectedBundlePath.c_str()));
}

JNI::PassLocalRef<Vector> AWKProcessPoolConfiguration::getCachePartitionedURLSchemes()
{
    notImplemented();
    return nullptr;
}

void AWKProcessPoolConfiguration::setCachePartitionedURLSchemes(JNI::PassLocalRef<Vector> cachePartitionedURLSchemes)
{
    notImplemented();
}

AWKProcessPoolConfiguration* AWKProcessPoolConfiguration::nativeCreate()
{
    WTF::initializeUIThread();
    std::unique_ptr<AWKProcessPoolConfiguration> ptr(new AWKProcessPoolConfiguration);
    ptr->m_private.reset(new AWKProcessPoolConfigurationPrivate);
    ptr->p<AWKProcessPoolConfigurationPrivate>().m_configuration = API::ProcessPoolConfiguration::create();
    return ptr.release();
}

}
} // namespace webkit
} // namespace org
