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
#include "AWKURLNatives.h"

#include "AWKSharedNativesCast.h"
#include "StringUtilities.h"
#include <WebCore/URL.h>
#include <wtf/text/CString.h>

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKURLPrivate : public AWKURL::Private {
public:
    AWKURLPrivate(WebCore::URL&& url)
        : m_url(url) { }

    WebCore::URL m_url;
};

template<> JNI::PassLocalRef<AWKURL> AWKURL::create(const WebCore::URL& url)
{
    JNI::LocalRef<AWKURL> ref = AWKURL::create();
    ref->p<AWKURLPrivate>().m_url = url;
    return ref.release();
}

template<> WebCore::URL& AWKURL::castTo()
{
    return p<AWKURLPrivate>().m_url;
}

std::string AWKURL::getAbsoluteString()
{
    return stdStringFromWebCoreString(p<AWKURLPrivate>().m_url.string());
}

std::string AWKURL::getProtocol()
{
    return stdStringFromWebCoreString(p<AWKURLPrivate>().m_url.protocol().toStringWithoutCopying());
}

std::string AWKURL::getHost()
{
    return stdStringFromWebCoreString(p<AWKURLPrivate>().m_url.host());
}

bool AWKURL::isFileURL()
{
    return core(this).isLocalFile();
}

AWKURL* AWKURL::nativeCreate()
{
    std::unique_ptr<AWKURL> ptr(new AWKURL);
    ptr->m_private.reset(new AWKURLPrivate(WebCore::URL()));
    return ptr.release();
}

AWKURL* AWKURL::nativeCreateWithString(std::string absoluteString)
{
    std::unique_ptr<AWKURL> ptr(new AWKURL);
    ptr->m_private.reset(new AWKURLPrivate(WebCore::URL(WebCore::URL(), String::fromUTF8(absoluteString.c_str()))));
    return ptr.release();
}

}
} // namespace webkit
} // namespace org
