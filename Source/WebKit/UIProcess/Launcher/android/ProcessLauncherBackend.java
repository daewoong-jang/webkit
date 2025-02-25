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

package org.webkit.launcher;

import org.webkit.AWKFunction;

import org.webkit.AccessedByNative;
import org.webkit.CalledByNative;
import org.webkit.NativeNamespace;
import static org.webkit.LogHelper.*;

@NativeNamespace("org.webkit.launcher")
class ProcessLauncherBackend {
    // UIProcess/Launcher/ProcessLauncher.h
    @AccessedByNative
    protected static final int PROCESS_TYPE_WEB = 0;
    @AccessedByNative
    protected static final int PROCESS_TYPE_NETWORK = 1;
    @AccessedByNative
    protected static final int PROCESS_TYPE_DATABASE = 2;

    @AccessedByNative
    protected static final int PROCESS_STRATEGY_SERVICE = 0;
    @AccessedByNative
    protected static final int PROCESS_STRATEGY_THREAD = 1;
    @AccessedByNative
    protected static final int PROCESS_STRATEGY_DEFAULT = PROCESS_STRATEGY_SERVICE;

    @CalledByNative
    private static int start(int processType, int processStrategy,
            int connector, AWKFunction didFinishLaunchingProcess) {
        return WebKitProcessLauncher.get().connect(processType,
                processStrategy, connector, didFinishLaunchingProcess);
    }

    @CalledByNative
    private static void stop(int pid) {
        if (!WebKitProcessLauncher.get().disconnect(pid)) {
            LOGW("Process %d is not a valid connection");
        }
    }

    protected static native int runWebKitMain(int processType, int connector);

    private ProcessLauncherBackend() {
        // Private class, do not instantiate.
    }
}
