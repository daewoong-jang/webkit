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
#include "InjectedBundle.h"

#include "NotImplemented.h"
#include "WKBundle.h"
#include "WKBundleAPICast.h"
#include "WKBundleInitialize.h"
#include "WKRetainPtr.h"
#include "WKString.h"
#include <WebCore/FileSystem.h>
#include <cutils/log.h>
#include <dlfcn.h>
#include <wtf/text/CString.h>

using namespace WebCore;

namespace WebKit {

bool InjectedBundle::initialize(const WebProcessCreationParameters&, API::Object* initializationUserData)
{
    const char *error;

    m_platformBundle = dlopen(fileSystemRepresentation(m_path).data(), RTLD_LOCAL);
    if (!m_platformBundle) {
        ALOGE("InjectedBundle dlopen error [%s]", dlerror());
        return false;
    }

    WKBundleInitializeFunctionPtr initializeFunction = 0;
    initializeFunction = reinterpret_cast<WKBundleInitializeFunctionPtr>(dlsym(m_platformBundle, "WKBundleInitialize"));

    if (!initializeFunction && (error = dlerror()) != NULL)  {
        ALOGE("InjectedBundle dlsym error [%s]", error);
        return false;
    }

    initializeFunction(toAPI(this), toAPI(initializationUserData));

    return true;
}

void InjectedBundle::setBundleParameter(const String& /*key*/, const IPC::DataReference& /*data*/)
{
    notImplemented();
}

void InjectedBundle::setBundleParameters(const IPC::DataReference&)
{
    notImplemented();
}

} // namespace WebKit
