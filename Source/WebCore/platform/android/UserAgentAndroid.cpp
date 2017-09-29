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
#include "UserAgentAndroid.h"

#include "WebCorePlatformNatives.h"
#include <wtf/text/StringBuilder.h>

namespace WebCore {

static const String platformForUAString()
{
#if OS(WINDOWS)
    return "";
#elif OS(ANDROID)
    return "Linux; ";
#endif
}

static const String platformVersionForUAString()
{
    return WebCorePlatform::platformVersionForUAString().c_str();
}

static const String versionForUAString()
{
    // FIXME: Need to get WebKit Version from script.
    return String::format("%d.%d.%d", 600, 5, 14); // r181602 belongs to Safari 600.5.14 version.
}

static String buildDefaultUserAgentString()
{
    StringBuilder uaString;
    uaString.appendLiteral("Mozilla/5.0 ");
    uaString.append('(');

    if (!platformForUAString().isEmpty()) {
        uaString.append(platformForUAString());
        uaString.appendLiteral("; ");
    }

    uaString.append(platformVersionForUAString());

    uaString.appendLiteral(") AppleWebKit/");
    uaString.append(versionForUAString());

    // Version/X is mandatory *before* Safari/X to be a valid Safari UA. See
    // https://bugs.webkit.org/show_bug.cgi?id=133403 for details.
    uaString.appendLiteral(" (KHTML, like Gecko) Version/7.0 Safari/");
    uaString.append(versionForUAString());

    return uaString.toString();
}

String standardUserAgent(const String& applicationName, const String& applicationVersion)
{
    if (applicationName.isEmpty())
        return buildDefaultUserAgentString();

    if (applicationVersion.isEmpty())
        return buildDefaultUserAgentString() + ' ' + applicationName;

    return buildDefaultUserAgentString() + ' ' + applicationName + '/' + applicationVersion;
}

}
