/*
 * Copyright (C) 2013 NAVER Corp. All rights reserved.
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
#include "ProcessLauncher.h"

#include "ProcessExecutablePath.h"
#include "ProcessLauncherStrategy.h"
#include <wtf/RunLoop.h>

namespace WebKit {

void ProcessLauncher::launchProcess()
{
    std::unique_ptr<ProcessLauncherStrategy> strategy = ProcessLauncherStrategy::create();

    IPC::Connection::SocketPair sockets = strategy->createPlatformConnection();
    if (sockets.client == -1 || sockets.server == -1) {
        LOG_ERROR("Creation of socket failed with errno: %d", errno);
        ASSERT_NOT_REACHED();
        return;
    }

    RefPtr<ProcessLauncher> protect(this);
    m_processIdentifier = strategy->launchProcess(m_launchOptions.processType,
        org::webkit::launcher::ProcessLauncherBackend::PROCESS_STRATEGY_SERVICE, sockets,
        JNI::wrap<org::webkit::AWKFunction>(lambda(
            [protect, sockets] (int pid) {
                RunLoop::main().dispatch([=] { protect->didFinishLaunchingProcess(pid, sockets.server); });
            })
        )
    );

    if (!m_processIdentifier) {
        LOG_ERROR("Failed to start process");
    }
}

void ProcessLauncher::terminateProcess()
{
    if (m_isLaunching) {
        invalidate();
        return;
    }

    if (!m_processIdentifier)
        return;

    org::webkit::launcher::ProcessLauncherBackend::stop(m_processIdentifier);

    m_processIdentifier = 0;
}

void ProcessLauncher::platformInvalidate()
{
    org::webkit::launcher::ProcessLauncherBackend::stop(m_processIdentifier);
}

} // namespace WebKit
