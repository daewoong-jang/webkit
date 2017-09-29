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
#include "AWKProcessPoolNatives.h"

#include "APIInjectedBundleClient.h"
#include "AWKNativesCast.h"
#include "DownloadClient.h"
#include "WKAPICast.h"
#include "WebProcessPool.h"
#include <WebCore/FileSystem.h>
#include <cutils/log.h>
#include <dlfcn.h>

using namespace WebKit;

namespace org {
namespace webkit {

class InjectedBundleClient : public API::InjectedBundleClient {
public:
    InjectedBundleClient(AWKProcessPool&);
    RefPtr<API::Object> getInjectedBundleInitializationUserData(WebProcessPool&) override;

private:
    AWKProcessPool& m_processPool;
};

InjectedBundleClient::InjectedBundleClient(AWKProcessPool& processPool)
    : m_processPool(processPool)
{
}

RefPtr<API::Object> InjectedBundleClient::getInjectedBundleInitializationUserData(WebProcessPool&)
{
    return toImpl(m_processPool.getInjectedBundleInitializationUserData());
}

JNI::PassLocalRef<AWKProcessPool> AWKProcessPool::create()
{
    return Natives::AWKProcessPool::create().as<AWKProcessPool>();
}

JNI::PassLocalRef<AWKProcessPool> AWKProcessPool::create(Ref<WebProcessPool> webProcessPool)
{
    JNI::LocalRef<AWKProcessPool> processPool = AWKProcessPool::create();
    processPool->initWithProcessPool(WTFMove(webProcessPool));
    return processPool;
}

AWKProcessPool::AWKProcessPool()
    : m_platformBundle(nullptr)
{
}

AWKProcessPool::AWKProcessPool(JNI::PassLocalRef<AWKProcessPoolConfiguration> configuration)
    : m_platformBundle(nullptr)
{
    initWithProcessPool(WebProcessPool::create(core(configuration)));
}

AWKProcessPool::~AWKProcessPool()
{
}

void AWKProcessPool::initWithProcessPool(Ref<WebProcessPool> context)
{
    m_processPool = WTFMove(context);
    m_processPool->setInjectedBundleClient(std::make_unique<InjectedBundleClient>(*this));
}

extern "C" typedef WKTypeRef (*AWKContextInitializeInjectedBundleClientFunctionPtr)(WKContextRef);

WKTypeRef AWKProcessPool::initializeInjectedBundleClient()
{
    if (m_platformBundle || processPool().injectedBundlePath().length() == 0)
        return nullptr;

    const char *error;

    m_platformBundle = dlopen(WebCore::fileSystemRepresentation(processPool().injectedBundlePath()).data(), RTLD_LOCAL);
    if (!m_platformBundle) {
        ALOGE("InjectedBundle dlopen error [%s]", dlerror());
        return nullptr;
    }

    AWKContextInitializeInjectedBundleClientFunctionPtr initializeFunction = 0;
    initializeFunction = reinterpret_cast<AWKContextInitializeInjectedBundleClientFunctionPtr>(dlsym(m_platformBundle, "AWKContextInitializeInjectedBundleClient"));

    if (!initializeFunction && (error = dlerror()) != NULL)  {
        ALOGE("InjectedBundle dlsym error [%s]", error);
        return nullptr;
    }

    processPool().configuration().setFullySynchronousModeIsAllowedForTesting(true);

    return initializeFunction(toAPI(&processPool()));
}

WKTypeRef AWKProcessPool::getInjectedBundleInitializationUserData()
{
    return initializeInjectedBundleClient();
}

void AWKProcessPool::warmInitialProcess()
{
    processPool().warmInitialProcess();
}

void AWKProcessPool::setDownloadDelegate(JNI::PassLocalRef<Natives::AWKDownloadDelegate> downloadDelegate)
{
    m_downloadDelegate = downloadDelegate;
    processPool().setDownloadClient(std::make_unique<DownloadClient>(*this, downloadDelegate));
}

namespace Natives {

template<> WebProcessPool& AWKProcessPool::castTo()
{
    return *static_cast<org::webkit::AWKProcessPool*>(this)->m_processPool;
}

AWKProcessPool* AWKProcessPool::nativeCreate()
{
    return new org::webkit::AWKProcessPool();
}

AWKProcessPool* AWKProcessPool::nativeCreateWithConfiguration(JNI::PassLocalRef<AWKProcessPoolConfiguration> configuration)
{
    return new org::webkit::AWKProcessPool(configuration.as<org::webkit::AWKProcessPoolConfiguration>());
}

}
} // namespace webkit
} // namespace org
