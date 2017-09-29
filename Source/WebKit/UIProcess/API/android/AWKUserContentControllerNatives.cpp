/*
 * Copyright (C) 2014-2015 NAVER Corp. All rights reserved.
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
#include "AWKUserContentControllerNatives.h"

#include "APIUserContentWorld.h"
#include "AWKNativesCast.h"
#include "AWKScriptMessageHandlerNatives.h"
#include "WebUserContentControllerProxy.h"
#include <WebCore/NotImplemented.h>
#include <java/util/Natives/Vector.h>

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKUserContentControllerPrivate : public AWKUserContentController::Private {
public:
    RefPtr<WebUserContentControllerProxy> m_userContentController;
};

template<> JNI::PassLocalRef<AWKUserContentController> AWKUserContentController::create(WebUserContentControllerProxy* userContentController)
{
    JNI::LocalRef<AWKUserContentController> ref = AWKUserContentController::create();
    ref->p<AWKUserContentControllerPrivate>().m_userContentController = userContentController;
    return ref.release();
}

template<> WebUserContentControllerProxy& AWKUserContentController::castTo()
{
    return *p<AWKUserContentControllerPrivate>().m_userContentController;
}

JNI::PassLocalRef<Vector> AWKUserContentController::getUserScripts()
{
    notImplemented();
    return nullptr;
}

void AWKUserContentController::addUserScript(JNI::PassLocalRef<AWKUserScript> userScript)
{
    return core(this).addUserScript(core(userScript));
}

void AWKUserContentController::removeAllUserScripts()
{
    core(this).removeAllUserScripts();
}

void AWKUserContentController::addScriptMessageHandler(JNI::PassLocalRef<AWKScriptMessageHandler> scriptMessageHandler, const std::string& name)
{
    notImplemented();
}

void AWKUserContentController::removeScriptMessageHandlerForName(const std::string& name)
{
    core(this).removeUserMessageHandlerForName(String::fromUTF8(name.c_str(), name.length()), API::UserContentWorld::normalWorld());
}

void AWKUserContentController::addUserStyleSheet(JNI::PassLocalRef<AWKUserStyleSheet> userStyleSheet)
{
    notImplemented();
}

void AWKUserContentController::removeAllUserStyleSheets()
{
    core(this).removeAllUserStyleSheets();
}

AWKUserContentController* AWKUserContentController::nativeCreate()
{
    std::unique_ptr<AWKUserContentController> ptr(new AWKUserContentController);
    ptr->m_private.reset(new AWKUserContentControllerPrivate);
    ptr->p<AWKUserContentControllerPrivate>().m_userContentController = WebUserContentControllerProxy::create();
    return ptr.release();
}

} // namespace Natives
} // namespace webkit
} // namespace org
