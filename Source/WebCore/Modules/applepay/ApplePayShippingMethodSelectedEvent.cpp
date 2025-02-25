/*
 * Copyright (C) 2016 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "ApplePayShippingMethodSelectedEvent.h"

#if ENABLE(APPLE_PAY)

#include <wtf/text/StringBuilder.h>

namespace WebCore {

static inline String convert(int64_t amount)
{
    StringBuilder amountString;
    amountString.appendNumber(amount / 100);
    amountString.append('.');

    unsigned decimals = amount % 100;
    if (decimals < 10)
        amountString.append('0');
    amountString.appendNumber(decimals);

    return amountString.toString();
}

static inline ApplePayShippingMethod convert(const ApplePaySessionPaymentRequest::ShippingMethod& shippingMethod)
{
    ApplePayShippingMethod convertedMethod;
    convertedMethod.label = shippingMethod.label;
    convertedMethod.detail = shippingMethod.detail;
    convertedMethod.identifier = shippingMethod.identifier;
    convertedMethod.amount = convert(shippingMethod.amount);

    return convertedMethod; 
}

ApplePayShippingMethodSelectedEvent::ApplePayShippingMethodSelectedEvent(const AtomicString& type, const ApplePaySessionPaymentRequest::ShippingMethod& shippingMethod)
    : Event(type, false, false)
    , m_shippingMethod(convert(shippingMethod))
{
}

ApplePayShippingMethodSelectedEvent::~ApplePayShippingMethodSelectedEvent()
{
}

EventInterface ApplePayShippingMethodSelectedEvent::eventInterface() const
{
    return ApplePayShippingMethodSelectedEventInterfaceType;
}

}

#endif
