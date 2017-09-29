/*
 * Copyright (C) 2013 NAVER Corp. All rights reserved.
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
#include "TextChecker.h"

#include "TextCheckerState.h"
#include "WebTextChecker.h"
#include <WebCore/NotImplemented.h>

using namespace WebCore;

namespace WebKit {

TextCheckerState& checkerState()
{
    static TextCheckerState textCheckerState;
    static std::once_flag onceFlag;
    std::call_once(onceFlag, [] {
        textCheckerState.isContinuousSpellCheckingEnabled = false;
        textCheckerState.isGrammarCheckingEnabled = false;
    });

    return textCheckerState;
}

const TextCheckerState& TextChecker::state()
{
    return checkerState();
}

static bool testingModeEnabled = false;

void TextChecker::setTestingMode(bool enabled)
{
    testingModeEnabled = enabled;
}

bool TextChecker::isTestingMode()
{
    return testingModeEnabled;
}

bool TextChecker::isContinuousSpellCheckingAllowed()
{
    return true;
}

void TextChecker::setContinuousSpellCheckingEnabled(bool isContinuousSpellCheckingEnabled)
{
    if (state().isContinuousSpellCheckingEnabled == isContinuousSpellCheckingEnabled)
        return;

    checkerState().isContinuousSpellCheckingEnabled = isContinuousSpellCheckingEnabled;
}

void TextChecker::setGrammarCheckingEnabled(bool isGrammarCheckingEnabled)
{
    if (state().isGrammarCheckingEnabled == isGrammarCheckingEnabled)
        return;

    checkerState().isGrammarCheckingEnabled = isGrammarCheckingEnabled;
}

void TextChecker::continuousSpellCheckingEnabledStateChanged(bool enabled)
{
    checkerState().isContinuousSpellCheckingEnabled = enabled;
}

void TextChecker::grammarCheckingEnabledStateChanged(bool enabled)
{
    checkerState().isGrammarCheckingEnabled = enabled;
}

int64_t TextChecker::uniqueSpellDocumentTag(WebPageProxy*)
{
    notImplemented();
    return 0;
}

void TextChecker::closeSpellDocumentWithTag(int64_t)
{
    notImplemented();
}

void TextChecker::checkSpellingOfString(int64_t, StringView, int32_t&, int32_t&)
{
    notImplemented();
}

void TextChecker::checkGrammarOfString(int64_t, StringView, Vector<WebCore::GrammarDetail>&, int32_t&, int32_t&)
{
    notImplemented();
}

bool TextChecker::spellingUIIsShowing()
{
    return false;
}

void TextChecker::toggleSpellingUIIsShowing()
{
    notImplemented();
}

void TextChecker::updateSpellingUIWithMisspelledWord(int64_t, const String&)
{
    notImplemented();
}

void TextChecker::updateSpellingUIWithGrammarString(int64_t, const String&, const GrammarDetail&)
{
    notImplemented();
}

void TextChecker::getGuessesForWord(int64_t, const String&, const String&, int32_t, Vector<String>&, bool)
{
    notImplemented();
}

void TextChecker::learnWord(int64_t, const String&)
{
    notImplemented();
}

void TextChecker::ignoreWord(int64_t, const String&)
{
    notImplemented();
}

void TextChecker::requestCheckingOfString(Ref<TextCheckerCompletion>&&, int32_t)
{
    notImplemented();
}

} // namespace WebKit
