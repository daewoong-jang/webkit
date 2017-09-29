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
#include "DownloadClient.h"

#include "DownloadProxy.h"
#include <WebCore/HTTPHeaderNames.h>
#include <WebCore/NotImplemented.h>
#include <WebCore/ResourceError.h>
#include <WebCore/ResourceResponse.h>

using namespace WebCore;
using namespace org::webkit;

namespace WebKit {

DownloadClient::DownloadClient(AWKProcessPool& processPool, JNI::PassLocalRef<AWKDownloadDelegate> delegate)
    : m_processPool(processPool)
    , m_delegate(delegate)
{
}

void DownloadClient::didStart(WebProcessPool*, DownloadProxy*)
{
    notImplemented();
}

void DownloadClient::didReceiveResponse(WebProcessPool*, DownloadProxy* downloadProxy, const ResourceResponse& resourceResponse)
{
    notImplemented();
}

void DownloadClient::didReceiveData(WebProcessPool*, DownloadProxy* downloadProxy, uint64_t length)
{
    notImplemented();
}

String DownloadClient::decideDestinationWithSuggestedFilename(WebProcessPool*, DownloadProxy* downloadProxy, const String& defaultFileName, bool& allowOverwrite)
{
    notImplemented();
    return String();
}

void DownloadClient::didFinish(WebProcessPool*, DownloadProxy* downloadProxy)
{
    notImplemented();
}

void DownloadClient::didFail(WebProcessPool*, DownloadProxy* downloadProxy, const ResourceError& error)
{
    notImplemented();
}

void DownloadClient::didCancel(WebProcessPool*, DownloadProxy* downloadProxy)
{
    notImplemented();
}

}
