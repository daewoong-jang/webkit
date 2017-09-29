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
#include "AWKDataNatives.h"

#include "APIData.h"

namespace org {
namespace webkit {
namespace Natives {

class AWKDataPrivate : public AWKData::Private {
public:
    RefPtr<API::Data> m_data;
};

template<> JNI::PassLocalRef<AWKData> AWKData::create(API::Data* data)
{
    JNI::LocalRef<AWKData> ref = AWKData::create();
    ref->p<AWKDataPrivate>().m_data = data;
    return ref.release();
}

template<> API::Data& AWKData::castTo()
{
    return *p<AWKDataPrivate>().m_data;
}

AWKData* AWKData::nativeCreate()
{
    std::unique_ptr<AWKData> ptr(new AWKData);
    ptr->m_private.reset(new AWKDataPrivate);
    return ptr.release();
}

AWKData* AWKData::nativeCreateWithBytes(JNI::PassArray<int8_t> bytes)
{
    std::unique_ptr<AWKData> ptr(new AWKData);
    ptr->m_private.reset(new AWKDataPrivate);
    ptr->p<AWKDataPrivate>().m_data = API::Data::create(reinterpret_cast<const unsigned char*>(bytes.data()), bytes.count());
    return ptr.release();
}

}
} // namespace webkit
} // namespace org
