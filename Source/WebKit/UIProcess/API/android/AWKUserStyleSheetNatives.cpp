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
#include "AWKUserStyleSheetNatives.h"

#include "APIUserStyleSheet.h"
#include "AWKNativesCast.h"
#include "StringUtilities.h"
#include <java/util/Natives/Vector.h>
#include <wtf/Vector.h>

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKUserStyleSheetPrivate : public AWKUserStyleSheet::Private {
public:
    RefPtr<API::UserStyleSheet> m_userStyleSheet;
};

template<> JNI::PassLocalRef<AWKUserStyleSheet> AWKUserStyleSheet::create(API::UserStyleSheet* userStyleSheet)
{
    JNI::LocalRef<AWKUserStyleSheet> ref = AWKUserStyleSheet::create();
    ref->p<AWKUserStyleSheetPrivate>().m_userStyleSheet = userStyleSheet;
    return ref.release();
}

template<> API::UserStyleSheet& AWKUserStyleSheet::castTo()
{
    return *p<AWKUserStyleSheetPrivate>().m_userStyleSheet;
}

template<> const WebCore::UserStyleSheet& AWKUserStyleSheet::castTo()
{
    return core(this).userStyleSheet();
}

std::string AWKUserStyleSheet::getSource()
{
    return stdStringFromWebCoreString(core(this).userStyleSheet().source());
}

JNI::PassLocalRef<AWKURL> AWKUserStyleSheet::getBaseURL()
{
    return JNI::wrap<AWKURL>(core(this).userStyleSheet().url());
}

bool AWKUserStyleSheet::isForMainFrameOnly()
{
    return core(this).userStyleSheet().injectedFrames() == WebCore::InjectInTopFrameOnly;
}

AWKUserStyleSheet* AWKUserStyleSheet::nativeCreate()
{
    std::unique_ptr<AWKUserStyleSheet> ptr(new AWKUserStyleSheet);
    ptr->m_private.reset(new AWKUserStyleSheetPrivate);
    return ptr.release();
}

AWKUserStyleSheet* AWKUserStyleSheet::nativeCreateWithSource(std::string source, bool forMainFrameOnly)
{
    std::unique_ptr<AWKUserStyleSheet> ptr(new AWKUserStyleSheet);
    ptr->m_private.reset(new AWKUserStyleSheetPrivate);
    ptr->p<AWKUserStyleSheetPrivate>().m_userStyleSheet = API::UserStyleSheet::create(WebCore::UserStyleSheet(String::fromUTF8(source.c_str(), source.length()), API::UserStyleSheet::generateUniqueURL(),
            WTF::Vector<String>(), WTF::Vector<String>(),
            forMainFrameOnly ? WebCore::InjectInTopFrameOnly : WebCore::InjectInAllFrames, WebCore::UserStyleUserLevel), API::UserContentWorld::normalWorld());
    return ptr.release();
}

AWKUserStyleSheet* AWKUserStyleSheet::nativeCreateWithLists(std::string source, bool forMainFrameOnly, JNI::PassLocalRef<Vector> whitelist, JNI::PassLocalRef<Vector> blacklist)
{
    std::unique_ptr<AWKUserStyleSheet> ptr(new AWKUserStyleSheet);
    ptr->m_private.reset(new AWKUserStyleSheetPrivate);
    ptr->p<AWKUserStyleSheetPrivate>().m_userStyleSheet = API::UserStyleSheet::create(WebCore::UserStyleSheet(String::fromUTF8(source.c_str(), source.length()), API::UserStyleSheet::generateUniqueURL(),
            WTF::Vector<String>(), WTF::Vector<String>(),
            forMainFrameOnly ? WebCore::InjectInTopFrameOnly : WebCore::InjectInAllFrames, WebCore::UserStyleUserLevel), API::UserContentWorld::normalWorld());
    return ptr.release();
}

AWKUserStyleSheet* AWKUserStyleSheet::nativeCreateWithBaseURL(std::string source, bool forMainFrameOnly, JNI::PassLocalRef<Vector> whitelist, JNI::PassLocalRef<Vector> blacklist, JNI::PassLocalRef<AWKURL> baseURL)
{
    std::unique_ptr<AWKUserStyleSheet> ptr(new AWKUserStyleSheet);
    ptr->m_private.reset(new AWKUserStyleSheetPrivate);
    ptr->p<AWKUserStyleSheetPrivate>().m_userStyleSheet = API::UserStyleSheet::create(WebCore::UserStyleSheet(String::fromUTF8(source.c_str(), source.length()), core(baseURL),
            WTF::Vector<String>(), WTF::Vector<String>(),
            forMainFrameOnly ? WebCore::InjectInTopFrameOnly : WebCore::InjectInAllFrames, WebCore::UserStyleUserLevel), API::UserContentWorld::normalWorld());
    return ptr.release();
}

} // namespace Natives
} // namespace webkit
} // namespace org

