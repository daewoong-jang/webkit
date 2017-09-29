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
#include "AWKBackForwardListItemNatives.h"

#include "AWKNativesCast.h"
#include "StringUtilities.h"
#include "WebBackForwardListItem.h"
#include <wtf/text/CString.h>

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKBackForwardListItemPrivate : public AWKBackForwardListItem::Private {
public:
    RefPtr<WebBackForwardListItem> m_item;
};

template<> JNI::PassLocalRef<AWKBackForwardListItem> AWKBackForwardListItem::create(WebBackForwardListItem* item)
{
    JNI::LocalRef<AWKBackForwardListItem> ref = AWKBackForwardListItem::create();
    ref->p<AWKBackForwardListItemPrivate>().m_item = item;
    return ref.release();
}

template<> WebBackForwardListItem& AWKBackForwardListItem::castTo()
{
    return *p<AWKBackForwardListItemPrivate>().m_item;
}

JNI::PassLocalRef<AWKURL> AWKBackForwardListItem::getURL()
{
    return AWKURL::create(stdStringFromWebCoreString(core(this).url()));
}

std::string AWKBackForwardListItem::getTitle()
{
    return stdStringFromWebCoreString(core(this).title());
}

std::string AWKBackForwardListItem::getOriginalURL()
{
    return stdStringFromWebCoreString(core(this).originalURL());
}

AWKBackForwardListItem* AWKBackForwardListItem::nativeCreate()
{
    std::unique_ptr<AWKBackForwardListItem> ptr(new AWKBackForwardListItem);
    ptr->m_private.reset(new AWKBackForwardListItemPrivate);
    return ptr.release();
}

} // namespace Natives
} // namespace webkit
} // namespace org
