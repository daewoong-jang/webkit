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
#include "AWKDownloadDelegate.h"

namespace org {
namespace webkit {
namespace Managed {

void AWKDownloadDelegate::didReceiveResponse(const std::shared_ptr<AWKDownload>& download, const std::shared_ptr<AWKURLResponse>& response)
{
}

void AWKDownloadDelegate::didReceiveData(const std::shared_ptr<AWKDownload>& download, int64_t length)
{
}

std::string AWKDownloadDelegate::decideDestinationWithSuggestedFilename(const std::shared_ptr<AWKDownload>& download, const std::string& filename)
{
    return "";
}

void AWKDownloadDelegate::didFinish(const std::shared_ptr<AWKDownload>& download)
{
}

void AWKDownloadDelegate::didFail(const std::shared_ptr<AWKDownload>& download, const std::shared_ptr<AWKError>& error)
{
}

void AWKDownloadDelegate::didCancel(const std::shared_ptr<AWKDownload>& download)
{
}

void AWKDownloadDelegate::INIT()
{
}

} // namespace Managed
} // namespace webkit
} // namespace org
