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
#include "AWKFunctionNatives.h"

#include <wtf/Function.h>
#include <wtf/text/WTFString.h>

namespace org {
namespace webkit {
namespace Natives {

class AWKFunctionPrivate : public AWKFunction::Private {
public:
    virtual void invoke()
    {
        ASSERT_NOT_REACHED();
    }

    virtual void invoke(bool value)
    {
        ASSERT_NOT_REACHED();
    }

    virtual void invoke(int32_t value)
    {
        ASSERT_NOT_REACHED();
    }

    virtual void invoke(int64_t value)
    {
        ASSERT_NOT_REACHED();
    }

    virtual void invoke(float value)
    {
        ASSERT_NOT_REACHED();
    }

    virtual void invoke(double value)
    {
        ASSERT_NOT_REACHED();
    }

    virtual void invoke(const std::string& value)
    {
        ASSERT_NOT_REACHED();
    }

    virtual void invoke(JNI::PassLocalRef<JNI::AnyObject> value)
    {
        ASSERT_NOT_REACHED();
    }

    virtual void invoke(JNI::PassLocalRef<JNI::AnyObject> first, JNI::PassLocalRef<JNI::AnyObject> second)
    {
        ASSERT_NOT_REACHED();
    }
};

template<template<typename> class F, typename... T>
class AWKFunctionPrivateBase : public AWKFunctionPrivate {
public:
    typedef F<void (T...)> BaseFunction;
protected:
    AWKFunctionPrivateBase(BaseFunction&& function)
        : m_function(WTFMove(function)) { }

    BaseFunction m_function;
};

void AWKFunction::invoke()
{
    p<AWKFunctionPrivate>().invoke();
}

void AWKFunction::invoke(bool value)
{
    p<AWKFunctionPrivate>().invoke(value);
}

void AWKFunction::invoke(int32_t value)
{
    p<AWKFunctionPrivate>().invoke(value);
}

void AWKFunction::invoke(int64_t value)
{
    p<AWKFunctionPrivate>().invoke(value);
}

void AWKFunction::invoke(float value)
{
    p<AWKFunctionPrivate>().invoke(value);
}

void AWKFunction::invoke(double value)
{
    p<AWKFunctionPrivate>().invoke(value);
}

void AWKFunction::invoke(const std::string& value)
{
    p<AWKFunctionPrivate>().invoke(value);
}

void AWKFunction::invoke(JNI::PassLocalRef<JNI::AnyObject> value)
{
    p<AWKFunctionPrivate>().invoke(value);
}

void AWKFunction::invoke(JNI::PassLocalRef<JNI::AnyObject> first, JNI::PassLocalRef<JNI::AnyObject> second)
{
    p<AWKFunctionPrivate>().invoke(first, second);
}

AWKFunction* AWKFunction::nativeCreate(int64_t id)
{
    std::unique_ptr<AWKFunction> ptr(new AWKFunction);
    ptr->m_private.reset(reinterpret_cast<AWKFunctionPrivate*>(id));
    return ptr.release();
}

template <template<typename> class, typename...>
class AWKFunctionPrivateImpl;

#define DEFINE_VOID_FUNCTION_PRIVATE_AND_CTOR(Base) \
    template<> class AWKFunctionPrivateImpl<Base> final : public AWKFunctionPrivateBase<Base> { \
    public: \
        AWKFunctionPrivateImpl(BaseFunction&& function) \
            : AWKFunctionPrivateBase(WTFMove(function)) { } \
        void invoke() override \
        { \
            m_function(); \
        } \
    }; \
    template<> JNI::PassLocalRef<AWKFunction> AWKFunction::create(Base<void ()>&& function) \
    { \
        return AWKFunction::create(reinterpret_cast<int64_t>(new AWKFunctionPrivateImpl<Base>(WTFMove(function)))); \
    }

DEFINE_VOID_FUNCTION_PRIVATE_AND_CTOR(std::function);
DEFINE_VOID_FUNCTION_PRIVATE_AND_CTOR(WTF::Function);

#define DEFINE_FUNCTION_PRIVATE_AND_CTOR_IMPL(Base, type) \
    template<> class AWKFunctionPrivateImpl<Base, type> final : public AWKFunctionPrivateBase<Base, type> { \
    public: \
        AWKFunctionPrivateImpl(BaseFunction&& function) \
            : AWKFunctionPrivateBase(WTFMove(function)) { } \
        void invoke(type value) override \
        { \
            m_function(value); \
        } \
    }; \
    template<> JNI::PassLocalRef<AWKFunction> AWKFunction::create(Base<void (type)>&& function) \
    { \
        return AWKFunction::create(reinterpret_cast<int64_t>(new AWKFunctionPrivateImpl<Base, type>(WTFMove(function)))); \
    }

#define DEFINE_FUNCTION_PRIVATE_AND_CTOR(type) \
    DEFINE_FUNCTION_PRIVATE_AND_CTOR_IMPL(std::function, type) \
    DEFINE_FUNCTION_PRIVATE_AND_CTOR_IMPL(WTF::Function, type)

DEFINE_FUNCTION_PRIVATE_AND_CTOR(bool);
DEFINE_FUNCTION_PRIVATE_AND_CTOR(int32_t);
DEFINE_FUNCTION_PRIVATE_AND_CTOR(int64_t);
DEFINE_FUNCTION_PRIVATE_AND_CTOR(float);
DEFINE_FUNCTION_PRIVATE_AND_CTOR(double);
DEFINE_FUNCTION_PRIVATE_AND_CTOR(const std::string&);
DEFINE_FUNCTION_PRIVATE_AND_CTOR(JNI::PassLocalRef<JNI::AnyObject>);

#define DEFINE_FUNCTION_PRIVATE_AND_CTOR_WITH_CAST(type, typeTo, Base) \
    template<> class AWKFunctionPrivateImpl<Base, typeTo> final : public AWKFunctionPrivateBase<Base, typeTo> { \
    public: \
        AWKFunctionPrivateImpl(BaseFunction&& function) \
            : AWKFunctionPrivateBase(WTFMove(function)) { } \
        void invoke(type value) override \
        { \
            m_function(kit(value)); \
        } \
    }; \
    template<> JNI::PassLocalRef<AWKFunction> AWKFunction::create(Base<void (typeTo)>&& function) \
    { \
        return AWKFunction::create(reinterpret_cast<int64_t>(new AWKFunctionPrivateImpl<Base, typeTo>(WTFMove(function)))); \
    }

String kit(const std::string& value)
{
    return String::fromUTF8(value.c_str());
}

DEFINE_FUNCTION_PRIVATE_AND_CTOR_WITH_CAST(const std::string&, const WTF::String&, WTF::Function);

}
} // namespace webkit
} // namespace org
