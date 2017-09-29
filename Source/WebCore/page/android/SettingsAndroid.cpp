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
#include "Settings.h"

namespace WebCore {

void Settings::initializeDefaultFontFamilies()
{
#if OS(WINDOWS)
    // Copied from the chromium codes.
    // https://code.google.com/p/chromium/codesearch#chromium/src/chrome/app/resources/locale_settings_win.grd
    setStandardFontFamily("Microsoft JhengHei", USCRIPT_TRADITIONAL_HAN);
    setSerifFontFamily("PMingLiU", USCRIPT_TRADITIONAL_HAN);
    setFixedFontFamily("MingLiU", USCRIPT_TRADITIONAL_HAN);
    setSansSerifFontFamily("Microsoft JhengHei", USCRIPT_TRADITIONAL_HAN);

    setStandardFontFamily("Microsoft YaHei", USCRIPT_SIMPLIFIED_HAN);
    setSerifFontFamily("Simsun", USCRIPT_SIMPLIFIED_HAN);
    setFixedFontFamily("NSimsun", USCRIPT_SIMPLIFIED_HAN);
    setSansSerifFontFamily("Microsoft YaHei", USCRIPT_SIMPLIFIED_HAN);

    setStandardFontFamily("Meiryo", USCRIPT_KATAKANA_OR_HIRAGANA);
    setFixedFontFamily("MS Gothic", USCRIPT_KATAKANA_OR_HIRAGANA);
    setSerifFontFamily("MS PMincho", USCRIPT_KATAKANA_OR_HIRAGANA);
    setSansSerifFontFamily("Meiryo", USCRIPT_KATAKANA_OR_HIRAGANA);

    setStandardFontFamily("Malgun Gothic", USCRIPT_HANGUL);
    setSerifFontFamily("Batang", USCRIPT_HANGUL);
    setFixedFontFamily("Gulimche", USCRIPT_HANGUL);
    setSansSerifFontFamily("Malgun Gothic", USCRIPT_HANGUL);
    setCursiveFontFamily("Gungsuh", USCRIPT_HANGUL);
    setFantasyFontFamily("Impact", USCRIPT_HANGUL);
    setPictographFontFamily("Times", USCRIPT_HANGUL);

    setStandardFontFamily("Times New Roman", USCRIPT_COMMON);
    setFixedFontFamily("Courier New", USCRIPT_COMMON);
    setSerifFontFamily("Times New Roman", USCRIPT_COMMON);
    setSansSerifFontFamily("Arial", USCRIPT_COMMON);
    setCursiveFontFamily("Comic Sans MS", USCRIPT_COMMON);
    setFantasyFontFamily("Impact", USCRIPT_COMMON);
    setPictographFontFamily("Segoe UI Symbol", USCRIPT_COMMON);
#endif
}

} // namespace WebCore
