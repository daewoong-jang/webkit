/*
 * Copyright (C) 2013 Apple Inc.  All rights reserved.
 * Copyright (C) 2017 Sony Interactive Entertainment Inc.
 * Copyright (C) 2017 NAVER Corp.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#pragma once

#include "CurlJobManager.h"
#include "FileSystem.h"
#include "ResourceHandle.h"
#include "ResourceRequest.h"
#include "ResourceResponse.h"
#include <wtf/Lock.h>
#include <wtf/Threading.h>

namespace WebCore {

class CurlDownloadListener {
public:
    virtual void didReceiveResponse() { }
    virtual void didReceiveDataOfLength(int) { }
    virtual void didFinish() { }
    virtual void didFail() { }
};

class CurlDownload : public ThreadSafeRefCounted<CurlDownload>, public CurlJobClient {
public:
    CurlDownload();
    ~CurlDownload();

    void init(CurlDownloadListener*, const URL&);
    void init(CurlDownloadListener*, ResourceHandle*, const ResourceRequest&, const ResourceResponse&);

    void setListener(CurlDownloadListener* listener) { m_listener = listener; }

    bool start();
    bool cancel();

    ResourceResponse getResponse() const;

    bool deletesFileUponFailure() const { return m_deletesFileUponFailure; }
    void setDeletesFileUponFailure(bool deletesFileUponFailure) { m_deletesFileUponFailure = deletesFileUponFailure; }

    void setDestination(const String& destination) { m_destination = destination; }

private:
    void retain() override;
    void release() override;

    CURL* handle() override { return m_curlHandle ? m_curlHandle->handle() : nullptr; }
    CURL* setupTransfer() override;
    void didCompleteTransfer(CURLcode) override;
    void didCancelTransfer() override;

    void closeFile();
    void moveFileToDestination();
    void writeDataToFile(const char* data, int size);

    // Called on download thread.
    void didReceiveHeader(const String& header);
    void didReceiveData(void* data, int size);

    // Called on main thread.
    void didReceiveResponse();
    void didReceiveDataOfLength(int size);
    void didFinish();
    void didFail();

    static size_t writeCallback(char* ptr, size_t, size_t nmemb, void* data);
    static size_t headerCallback(char* ptr, size_t, size_t nmemb, void* data);

    std::unique_ptr<CurlHandle> m_curlHandle;
    ResourceRequest m_request;
    ResourceResponse m_response;

    String m_tempPath;
    String m_destination;
    PlatformFileHandle m_tempHandle { invalidPlatformFileHandle };
    bool m_deletesFileUponFailure { false };
    mutable Lock m_mutex;
    CurlDownloadListener* m_listener { nullptr };
};

}
