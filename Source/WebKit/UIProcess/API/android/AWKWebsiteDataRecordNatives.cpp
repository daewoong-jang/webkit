/*
 * Copyright (C) 2014 NAVER Corp. All rights reserved.
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
#include "AWKWebsiteDataRecordNatives.h"

#include "APIWebsiteDataRecord.h"
#include "AWKNativesCast.h"
#include "StringUtilities.h"
#include <WebCore/NotImplemented.h>
#include <java/util/Natives/Vector.h>
#include <wtf/text/CString.h>

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKWebsiteDataRecordPrivate : public AWKWebsiteDataRecord::Private {
public:
    RefPtr<API::WebsiteDataRecord> m_websiteDataRecord;
};

template<> JNI::PassLocalRef<AWKWebsiteDataRecord> AWKWebsiteDataRecord::create(API::WebsiteDataRecord* websiteDataRecord)
{
    JNI::LocalRef<AWKWebsiteDataRecord> ref = AWKWebsiteDataRecord::create();
    ref->p<AWKWebsiteDataRecordPrivate>().m_websiteDataRecord = websiteDataRecord;
    return ref.release();
}

template<> API::WebsiteDataRecord& AWKWebsiteDataRecord::castTo()
{
    return *p<AWKWebsiteDataRecordPrivate>().m_websiteDataRecord;
}

std::string AWKWebsiteDataRecord::getDisplayName()
{
    return stdStringFromWebCoreString(core(this).websiteDataRecord().displayName);
}

JNI::PassLocalRef<Vector> AWKWebsiteDataRecord::getDataTypes()
{
    notImplemented();
    return nullptr;
}

AWKWebsiteDataRecord* AWKWebsiteDataRecord::nativeCreate()
{
    std::unique_ptr<AWKWebsiteDataRecord> ptr(new AWKWebsiteDataRecord);
    ptr->m_private.reset(new AWKWebsiteDataRecordPrivate);
    return ptr.release();
}

} // namespace Natives
} // namespace webkit
} // namespace org
