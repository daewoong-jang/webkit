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
#include "AWKWebContentConfigurationNatives.h"

#include "AWKPreferencesNatives.h"
#include "AWKProcessPoolNatives.h"
#include "AWKUserContentControllerNatives.h"
#include "AWKWebsiteDataStoreNatives.h"
#include <androidjni/GlobalRef.h>

namespace org {
namespace webkit {
namespace Natives {

template<typename T>
class LazyInitializedGlobalRef {
public:
    template<typename F>
    T* get(F&& f)
    {
        if (!m_isInitialized) {
            m_value = f();
            m_isInitialized = true;
        }

        return m_value.getPtr();
    }

    void set(JNI::PassLocalRef<T> t)
    {
        m_value = t;
        m_isInitialized = true;
    }

    T* peek()
    {
        return m_value.getPtr();
    }

private:
    bool m_isInitialized = false;
    JNI::GlobalRef<T> m_value;
};

class AWKWebContentConfigurationPrivate : public AWKWebContentConfiguration::Private {
public:
    std::string m_applicationNameForUserAgent { "" };
    LazyInitializedGlobalRef<AWKPreferences> m_preferences;
    LazyInitializedGlobalRef<AWKProcessPool> m_processPool;
    LazyInitializedGlobalRef<AWKUserContentController> m_userContentController;
    LazyInitializedGlobalRef<AWKWebsiteDataStore> m_websiteDataStore;
    bool m_ignoresViewportScaleLimits { false };
    bool m_suppressesIncrementalRendering { false };
    bool m_allowsInlineMediaPlayback { false };
    int32_t m_selectionGranularity { 0 };
    int32_t m_userInterfaceDirectionPolicy { 0 };
    int32_t m_dataDetectorTypes { 0 };
};

void AWKWebContentConfiguration::setApplicationNameForUserAgent(const std::string& applicationNameForUserAgent)
{
    p<AWKWebContentConfigurationPrivate>().m_applicationNameForUserAgent = applicationNameForUserAgent;
}

std::string AWKWebContentConfiguration::getApplicationNameForUserAgent()
{
    return p<AWKWebContentConfigurationPrivate>().m_applicationNameForUserAgent;
}

void AWKWebContentConfiguration::setPreferences(JNI::PassLocalRef<AWKPreferences> preferences)
{
    p<AWKWebContentConfigurationPrivate>().m_preferences.set(preferences);
}

JNI::PassLocalRef<AWKPreferences> AWKWebContentConfiguration::getPreferences()
{
    return p<AWKWebContentConfigurationPrivate>().m_preferences.get([] { return AWKPreferences::create(); });
}

void AWKWebContentConfiguration::setProcessPool(JNI::PassLocalRef<AWKProcessPool> processPool)
{
    p<AWKWebContentConfigurationPrivate>().m_processPool.set(processPool);
}

JNI::PassLocalRef<AWKProcessPool> AWKWebContentConfiguration::getProcessPool()
{
    return p<AWKWebContentConfigurationPrivate>().m_processPool.get([] { return AWKProcessPool::create(); });
}

void AWKWebContentConfiguration::setUserContentController(JNI::PassLocalRef<AWKUserContentController> userContentController)
{
    p<AWKWebContentConfigurationPrivate>().m_userContentController.set(userContentController);
}

JNI::PassLocalRef<AWKUserContentController> AWKWebContentConfiguration::getUserContentController()
{
    return p<AWKWebContentConfigurationPrivate>().m_userContentController.get([] { return AWKUserContentController::create(); });
}

void AWKWebContentConfiguration::setWebsiteDataStore(JNI::PassLocalRef<AWKWebsiteDataStore> websiteDataStore)
{
    p<AWKWebContentConfigurationPrivate>().m_websiteDataStore.set(websiteDataStore);
}

JNI::PassLocalRef<AWKWebsiteDataStore> AWKWebContentConfiguration::getWebsiteDataStore()
{
    return p<AWKWebContentConfigurationPrivate>().m_websiteDataStore.get([] { return AWKWebsiteDataStore::getDefaultDataStore(); });
}

void AWKWebContentConfiguration::setIgnoresViewportScaleLimits(bool ignoresViewportScaleLimits)
{
    p<AWKWebContentConfigurationPrivate>().m_ignoresViewportScaleLimits = ignoresViewportScaleLimits;
}

bool AWKWebContentConfiguration::getIgnoresViewportScaleLimits()
{
    return p<AWKWebContentConfigurationPrivate>().m_ignoresViewportScaleLimits;
}

void AWKWebContentConfiguration::setSuppressesIncrementalRendering(bool suppressesIncrementalRendering)
{
    p<AWKWebContentConfigurationPrivate>().m_suppressesIncrementalRendering = suppressesIncrementalRendering;
}

bool AWKWebContentConfiguration::getSuppressesIncrementalRendering()
{
    return p<AWKWebContentConfigurationPrivate>().m_suppressesIncrementalRendering;
}

void AWKWebContentConfiguration::setAllowsInlineMediaPlayback(bool allowsInlineMediaPlayback)
{
    p<AWKWebContentConfigurationPrivate>().m_allowsInlineMediaPlayback = allowsInlineMediaPlayback;
}

bool AWKWebContentConfiguration::getAllowsInlineMediaPlayback()
{
    return p<AWKWebContentConfigurationPrivate>().m_allowsInlineMediaPlayback;
}

void AWKWebContentConfiguration::setSelectionGranularity(int32_t selectionGranularity)
{
    p<AWKWebContentConfigurationPrivate>().m_selectionGranularity = selectionGranularity;
}

int32_t AWKWebContentConfiguration::getSelectionGranularity()
{
    return p<AWKWebContentConfigurationPrivate>().m_selectionGranularity;
}

void AWKWebContentConfiguration::setUserInterfaceDirectionPolicy(int32_t userInterfaceDirectionPolicy)
{
    p<AWKWebContentConfigurationPrivate>().m_userInterfaceDirectionPolicy = userInterfaceDirectionPolicy;
}

int32_t AWKWebContentConfiguration::getUserInterfaceDirectionPolicy()
{
    return p<AWKWebContentConfigurationPrivate>().m_userInterfaceDirectionPolicy;
}

void AWKWebContentConfiguration::setDataDetectorTypes(int32_t dataDetectorTypes)
{
    p<AWKWebContentConfigurationPrivate>().m_dataDetectorTypes = dataDetectorTypes;
}

int32_t AWKWebContentConfiguration::getDataDetectorTypes()
{
    return p<AWKWebContentConfigurationPrivate>().m_dataDetectorTypes;
}

AWKWebContentConfiguration* AWKWebContentConfiguration::nativeCreate()
{
    std::unique_ptr<AWKWebContentConfiguration> ptr(new AWKWebContentConfiguration);
    ptr->m_private.reset(new AWKWebContentConfigurationPrivate);
    return ptr.release();
}

}
} // namespace webkit
} // namespace org
