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
#include "AWKWebsiteDataStoreNatives.h"

#include "APIWebsiteDataStore.h"
#include "AWKCallbackNatives.h"
#include <WebCore/NotImplemented.h>
#include <java/util/Natives/Vector.h>

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKWebsiteDataStorePrivate : public AWKWebsiteDataStore::Private {
public:
    RefPtr<API::WebsiteDataStore> m_websiteDataStore;
};

template<> JNI::PassLocalRef<AWKWebsiteDataStore> AWKWebsiteDataStore::create(API::WebsiteDataStore* websiteDataStore)
{
    JNI::LocalRef<AWKWebsiteDataStore> ref = AWKWebsiteDataStore::create();
    ref->p<AWKWebsiteDataStorePrivate>().m_websiteDataStore = websiteDataStore;
    return ref.release();
}

template<> API::WebsiteDataStore& AWKWebsiteDataStore::castTo()
{
    return *p<AWKWebsiteDataStorePrivate>().m_websiteDataStore;
}

JNI::PassLocalRef<AWKWebsiteDataStore> AWKWebsiteDataStore::getDefaultDataStore()
{
    return JNI::wrap<AWKWebsiteDataStore>(API::WebsiteDataStore::defaultDataStore().ptr());
}

JNI::PassLocalRef<AWKWebsiteDataStore> AWKWebsiteDataStore::createNonPersistentDataStore()
{
    return JNI::wrap<AWKWebsiteDataStore>(API::WebsiteDataStore::createNonPersistentDataStore().ptr());
}

JNI::PassLocalRef<Vector> AWKWebsiteDataStore::getAllWebsiteDataTypes()
{
    notImplemented();
    return nullptr;
}

void AWKWebsiteDataStore::fetchDataRecordsOfTypes(JNI::PassLocalRef<Vector> dataTypes, JNI::PassLocalRef<AWKCallback> completionHandler)
{
    notImplemented();
}

void AWKWebsiteDataStore::removeDataOfTypes(JNI::PassLocalRef<Vector> dataTypes, JNI::PassLocalRef<Vector> dataRecords, JNI::PassLocalRef<AWKCallback> completionHandler)
{
    notImplemented();
}

void AWKWebsiteDataStore::removeDataOfTypes(JNI::PassLocalRef<Vector> dataTypes, double modifiedSince, JNI::PassLocalRef<AWKCallback> completionHandler)
{
    notImplemented();
}

AWKWebsiteDataStore* AWKWebsiteDataStore::nativeCreate()
{
    std::unique_ptr<AWKWebsiteDataStore> ptr(new AWKWebsiteDataStore);
    ptr->m_private.reset(new AWKWebsiteDataStorePrivate);
    return ptr.release();
}

} // namespace Natives
} // namespace webkit
} // namespace org
