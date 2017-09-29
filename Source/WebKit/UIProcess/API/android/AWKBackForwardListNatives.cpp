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
#include "AWKBackForwardListNatives.h"

#include "AWKNativesCast.h"
#include "WebBackForwardList.h"
#include <java/util/Natives/Vector.h>

using namespace WebKit;

namespace org {
namespace webkit {
namespace Natives {

class AWKBackForwardListPrivate : public AWKBackForwardList::Private {
public:
    RefPtr<WebBackForwardList> m_backForwardList;
};

template<> JNI::PassLocalRef<AWKBackForwardList> AWKBackForwardList::create(WebBackForwardList* backForwardList)
{
    JNI::LocalRef<AWKBackForwardList> ref = AWKBackForwardList::create();
    ref->p<AWKBackForwardListPrivate>().m_backForwardList = backForwardList;
    return ref.release();
}

template<> WebBackForwardList& AWKBackForwardList::castTo()
{
    return *p<AWKBackForwardListPrivate>().m_backForwardList;
}

JNI::PassLocalRef<AWKBackForwardListItem> AWKBackForwardList::getCurrentItem()
{
    WebBackForwardListItem* currentItem = core(this).currentItem();
    if (!currentItem)
        return nullptr;

    return AWKBackForwardListItem::create(currentItem);
}

JNI::PassLocalRef<AWKBackForwardListItem> AWKBackForwardList::getBackItem()
{
    WebBackForwardListItem* backItem = core(this).backItem();
    if (!backItem)
        return nullptr;

    return AWKBackForwardListItem::create(backItem);
}

JNI::PassLocalRef<AWKBackForwardListItem> AWKBackForwardList::getForwardItem()
{
    WebBackForwardListItem* forwardItem = core(this).forwardItem();
    if (!forwardItem)
        return nullptr;

    return AWKBackForwardListItem::create(forwardItem);
}

JNI::PassLocalRef<AWKBackForwardListItem> AWKBackForwardList::getItemAtIndex(int32_t index)
{
    WebBackForwardListItem* item = core(this).itemAtIndex(index);
    if (!item)
        return nullptr;

    return AWKBackForwardListItem::create(item);
}

JNI::PassLocalRef<Vector> AWKBackForwardList::getBackList()
{
    return nullptr;
}

JNI::PassLocalRef<Vector> AWKBackForwardList::getForwardList()
{
    return nullptr;
}

void AWKBackForwardList::removeAllItems()
{
    core(this).removeAllItems();
}

void AWKBackForwardList::clear()
{
    core(this).clear();
}

AWKBackForwardList* AWKBackForwardList::nativeCreate()
{
    std::unique_ptr<AWKBackForwardList> ptr(new AWKBackForwardList);
    ptr->m_private.reset(new AWKBackForwardListPrivate);
    return ptr.release();
}

} // namespace Natives
} // namespace webkit
} // namespace org

