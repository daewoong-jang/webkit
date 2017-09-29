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

#ifndef KeyedVariant_h
#define KeyedVariant_h

#include <wtf/RefCounted.h>
#include <wtf/text/WTFString.h>

namespace WebCore {

class SharedBuffer;

enum class DataType : uint32_t {
    NoType,
    Bytes,
    Boolean,
    UInt32,
    Int32,
    Int64,
    Float,
    Double,
    String,
    Object,
    Array,
    Variants,
};

class KeyedVariant final : public RefCounted<KeyedVariant> {
public:
    static RefPtr<KeyedVariant> create();
    static RefPtr<KeyedVariant> create(const String& key, DataType, const String& value);
    ~KeyedVariant() = default;

    DataType type() const { return m_type; }
    const String& key() const { return m_key; }
    const String& value() const { return m_value; }

    void serialize(SharedBuffer&);
    void deserialize(SharedBuffer&, unsigned&);

private:
    KeyedVariant();
    KeyedVariant(const String&, DataType, const String&);

    DataType m_type;
    String m_key;
    String m_value;
};

} // namespace WebCore

#endif // KeyedVariant_h
