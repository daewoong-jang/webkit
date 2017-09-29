/*
 * Copyright (C) 2017 Daewoong Jang.
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

#if OS(WINDOWS)

#include <windows.h>

#include "android/WebKitProcess.h"
#include "android/WebKitWebProcess1.h"
#include "android/WebKitWebProcess2.h"
#include "android/WebKitWebProcess3.h"
#include "android/WebKitWebProcess4.h"
#include "android/WebKitWebProcess5.h"
#include "android/WebKitWebProcess6.h"
#include "android/WebKitWebProcess7.h"
#include "android/WebKitWebProcess8.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            classT<WebKitProcess>(L"org.webkit.launcher", L"WebKitProcess");
            classT<WebKitWebProcess1>(L"org.webkit.launcher", L"WebKitWebProcess1");
            classT<WebKitWebProcess2>(L"org.webkit.launcher", L"WebKitWebProcess2");
            classT<WebKitWebProcess3>(L"org.webkit.launcher", L"WebKitWebProcess3");
            classT<WebKitWebProcess4>(L"org.webkit.launcher", L"WebKitWebProcess4");
            classT<WebKitWebProcess5>(L"org.webkit.launcher", L"WebKitWebProcess5");
            classT<WebKitWebProcess6>(L"org.webkit.launcher", L"WebKitWebProcess6");
            classT<WebKitWebProcess7>(L"org.webkit.launcher", L"WebKitWebProcess7");
            classT<WebKitWebProcess8>(L"org.webkit.launcher", L"WebKitWebProcess8");
            return TRUE;

        case DLL_PROCESS_DETACH:
            break;

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
    }
    return FALSE;
}

#endif