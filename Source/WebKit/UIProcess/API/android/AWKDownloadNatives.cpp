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
#include "AWKDownloadNatives.h"

#include "AWKNativesCast.h"
#include "AWKWebContentNatives.h"
#include "DownloadProxy.h"

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKDownloadPrivate : public AWKDownload::Private {
public:
    JNI::GlobalRef<AWKWebContent> m_originatingWebContent;
    RefPtr<DownloadProxy> m_download;
};

template<> JNI::PassLocalRef<AWKDownload> AWKDownload::create(JNI::PassLocalRef<AWKWebContent> originatingWebContent, DownloadProxy* download)
{
    JNI::LocalRef<AWKDownload> ref = AWKDownload::create();
    ref->p<AWKDownloadPrivate>().m_originatingWebContent = originatingWebContent;
    ref->p<AWKDownloadPrivate>().m_download = download;
    return ref.release();
}

template<> DownloadProxy& AWKDownload::castTo()
{
    return *p<AWKDownloadPrivate>().m_download;
}

void AWKDownload::cancel()
{
    core(this).cancel();
}

JNI::PassLocalRef<AWKURLRequest> AWKDownload::getRequest()
{
    auto request = API::URLRequest::create(core(this).request());
    return AWKURLRequest::create(request.ptr());
}

JNI::PassLocalRef<AWKWebContent> AWKDownload::getOriginatingWebContent()
{
    return p<AWKDownloadPrivate>().m_originatingWebContent;
}

AWKDownload* AWKDownload::nativeCreate()
{
    std::unique_ptr<AWKDownload> ptr(new AWKDownload);
    ptr->m_private.reset(new AWKDownloadPrivate);
    return ptr.release();
}

} // namespace Natives
} // namespace webkit
} // namespace org
