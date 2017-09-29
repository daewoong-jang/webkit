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

#if OS(ANDROID)

#include <androidjni/JavaVM.h>

#include "AWKBackForwardListItemNatives.h"
#include "AWKBackForwardListNatives.h"
#include "AWKCallbackNatives.h"
#include "AWKDataNatives.h"
#include "AWKDownloadDelegateNatives.h"
#include "AWKDownloadNatives.h"
#include "AWKErrorNatives.h"
#include "AWKFrameInfoNatives.h"
#include "AWKFunctionNatives.h"
#include "AWKNavigationActionNatives.h"
#include "AWKNavigationDelegateNatives.h"
#include "AWKNavigationNatives.h"
#include "AWKNavigationResponseNatives.h"
#include "AWKOpenPanelParametersNatives.h"
#include "AWKPreferencesNatives.h"
#include "AWKPreviewActionItemNatives.h"
#include "AWKPreviewElementInfoNatives.h"
#include "AWKProcessPoolConfigurationNatives.h"
#include "AWKProcessPoolNatives.h"
#include "AWKScriptMessageHandlerNatives.h"
#include "AWKScriptMessageNatives.h"
#include "AWKSecurityOriginNatives.h"
#include "AWKUIDelegateNatives.h"
#include "AWKURLNatives.h"
#include "AWKURLRequestNatives.h"
#include "AWKURLResponseNatives.h"
#include "AWKUserContentControllerNatives.h"
#include "AWKUserScriptNatives.h"
#include "AWKUserStyleSheetNatives.h"
#include "AWKViewNatives.h"
#include "AWKWebContentConfigurationNatives.h"
#include "AWKWebContentNatives.h"
#include "AWKWebsiteDataRecordNatives.h"
#include "AWKWebsiteDataStoreNatives.h"
#include "AWKWindowFeaturesNatives.h"
#include "ProcessLauncherBackendNatives.h"
#include "WebCorePlatformNatives.h"
#include "WebEditableNatives.h"
#include "WebRendererNatives.h"
#include <org/webkit/Natives/CompositionClause.h>
#include <org/webkit/wtf/Natives/ThreadLoopBackend.h>
#include <org/webkit/wtf/Natives/WTFMainThread.h>

using namespace WTF::Natives;
using namespace WebCore;
using namespace org::webkit::Natives;
using namespace org::webkit::launcher;

static void registerClasses()
{
    AWKBackForwardList::registerClass();
    AWKBackForwardListItem::registerClass();
    AWKCallback::registerClass();
    AWKData::registerClass();
    AWKDownload::registerClass();
    AWKDownloadDelegate::registerClass();
    AWKError::registerClass();
    AWKFrameInfo::registerClass();
    AWKFunction::registerClass();
    AWKNavigation::registerClass();
    AWKNavigationAction::registerClass();
    AWKNavigationDelegate::registerClass();
    AWKNavigationResponse::registerClass();
    AWKOpenPanelParameters::registerClass();
    AWKPreferences::registerClass();
    AWKPreviewActionItem::registerClass();
    AWKPreviewElementInfo::registerClass();
    AWKProcessPool::registerClass();
    AWKProcessPoolConfiguration::registerClass();
    AWKScriptMessage::registerClass();
    AWKScriptMessageHandler::registerClass();
    AWKSecurityOrigin::registerClass();
    AWKUIDelegate::registerClass();
    AWKURL::registerClass();
    AWKURLRequest::registerClass();
    AWKURLResponse::registerClass();
    AWKUserContentController::registerClass();
    AWKUserScript::registerClass();
    AWKUserStyleSheet::registerClass();
    AWKView::registerClass();
    AWKWebContent::registerClass();
    AWKWebContentConfiguration::registerClass();
    AWKWebsiteDataRecord::registerClass();
    AWKWebsiteDataStore::registerClass();
    AWKWindowFeatures::registerClass();
    CompositionClause::registerClass();
    ProcessLauncherBackend::registerClass();
    ThreadLoopBackend::registerClass();
    WTFMainThread::registerClass();
    WebCorePlatform::registerClass();
    WebEditable::registerClass();
    WebRenderer::registerClass();
}

JNI_EXPORT jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    // Save the JavaVM pointer for use globally.
    JNI::setVM(vm);

    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("GetEnv failed!");
        return result;
    }

    registerClasses();

    return JNI_VERSION_1_4;
}

#endif // OS(ANDROID)
