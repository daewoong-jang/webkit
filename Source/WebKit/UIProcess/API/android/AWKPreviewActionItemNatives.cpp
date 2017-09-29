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
#include "AWKPreviewActionItemNatives.h"

#include "AWKFunctionNatives.h"
#include <androidjni/GlobalRef.h>

namespace org {
namespace webkit {
namespace Natives {

class AWKPreviewActionItemPrivate : public AWKPreviewActionItem::Private {
public:
    std::string m_identifier;
    std::string m_title;
    int32_t m_style;
    JNI::GlobalRef<AWKFunction> m_handler;
};

std::string AWKPreviewActionItem::getIdentifier()
{
    return p<AWKPreviewActionItemPrivate>().m_identifier;
}

AWKPreviewActionItem* AWKPreviewActionItem::nativeCreate(std::string identifier, std::string title, int32_t style, JNI::PassLocalRef<AWKFunction> handler)
{
    std::unique_ptr<AWKPreviewActionItem> ptr(new AWKPreviewActionItem);
    ptr->m_private.reset(new AWKPreviewActionItemPrivate);
    ptr->p<AWKPreviewActionItemPrivate>().m_identifier = identifier;
    ptr->p<AWKPreviewActionItemPrivate>().m_title = title;
    ptr->p<AWKPreviewActionItemPrivate>().m_style = style;
    ptr->p<AWKPreviewActionItemPrivate>().m_handler = handler;
    return ptr.release();
}

}
} // namespace webkit
} // namespace org
