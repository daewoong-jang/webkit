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

#ifndef AWKNativesCast_h
#define AWKNativesCast_h

#include "AWKBackForwardListItemNatives.h"
#include "AWKBackForwardListNatives.h"
#include "AWKDownloadNatives.h"
#include "AWKFrameInfoNatives.h"
#include "AWKNavigationActionNatives.h"
#include "AWKNavigationNatives.h"
#include "AWKNavigationResponseNatives.h"
#include "AWKOpenPanelParametersNatives.h"
#include "AWKPreferencesNatives.h"
#include "AWKProcessPoolConfigurationNatives.h"
#include "AWKProcessPoolNatives.h"
#include "AWKSharedNativesCast.h"
#include "AWKUserContentControllerNatives.h"
#include "AWKUserScriptNatives.h"
#include "AWKUserStyleSheetNatives.h"
#include "AWKWebsiteDataRecordNatives.h"
#include "AWKWebsiteDataStoreNatives.h"
#include "AWKWindowFeaturesNatives.h"

namespace API {
class FrameInfo;
class Navigation;
class NavigationAction;
class NavigationResponse;
class OpenPanelParameters;
class ProcessPoolConfiguration;
class UserScript;
class UserStyleSheet;
class WebsiteDataRecord;
class WebsiteDataStore;
class WindowFeatures;
}

namespace WebKit {

class DownloadProxy;
class WebBackForwardList;
class WebBackForwardListItem;
class WebPreferences;
class WebProcessPool;
class WebUserContentControllerProxy;

AWK_ADD_IMPL_MAPPING(org::webkit::AWKBackForwardList, WebBackForwardList)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKBackForwardListItem, WebBackForwardListItem)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKDownload, DownloadProxy)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKFrameInfo, API::FrameInfo)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKNavigation, API::Navigation)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKNavigationAction, API::NavigationAction)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKNavigationResponse, API::NavigationResponse)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKOpenPanelParameters, API::OpenPanelParameters)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKPreferences, WebPreferences)
AWK_ADD_IMPL_MAPPING(org::webkit::Natives::AWKProcessPool, WebProcessPool)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKProcessPoolConfiguration, API::ProcessPoolConfiguration)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKUserContentController, WebUserContentControllerProxy)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKUserScript, API::UserScript)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKUserStyleSheet, API::UserStyleSheet)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKWebsiteDataRecord, API::WebsiteDataRecord)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKWebsiteDataStore, API::WebsiteDataStore)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKWindowFeatures, API::WindowFeatures)

} // namespace WebKit

#endif // AWKNativesCast_h
