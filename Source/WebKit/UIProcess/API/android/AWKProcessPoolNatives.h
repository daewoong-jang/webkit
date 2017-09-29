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

#ifndef AWKProcessPool_h
#define AWKProcessPool_h

#include "AWKProcessPoolConfigurationNatives.h"
#include <WebKit/WKBase.h>
#include <androidjni/GlobalRef.h>
#include <org/webkit/Natives/AWKProcessPool.h>
#include <wtf/Ref.h>
#include <wtf/RefPtr.h>

namespace WebKit {
class WebProcessPool;
}

namespace org {
namespace webkit {

class AWKProcessPool : public Natives::AWKProcessPool {
    friend class Natives::AWKProcessPool;
public:
    static JNI::PassLocalRef<AWKProcessPool> create();
    static JNI::PassLocalRef<AWKProcessPool> create(Ref<WebKit::WebProcessPool>);
    ~AWKProcessPool();

    WebKit::WebProcessPool& processPool() { return *m_processPool; }

protected:
    void warmInitialProcess() override;

    void setDownloadDelegate(JNI::PassLocalRef<Natives::AWKDownloadDelegate>) override;

private:
    AWKProcessPool();
    AWKProcessPool(JNI::PassLocalRef<AWKProcessPoolConfiguration>);

    void initWithProcessPool(Ref<WebKit::WebProcessPool>);

    friend class InjectedBundleClient;
    WKTypeRef initializeInjectedBundleClient();
    WKTypeRef getInjectedBundleInitializationUserData();

    RefPtr<WebKit::WebProcessPool> m_processPool;
    JNI::GlobalRef<Natives::AWKDownloadDelegate> m_downloadDelegate;
    void* m_platformBundle;
};

} // namespace webkit
} // namespace org

#endif // AWKProcessPool_h
