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
#include "AWKPreferencesNatives.h"

#include "StringUtilities.h"
#include "WebPreferences.h"
#include <wtf/text/WTFString.h>

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKPreferencesPrivate : public AWKPreferences::Private {
public:
    void setToDefaults();

    RefPtr<WebPreferences> m_preferences;
};

void AWKPreferencesPrivate::setToDefaults()
{
    m_preferences->setDeveloperExtrasEnabled(true);
    m_preferences->setFullScreenEnabled(true);
    m_preferences->setOfflineWebApplicationCacheEnabled(true);
    m_preferences->setHiddenPageDOMTimerThrottlingEnabled(true);
    m_preferences->setHiddenPageCSSAnimationSuspensionEnabled(true);
    m_preferences->setUsesPageCache(false);
    m_preferences->setPageCacheSupportsPlugins(false);
    m_preferences->setAcceleratedCompositingEnabled(true);
    m_preferences->setForceCompositingMode(true);

    char* debugVisualsEnvironment = getenv("WEBKIT_SHOW_COMPOSITING_DEBUG_VISUALS");
    bool showDebugVisuals = debugVisualsEnvironment && !strcmp(debugVisualsEnvironment, "1");
    m_preferences->setCompositingBordersVisible(showDebugVisuals);
    m_preferences->setCompositingRepaintCountersVisible(showDebugVisuals);
}

template<> JNI::PassLocalRef<AWKPreferences> AWKPreferences::create(WebPreferences* preferences)
{
    JNI::LocalRef<AWKPreferences> ref = AWKPreferences::create();
    ref->p<AWKPreferencesPrivate>().m_preferences = preferences;
    ref->p<AWKPreferencesPrivate>().setToDefaults();
    return ref.release();
}

template<> WebPreferences& AWKPreferences::castTo()
{
    return *p<AWKPreferencesPrivate>().m_preferences;
}

template<typename T> static T castValue(T value)
{
    return value;
}

static String castValue(const std::string& value)
{
    return String::fromUTF8(value.c_str(), value.length());
}

static std::string castValue(const String& value)
{
    return stdStringFromWebCoreString(value);
}

#define DEFINE_PREFERENCE_GETTER_SETTER_IMPL(getter, setter, preferenceGetter, preferenceSetter, type, returnType) \
    void AWKPreferences::setter(type preferenceGetter) \
    { \
        p<AWKPreferencesPrivate>().m_preferences->preferenceSetter(castValue(preferenceGetter)); \
    } \
    returnType AWKPreferences::getter() \
    { \
        return castValue(p<AWKPreferencesPrivate>().m_preferences->preferenceGetter()); \
    }

#define DEFINE_PREFERENCE_GETTER_SETTER(name, Name, type) DEFINE_PREFERENCE_GETTER_SETTER_IMPL(get##Name, set##Name, name, set##Name, type, type)

DEFINE_PREFERENCE_GETTER_SETTER(minimumFontSize, MinimumFontSize, int32_t);
DEFINE_PREFERENCE_GETTER_SETTER(javaScriptEnabled, JavaScriptEnabled, bool);
DEFINE_PREFERENCE_GETTER_SETTER(javaScriptCanOpenWindowsAutomatically, JavaScriptCanOpenWindowsAutomatically, bool);
DEFINE_PREFERENCE_GETTER_SETTER_IMPL(getTelephoneNumberDetectionIsEnabled, setTelephoneNumberDetectionIsEnabled, telephoneNumberParsingEnabled, setTelephoneNumberParsingEnabled, bool, bool);
DEFINE_PREFERENCE_GETTER_SETTER(storageBlockingPolicy, StorageBlockingPolicy, int32_t);
DEFINE_PREFERENCE_GETTER_SETTER(compositingBordersVisible, CompositingBordersVisible, bool);
DEFINE_PREFERENCE_GETTER_SETTER(compositingRepaintCountersVisible, CompositingRepaintCountersVisible, bool);
DEFINE_PREFERENCE_GETTER_SETTER(tiledScrollingIndicatorVisible, TiledScrollingIndicatorVisible, bool);
DEFINE_PREFERENCE_GETTER_SETTER(visibleDebugOverlayRegions, VisibleDebugOverlayRegions, int32_t);
DEFINE_PREFERENCE_GETTER_SETTER(simpleLineLayoutDebugBordersEnabled, SimpleLineLayoutDebugBordersEnabled, bool);
DEFINE_PREFERENCE_GETTER_SETTER(visualViewportEnabled, VisualViewportEnabled, bool);
DEFINE_PREFERENCE_GETTER_SETTER(largeImageAsyncDecodingEnabled, LargeImageAsyncDecodingEnabled, bool);
DEFINE_PREFERENCE_GETTER_SETTER(animatedImageAsyncDecodingEnabled, AnimatedImageAsyncDecodingEnabled, bool);
DEFINE_PREFERENCE_GETTER_SETTER(textAutosizingEnabled, TextAutosizingEnabled, bool);
DEFINE_PREFERENCE_GETTER_SETTER(developerExtrasEnabled, DeveloperExtrasEnabled, bool);
DEFINE_PREFERENCE_GETTER_SETTER(logsPageMessagesToSystemConsoleEnabled, LogsPageMessagesToSystemConsoleEnabled, bool);
DEFINE_PREFERENCE_GETTER_SETTER(hiddenPageDOMTimerThrottlingEnabled, HiddenPageDOMTimerThrottlingEnabled, bool);
DEFINE_PREFERENCE_GETTER_SETTER(hiddenPageDOMTimerThrottlingAutoIncreases, HiddenPageDOMTimerThrottlingAutoIncreases, bool);
DEFINE_PREFERENCE_GETTER_SETTER(pageVisibilityBasedProcessSuppressionEnabled, PageVisibilityBasedProcessSuppressionEnabled, bool);
DEFINE_PREFERENCE_GETTER_SETTER(allowFileAccessFromFileURLs, AllowFileAccessFromFileURLs, bool);
DEFINE_PREFERENCE_GETTER_SETTER(javaScriptRuntimeFlags, JavaScriptRuntimeFlags, int32_t);
DEFINE_PREFERENCE_GETTER_SETTER_IMPL(isStandalone, setStandalone, standalone, setStandalone, bool, bool);
DEFINE_PREFERENCE_GETTER_SETTER(diagnosticLoggingEnabled, DiagnosticLoggingEnabled, bool);
DEFINE_PREFERENCE_GETTER_SETTER(defaultFontSize, DefaultFontSize, int32_t);
DEFINE_PREFERENCE_GETTER_SETTER(defaultFixedFontSize, DefaultFixedFontSize, int32_t);
DEFINE_PREFERENCE_GETTER_SETTER_IMPL(getFixedFontFamily, setFixedFontFamily, fixedFontFamily, setFixedFontFamily, const std::string&, std::string);
DEFINE_PREFERENCE_GETTER_SETTER(offlineWebApplicationCacheEnabled, OfflineWebApplicationCacheEnabled, bool);
DEFINE_PREFERENCE_GETTER_SETTER(fullScreenEnabled, FullScreenEnabled, bool);

AWKPreferences* AWKPreferences::nativeCreate()
{
    std::unique_ptr<AWKPreferences> ptr(new AWKPreferences);
    ptr->m_private.reset(new AWKPreferencesPrivate);
    ptr->p<AWKPreferencesPrivate>().m_preferences = WebPreferences::create(String(), "WebKit", "WebKitDebug");
    ptr->p<AWKPreferencesPrivate>().setToDefaults();
    return ptr.release();
}

}
} // namespace webkit
} // namespace org
