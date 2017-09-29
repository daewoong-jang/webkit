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
#include "AWKScriptMessageNatives.h"

#include "AWKFrameInfoNatives.h"
#include "AWKWebContentNatives.h"
#include <androidjni/GlobalRef.h>

namespace org {
namespace webkit {
namespace Natives {

class AWKScriptMessagePrivate : public AWKScriptMessage::Private {
public:
    JNI::GlobalRef<JNI::NativeObject> m_body;
    JNI::GlobalRef<AWKWebContent> m_webView;
    JNI::GlobalRef<AWKFrameInfo> m_frameInfo;
    std::string m_name;
};

JNI::PassLocalRef<JNI::AnyObject> AWKScriptMessage::getBody()
{
    return p<AWKScriptMessagePrivate>().m_body;
}

JNI::PassLocalRef<AWKWebContent> AWKScriptMessage::getWebContent()
{
    return p<AWKScriptMessagePrivate>().m_webView;
}

JNI::PassLocalRef<AWKFrameInfo> AWKScriptMessage::getFrameInfo()
{
    return p<AWKScriptMessagePrivate>().m_frameInfo;
}

std::string AWKScriptMessage::getName()
{
    return p<AWKScriptMessagePrivate>().m_name;
}

AWKScriptMessage* AWKScriptMessage::nativeCreate(JNI::PassLocalRef<JNI::AnyObject> body, JNI::PassLocalRef<AWKWebContent> webContent, JNI::PassLocalRef<AWKFrameInfo> frameInfo, std::string name)
{
    std::unique_ptr<AWKScriptMessage> ptr(new AWKScriptMessage);
    ptr->m_private.reset(new AWKScriptMessagePrivate);
    ptr->p<AWKScriptMessagePrivate>().m_body = body;
    ptr->p<AWKScriptMessagePrivate>().m_webView = webContent;
    ptr->p<AWKScriptMessagePrivate>().m_frameInfo = frameInfo;
    ptr->p<AWKScriptMessagePrivate>().m_name = name;
    return ptr.release();
}

} // namespace Natives
} // namespace webkit
} // namespace org
