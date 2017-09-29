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
#include "LegacyCustomProtocolManager.h"

#include "ChildProcess.h"
#include "LegacyCustomProtocolManagerMessages.h"
#include "NetworkProcessCreationParameters.h"
#include "WebProcessCreationParameters.h"
#include <WebCore/NotImplemented.h>

namespace WebKit {

void LegacyCustomProtocolManager::registerProtocolClass()
{
    notImplemented();
}

void LegacyCustomProtocolManager::registerScheme(const String& scheme)
{
    notImplemented();
}

void LegacyCustomProtocolManager::unregisterScheme(const String&)
{
    notImplemented();
}

bool LegacyCustomProtocolManager::supportsScheme(const String& scheme)
{
    notImplemented();
    return false;
}

void LegacyCustomProtocolManager::didFailWithError(uint64_t customProtocolID, const WebCore::ResourceError& error)
{
    notImplemented();
}

void LegacyCustomProtocolManager::didLoadData(uint64_t customProtocolID, const IPC::DataReference& dataReference)
{
    notImplemented();
}

void LegacyCustomProtocolManager::didReceiveResponse(uint64_t customProtocolID, const WebCore::ResourceResponse& response, uint32_t)
{
    notImplemented();
}

void LegacyCustomProtocolManager::didFinishLoading(uint64_t customProtocolID)
{
    notImplemented();
}

void LegacyCustomProtocolManager::wasRedirectedToRequest(uint64_t, const WebCore::ResourceRequest&, const WebCore::ResourceResponse&)
{
    notImplemented();
}

} // namespace WebKit
