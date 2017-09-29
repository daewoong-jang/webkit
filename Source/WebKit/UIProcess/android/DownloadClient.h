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

#ifndef DownloadClient_h
#define DownloadClient_h

#include "APIDownloadClient.h"
#include "AWKDownloadDelegateNatives.h"
#include "AWKProcessPoolNatives.h"
#include <androidjni/GlobalRef.h>
#include <wtf/HashMap.h>

namespace WebKit {

class DownloadClient final : public API::DownloadClient {
public:
    DownloadClient(org::webkit::AWKProcessPool&, JNI::PassLocalRef<org::webkit::AWKDownloadDelegate>);

private:
    void didStart(WebProcessPool*, DownloadProxy*) override;
    void didReceiveResponse(WebProcessPool*, DownloadProxy*, const WebCore::ResourceResponse&) override;
    void didReceiveData(WebProcessPool*, DownloadProxy*, uint64_t) override;
    String decideDestinationWithSuggestedFilename(WebProcessPool*, DownloadProxy*, const String&, bool&) override;
    void didFinish(WebProcessPool*, DownloadProxy*) override;
    void didFail(WebProcessPool*, DownloadProxy*, const WebCore::ResourceError&) override;
    void didCancel(WebProcessPool*, DownloadProxy*) override;

private:
    org::webkit::AWKProcessPool& m_processPool;
    JNI::GlobalRef<org::webkit::AWKDownloadDelegate> m_delegate;
};

} // namespace WebKit

#endif // DownloadClient_h
