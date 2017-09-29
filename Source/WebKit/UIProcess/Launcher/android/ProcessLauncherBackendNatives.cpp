/*
 * Copyright (C) 2017 NAVER Corp. All rights reserved.
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
#include "ProcessLauncherBackendNatives.h"

#include "Logging.h"
#include "NetworkProcessMainUnix.h"
#include "ProcessLauncher.h"
#include "WebProcessMainUnix.h"

#if ENABLE(DATABASE_PROCESS)
#include "DatabaseProcessMainUnix.h"
#endif

using namespace WebKit;

namespace org {
namespace webkit {
namespace launcher {
namespace Natives {

static const char* processTypeString(ProcessLauncher::ProcessType processType)
{
    switch (processType) {
    case ProcessLauncher::ProcessType::Web:
        return "webprocess";
    case ProcessLauncher::ProcessType::Network:
        return "networkprocess";
#if ENABLE(DATABASE_PROCESS)
    case ProcessLauncher::ProcessType::Database:
        return "databaseprocess";
#endif
    }

    ASSERT_NOT_REACHED();
    return 0;
}

static ProcessLauncher::ProcessType processTypeEnum(int processType)
{
    switch (processType) {
    case ProcessLauncherBackend::PROCESS_TYPE_WEB:
        return ProcessLauncher::ProcessType::Web;
    case ProcessLauncherBackend::PROCESS_TYPE_NETWORK:
        return ProcessLauncher::ProcessType::Network;
#if ENABLE(DATABASE_PROCESS)
    case ProcessLauncherBackend::PROCESS_TYPE_DATABASE:
        return ProcessLauncher::ProcessType::Database;
#endif
    }

    ASSERT_NOT_REACHED();
    return ProcessLauncher::ProcessType::Web;
}

static int WebKitMain(int processType, int connector)
{
    std::string connectorString = std::to_string(connector);
    const int argc = 2;
    char* argv[argc] = {
        const_cast<char*>(processTypeString(static_cast<ProcessLauncher::ProcessType>(processType))),
        const_cast<char*>(connectorString.data()),
    };

    switch (processTypeEnum(processType)) {
    case ProcessLauncher::ProcessType::Web:
        return WebProcessMainUnix(argc, argv);
    case ProcessLauncher::ProcessType::Network:
        return NetworkProcessMainUnix(argc, argv);
#if ENABLE(DATABASE_PROCESS)
    case ProcessLauncher::ProcessType::Database:
        return DatabaseProcessMainUnix(argc, argv);
#endif
    default:
        ASSERT_NOT_REACHED();
        break;
    }

    return -1;
}

int32_t ProcessLauncherBackend::runWebKitMain(int32_t processType, int32_t connector)
{
    return WebKitMain(processType, connector);
}

ProcessLauncherBackend* ProcessLauncherBackend::CTOR()
{
    return new ProcessLauncherBackend();
}

} // namespace Natives
} // namespace launcher
} // namespace webkit
} // namespace org
