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
#include "AWKUserScriptNatives.h"

#include "APIUserScript.h"
#include "AWKNativesCast.h"
#include "StringUtilities.h"

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKUserScriptPrivate : public AWKUserScript::Private {
public:
    RefPtr<API::UserScript> m_userScript;
};

template<> JNI::PassLocalRef<AWKUserScript> AWKUserScript::create(API::UserScript* userScript)
{
    JNI::LocalRef<AWKUserScript> ref = AWKUserScript::create();
    ref->p<AWKUserScriptPrivate>().m_userScript = userScript;
    return ref.release();
}

template<> API::UserScript& AWKUserScript::castTo()
{
    return *p<AWKUserScriptPrivate>().m_userScript;
}

template<> const WebCore::UserScript& AWKUserScript::castTo()
{
    return core(this).userScript();
}

std::string AWKUserScript::getSource()
{
    return stdStringFromWebCoreString(core(this).userScript().source());
}

int32_t AWKUserScript::getInjectionTime()
{
    return core(this).userScript().injectionTime();
}

bool AWKUserScript::isForMainFrameOnly()
{
    return core(this).userScript().injectedFrames() == WebCore::InjectInTopFrameOnly;
}

static WebCore::UserScriptInjectionTime toWebCoreUserScriptInjectionTime(int32_t injectionTime)
{
    switch (injectionTime) {
    case AWKUserScript::INJECTION_TIME_AT_DOCUMENT_START:
        return WebCore::InjectAtDocumentStart;
    case AWKUserScript::INJECTION_TIME_AT_DOCUMENT_END:
        return WebCore::InjectAtDocumentEnd;
    }
    ASSERT_NOT_REACHED();
    return WebCore::InjectAtDocumentEnd;
}

AWKUserScript* AWKUserScript::nativeCreate()
{
    std::unique_ptr<AWKUserScript> ptr(new AWKUserScript);
    ptr->m_private.reset(new AWKUserScriptPrivate);
    return ptr.release();
}

AWKUserScript* AWKUserScript::nativeCreateWithSource(std::string source, int32_t injectionTime, bool forMainFrameOnly)
{
    std::unique_ptr<AWKUserScript> ptr(new AWKUserScript);
    ptr->m_private.reset(new AWKUserScriptPrivate);
    ptr->p<AWKUserScriptPrivate>().m_userScript = API::UserScript::create(WebCore::UserScript(String::fromUTF8(source.c_str(), source.length()), API::UserScript::generateUniqueURL(),
            WTF::Vector<String>(), WTF::Vector<String>(), toWebCoreUserScriptInjectionTime(injectionTime),
            forMainFrameOnly ? WebCore::InjectInTopFrameOnly : WebCore::InjectInAllFrames), API::UserContentWorld::normalWorld());
    return ptr.release();
}

} // namespace Natives
} // namespace webkit
} // namespace org

