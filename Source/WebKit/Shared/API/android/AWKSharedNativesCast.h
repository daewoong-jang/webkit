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

#ifndef AWKSharedNativesCast_h
#define AWKSharedNativesCast_h

#include "AWKDataNatives.h"
#include "AWKErrorNatives.h"
#include "AWKSecurityOriginNatives.h"
#include "AWKURLNatives.h"
#include "AWKURLRequestNatives.h"
#include "AWKURLResponseNatives.h"

namespace API {
class Data;
class Error;
class SecurityOrigin;
class URLRequest;
class URLResponse;
}

namespace WebCore {
class URL;
}

namespace WebKit {

template<typename AWKType> struct AWKTypeInfo;

#define AWK_ADD_IMPL_MAPPING(TheAWKType, TheImplType) \
    template<> struct AWKTypeInfo<TheAWKType> { typedef TheImplType ImplType; };

AWK_ADD_IMPL_MAPPING(org::webkit::AWKData, API::Data)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKError, API::Error)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKSecurityOrigin, API::SecurityOrigin)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKURL, WebCore::URL)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKURLRequest, API::URLRequest)
AWK_ADD_IMPL_MAPPING(org::webkit::AWKURLResponse, API::URLResponse)

template<typename T, template<typename> class RefType, typename ImplType = typename AWKTypeInfo<T>::ImplType>
auto core(RefType<T> ref) -> ImplType&
{
    return ref->template castTo<ImplType&>();
}

template<typename T, typename ImplType = typename AWKTypeInfo<T>::ImplType>
auto core(T* ptr) -> ImplType&
{
    return ptr->template castTo<ImplType&>();
}

} // namespace WebKit

#endif // AWKSharedNativesCast_h
