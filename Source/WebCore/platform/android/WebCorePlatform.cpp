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
#include "WebCorePlatform.h"

#include <android/content/res/Resources.h>
#include <android/graphics/Point.h>
#include <java/lang/Managed/String.h>
#include <java/util/Vector.h>

namespace WebCore {
namespace Managed {

using String = java::lang::Managed::String;

int32_t WebCorePlatform::screenDepth()
{
    return 24;
}

bool WebCorePlatform::screenIsMonochrome()
{
    return false;
}

std::shared_ptr<Point> WebCorePlatform::screenSize()
{
    // FIXME: it should return physical size.
    return availableScreenSize();
}

std::shared_ptr<Point> WebCorePlatform::availableScreenSize()
{
    DisplayMetrics& metrics = Resources::getSystem().getDisplayMetrics();
    return Point::create(metrics.widthPixels, metrics.heightPixels);
}

bool WebCorePlatform::requestAllowHostDecision(const std::string& host)
{
    return true;
}

std::string WebCorePlatform::signedPublicKeyAndChallengeString(int32_t index, const std::string& challenge, const std::string& url)
{
    return "";
}

std::string WebCorePlatform::platformVersionForUAString()
{
    return "Android";
}

std::string WebCorePlatform::inspectorBasePath()
{
    return "";
}

std::string WebCorePlatform::websiteDataDirectory()
{
    return "";
}

std::shared_ptr<Vector> WebCorePlatform::getSupportedMIMETypes()
{
    std::shared_ptr<Vector> mimeTypes = Vector::create();
    mimeTypes->addElement(String::create("audio/3gpp"));
    mimeTypes->addElement(String::create("audio/3gpp2"));
    mimeTypes->addElement(String::create("audio/aac"));
    mimeTypes->addElement(String::create("audio/amr"));
    mimeTypes->addElement(String::create("audio/flac"));
    mimeTypes->addElement(String::create("audio/midi"));
    mimeTypes->addElement(String::create("audio/mp4"));
    mimeTypes->addElement(String::create("audio/mpeg"));
    mimeTypes->addElement(String::create("audio/ogg"));
    mimeTypes->addElement(String::create("audio/x-matroska"));
    mimeTypes->addElement(String::create("audio/x-wav"));
    mimeTypes->addElement(String::create("video/3gpp"));
    mimeTypes->addElement(String::create("video/3gpp2"));
    mimeTypes->addElement(String::create("video/mp4"));
    mimeTypes->addElement(String::create("video/mpeg"));
    mimeTypes->addElement(String::create("video/webm"));
    mimeTypes->addElement(String::create("video/x-matroska"));
    return mimeTypes;
}

}
} // namespace WebCore
