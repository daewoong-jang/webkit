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
#include "KeyedEncoderAndroid.h"

#include "SharedBuffer.h"
#include <wtf/text/CString.h>

namespace WebCore {

std::unique_ptr<KeyedEncoder> KeyedEncoder::encoder()
{
    return std::make_unique<KeyedEncoderAndroid>();
}

KeyedEncoderAndroid::KeyedEncoderAndroid()
{
    m_variants = std::make_shared<VariantVector>();
    m_variantsStack.append(m_variants);
}

KeyedEncoderAndroid::~KeyedEncoderAndroid()
{
    ASSERT(m_variantsStack.size() == 1);
    ASSERT(m_variantsStack.last() == m_variants);
    ASSERT(m_arrayStack.isEmpty());
    ASSERT(m_objectStack.isEmpty());
}

void KeyedEncoderAndroid::encodeBytes(const String& key, const uint8_t* bytes, size_t size)
{
    m_variantsStack.last()->append(KeyedVariant::create(key, DataType::Bytes, String(bytes, size)));
}

void KeyedEncoderAndroid::encodeBool(const String& key, bool value)
{
    m_variantsStack.last()->append(KeyedVariant::create(key, DataType::Boolean, String(value ? "1" : "0")));
}

template<typename T>
void KeyedEncoderAndroid::encodeNumber(const String& key, T value, DataType type)
{
    String number= String::number(value);
    m_variantsStack.last()->append(KeyedVariant::create(key, type, number));
}

void KeyedEncoderAndroid::encodeUInt32(const String& key, uint32_t value)
{
    encodeNumber(key, value, DataType::UInt32);
}

void KeyedEncoderAndroid::encodeInt32(const String& key, int32_t value)
{
    encodeNumber(key, value, DataType::Int32);
}

void KeyedEncoderAndroid::encodeInt64(const String& key, int64_t value)
{
    encodeNumber(key, value, DataType::Int64);
}

void KeyedEncoderAndroid::encodeFloat(const String& key, float value)
{
    encodeNumber(key, value, DataType::Float);
}

void KeyedEncoderAndroid::encodeDouble(const String& key, double value)
{
    encodeNumber(key, value, DataType::Double);
}

void KeyedEncoderAndroid::encodeString(const String& key, const String& value)
{
    m_variantsStack.last()->append(KeyedVariant::create(key, DataType::String, value));
}

String KeyedEncoderAndroid::variantsToBytes(const std::shared_ptr<VariantVector>& variants)
{
    RefPtr<SharedBuffer> buffer = SharedBuffer::create();
    size_t dictionarySize = variants->size();
    buffer->append(reinterpret_cast<const char*>(&dictionarySize), sizeof(size_t));
    for (auto& variant : *variants)
        variant->serialize(*buffer);

    return String(buffer->data(), buffer->size());
}

void KeyedEncoderAndroid::beginObject(const String& key)
{
    std::shared_ptr<VariantVector> vector = std::make_shared<VariantVector>();
    m_objectStack.append(std::make_pair(key, vector));
    m_variantsStack.append(vector);
}

void KeyedEncoderAndroid::endObject()
{
    String value = variantsToBytes(m_variantsStack.takeLast());
    m_variantsStack.last()->append(KeyedVariant::create(m_objectStack.last().first, DataType::Object, value));
    m_objectStack.removeLast();
}

void KeyedEncoderAndroid::beginArray(const String& key)
{
    std::shared_ptr<VariantVector> vector = std::make_shared<VariantVector>();
    m_arrayStack.append(std::make_pair(key, vector));
}

void KeyedEncoderAndroid::beginArrayElement()
{
    std::shared_ptr<VariantVector> vector = std::make_shared<VariantVector>();
    m_variantsStack.append(vector);
}

void KeyedEncoderAndroid::endArrayElement()
{
    String value = variantsToBytes(m_variantsStack.takeLast());
    m_arrayStack.last().second->append(KeyedVariant::create(m_arrayStack.last().first, DataType::Variants, value));
}

void KeyedEncoderAndroid::endArray()
{
    String value = variantsToBytes(m_arrayStack.last().second);
    m_variantsStack.last()->append(KeyedVariant::create(m_arrayStack.last().first, DataType::Array, value));
    m_arrayStack.removeLast();
}

RefPtr<SharedBuffer> KeyedEncoderAndroid::finishEncoding()
{
    RefPtr<SharedBuffer> buffer = SharedBuffer::create();
    String data = variantsToBytes(m_variantsStack.last());
    return SharedBuffer::create(data.latin1().data(), data.length());;
}

} // namespace WebKit
