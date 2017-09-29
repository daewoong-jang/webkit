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

package org.webkit;

import android.view.KeyEvent;

public final class VirtualKeyMap {
    static final int VK_BACK = 0x08;
    static final int VK_TAB = 0x09;
    static final int VK_BACKTAB = 0x0A;
    static final int VK_CLEAR = 0x0C;
    static final int VK_RETURN = 0x0D;
    static final int VK_SHIFT = 0x10;
    static final int VK_CONTROL = 0x11;
    static final int VK_MENU = 0x12;
    static final int VK_PAUSE = 0x13;
    static final int VK_CAPITAL = 0x14;
    static final int VK_KANA = 0x15;
    static final int VK_HANGUL = 0x15;
    static final int VK_JUNJA = 0x17;
    static final int VK_FINAL = 0x18;
    static final int VK_HANJA = 0x19;
    static final int VK_KANJI = 0x19;
    static final int VK_ESCAPE = 0x1B;
    static final int VK_CONVERT = 0x1C;
    static final int VK_NONCONVERT = 0x1D;
    static final int VK_ACCEPT = 0x1E;
    static final int VK_MODECHANGE = 0x1F;
    static final int VK_SPACE = 0x20;
    static final int VK_PRIOR = 0x21;
    static final int VK_NEXT = 0x22;
    static final int VK_END = 0x23;
    static final int VK_HOME = 0x24;
    static final int VK_LEFT = 0x25;
    static final int VK_UP = 0x26;
    static final int VK_RIGHT = 0x27;
    static final int VK_DOWN = 0x28;
    static final int VK_SELECT = 0x29;
    static final int VK_PRINT = 0x2A;
    static final int VK_EXECUTE = 0x2B;
    static final int VK_SNAPSHOT = 0x2C;
    static final int VK_INSERT = 0x2D;
    static final int VK_DELETE = 0x2E;
    static final int VK_HELP = 0x2F;
    static final int VK_0 = 0x30;
    static final int VK_1 = 0x31;
    static final int VK_2 = 0x32;
    static final int VK_3 = 0x33;
    static final int VK_4 = 0x34;
    static final int VK_5 = 0x35;
    static final int VK_6 = 0x36;
    static final int VK_7 = 0x37;
    static final int VK_8 = 0x38;
    static final int VK_9 = 0x39;
    static final int VK_A = 0x41;
    static final int VK_B = 0x42;
    static final int VK_C = 0x43;
    static final int VK_D = 0x44;
    static final int VK_E = 0x45;
    static final int VK_F = 0x46;
    static final int VK_G = 0x47;
    static final int VK_H = 0x48;
    static final int VK_I = 0x49;
    static final int VK_J = 0x4A;
    static final int VK_K = 0x4B;
    static final int VK_L = 0x4C;
    static final int VK_M = 0x4D;
    static final int VK_N = 0x4E;
    static final int VK_O = 0x4F;
    static final int VK_P = 0x50;
    static final int VK_Q = 0x51;
    static final int VK_R = 0x52;
    static final int VK_S = 0x53;
    static final int VK_T = 0x54;
    static final int VK_U = 0x55;
    static final int VK_V = 0x56;
    static final int VK_W = 0x57;
    static final int VK_X = 0x58;
    static final int VK_Y = 0x59;
    static final int VK_Z = 0x5A;
    static final int VK_LWIN = 0x5B;
    static final int VK_COMMAND = VK_LWIN;
    static final int VK_RWIN = 0x5C;
    static final int VK_APPS = 0x5D;
    static final int VK_SLEEP = 0x5F;
    static final int VK_NUMPAD0 = 0x60;
    static final int VK_NUMPAD1 = 0x61;
    static final int VK_NUMPAD2 = 0x62;
    static final int VK_NUMPAD3 = 0x63;
    static final int VK_NUMPAD4 = 0x64;
    static final int VK_NUMPAD5 = 0x65;
    static final int VK_NUMPAD6 = 0x66;
    static final int VK_NUMPAD7 = 0x67;
    static final int VK_NUMPAD8 = 0x68;
    static final int VK_NUMPAD9 = 0x69;
    static final int VK_MULTIPLY = 0x6A;
    static final int VK_ADD = 0x6B;
    static final int VK_SEPARATOR = 0x6C;
    static final int VK_SUBTRACT = 0x6D;
    static final int VK_DECIMAL = 0x6E;
    static final int VK_DIVIDE = 0x6F;
    static final int VK_F1 = 0x70;
    static final int VK_F2 = 0x71;
    static final int VK_F3 = 0x72;
    static final int VK_F4 = 0x73;
    static final int VK_F5 = 0x74;
    static final int VK_F6 = 0x75;
    static final int VK_F7 = 0x76;
    static final int VK_F8 = 0x77;
    static final int VK_F9 = 0x78;
    static final int VK_F10 = 0x79;
    static final int VK_F11 = 0x7A;
    static final int VK_F12 = 0x7B;
    static final int VK_F13 = 0x7C;
    static final int VK_F14 = 0x7D;
    static final int VK_F15 = 0x7E;
    static final int VK_F16 = 0x7F;
    static final int VK_F17 = 0x80;
    static final int VK_F18 = 0x81;
    static final int VK_F19 = 0x82;
    static final int VK_F20 = 0x83;
    static final int VK_F21 = 0x84;
    static final int VK_F22 = 0x85;
    static final int VK_F23 = 0x86;
    static final int VK_F24 = 0x87;
    static final int VK_NUMLOCK = 0x90;
    static final int VK_SCROLL = 0x91;
    static final int VK_LSHIFT = 0xA0;
    static final int VK_RSHIFT = 0xA1;
    static final int VK_LCONTROL = 0xA2;
    static final int VK_RCONTROL = 0xA3;
    static final int VK_LMENU = 0xA4;
    static final int VK_RMENU = 0xA5;
    static final int VK_BROWSER_BACK = 0xA6;
    static final int VK_BROWSER_FORWARD = 0xA7;
    static final int VK_BROWSER_REFRESH = 0xA8;
    static final int VK_BROWSER_STOP = 0xA9;
    static final int VK_BROWSER_SEARCH = 0xAA;
    static final int VK_BROWSER_FAVORITES = 0xAB;
    static final int VK_BROWSER_HOME = 0xAC;
    static final int VK_VOLUME_MUTE = 0xAD;
    static final int VK_VOLUME_DOWN = 0xAE;
    static final int VK_VOLUME_UP = 0xAF;
    static final int VK_MEDIA_NEXT_TRACK = 0xB0;
    static final int VK_MEDIA_PREV_TRACK = 0xB1;
    static final int VK_MEDIA_STOP = 0xB2;
    static final int VK_MEDIA_PLAY_PAUSE = 0xB3;
    static final int VK_MEDIA_LAUNCH_MAIL = 0xB4;
    static final int VK_MEDIA_LAUNCH_MEDIA_SELECT = 0xB5;
    static final int VK_MEDIA_LAUNCH_APP1 = 0xB6;
    static final int VK_MEDIA_LAUNCH_APP2 = 0xB7;
    static final int VK_OEM_1 = 0xBA;
    static final int VK_OEM_PLUS = 0xBB;
    static final int VK_OEM_COMMA = 0xBC;
    static final int VK_OEM_MINUS = 0xBD;
    static final int VK_OEM_PERIOD = 0xBE;
    static final int VK_OEM_2 = 0xBF;
    static final int VK_OEM_3 = 0xC0;
    static final int VK_OEM_4 = 0xDB;
    static final int VK_OEM_5 = 0xDC;
    static final int VK_OEM_6 = 0xDD;
    static final int VK_OEM_7 = 0xDE;
    static final int VK_OEM_8 = 0xDF;
    static final int VK_OEM_102 = 0xE2;
    static final int VK_OEM_103 = 0xE3;
    static final int VK_OEM_104 = 0xE4;
    static final int VK_PROCESSKEY = 0xE5;
    static final int VK_PACKET = 0xE7;
    static final int VK_DBE_SBCSCHAR = 0xF3;
    static final int VK_DBE_DBCSCHAR = 0xF4;
    static final int VK_ATTN = 0xF6;
    static final int VK_CRSEL = 0xF7;
    static final int VK_EXSEL = 0xF8;
    static final int VK_EREOF = 0xF9;
    static final int VK_PLAY = 0xFA;
    static final int VK_ZOOM = 0xFB;
    static final int VK_NONAME = 0xFC;
    static final int VK_PA1 = 0xFD;
    static final int VK_OEM_CLEAR = 0xFE;
    static final int VK_UNKNOWN = 0;
    static final int VK_WLAN = 0x97;
    static final int VK_POWER = 0x98;
    static final int VK_BRIGHTNESS_DOWN = 0xD8;
    static final int VK_BRIGHTNESS_UP = 0xD9;
    static final int VK_KBD_BRIGHTNESS_DOWN = 0xDA;
    static final int VK_KBD_BRIGHTNESS_UP = 0xE8;
    static final int VK_ALTGR = 0xE1;

    public static String fromUnicode(int code) {
        return String.valueOf((char) code);
    }

    public static boolean isKeypadKey(int keyCode) {
        return keyCode > KeyEvent.KEYCODE_NUM_LOCK
                && keyCode <= KeyEvent.KEYCODE_NUMPAD_RIGHT_PAREN;
    }

    public static int toVirtualKeyCode(int keyCode) {
        // Does not provide all key codes, and does not handle all keys.
        switch (keyCode) {
        case KeyEvent.KEYCODE_DEL:
            return VK_BACK;
        case KeyEvent.KEYCODE_TAB:
            return VK_TAB;
        case KeyEvent.KEYCODE_CLEAR:
            return VK_CLEAR;
        case KeyEvent.KEYCODE_DPAD_CENTER:
        case KeyEvent.KEYCODE_ENTER:
            return VK_RETURN;
        case KeyEvent.KEYCODE_SHIFT_LEFT:
            return VK_LSHIFT;
        case KeyEvent.KEYCODE_SHIFT_RIGHT:
            return VK_RSHIFT;
            // Back will serve as escape, although we may not have access to it.
        case KeyEvent.KEYCODE_BACK:
            return VK_ESCAPE;
        case KeyEvent.KEYCODE_SPACE:
            return VK_SPACE;
        case KeyEvent.KEYCODE_MOVE_HOME:
            return VK_HOME;
        case KeyEvent.KEYCODE_DPAD_LEFT:
            return VK_LEFT;
        case KeyEvent.KEYCODE_DPAD_UP:
            return VK_UP;
        case KeyEvent.KEYCODE_DPAD_RIGHT:
            return VK_RIGHT;
        case KeyEvent.KEYCODE_DPAD_DOWN:
            return VK_DOWN;
        case KeyEvent.KEYCODE_0:
            return VK_0;
        case KeyEvent.KEYCODE_1:
            return VK_1;
        case KeyEvent.KEYCODE_2:
            return VK_2;
        case KeyEvent.KEYCODE_3:
            return VK_3;
        case KeyEvent.KEYCODE_4:
            return VK_4;
        case KeyEvent.KEYCODE_5:
            return VK_5;
        case KeyEvent.KEYCODE_6:
            return VK_6;
        case KeyEvent.KEYCODE_7:
            return VK_7;
        case KeyEvent.KEYCODE_8:
            return VK_8;
        case KeyEvent.KEYCODE_9:
            return VK_9;
        case KeyEvent.KEYCODE_A:
            return VK_A;
        case KeyEvent.KEYCODE_B:
            return VK_B;
        case KeyEvent.KEYCODE_C:
            return VK_C;
        case KeyEvent.KEYCODE_D:
            return VK_D;
        case KeyEvent.KEYCODE_E:
            return VK_E;
        case KeyEvent.KEYCODE_F:
            return VK_F;
        case KeyEvent.KEYCODE_G:
            return VK_G;
        case KeyEvent.KEYCODE_H:
            return VK_H;
        case KeyEvent.KEYCODE_I:
            return VK_I;
        case KeyEvent.KEYCODE_J:
            return VK_J;
        case KeyEvent.KEYCODE_K:
            return VK_K;
        case KeyEvent.KEYCODE_L:
            return VK_L;
        case KeyEvent.KEYCODE_M:
            return VK_M;
        case KeyEvent.KEYCODE_N:
            return VK_N;
        case KeyEvent.KEYCODE_O:
            return VK_O;
        case KeyEvent.KEYCODE_P:
            return VK_P;
        case KeyEvent.KEYCODE_Q:
            return VK_Q;
        case KeyEvent.KEYCODE_R:
            return VK_R;
        case KeyEvent.KEYCODE_S:
            return VK_S;
        case KeyEvent.KEYCODE_T:
            return VK_T;
        case KeyEvent.KEYCODE_U:
            return VK_U;
        case KeyEvent.KEYCODE_V:
            return VK_V;
        case KeyEvent.KEYCODE_W:
            return VK_W;
        case KeyEvent.KEYCODE_X:
            return VK_X;
        case KeyEvent.KEYCODE_Y:
            return VK_Y;
        case KeyEvent.KEYCODE_Z:
            return VK_Z;
        case KeyEvent.KEYCODE_VOLUME_DOWN:
            return VK_VOLUME_DOWN;
        case KeyEvent.KEYCODE_VOLUME_UP:
            return VK_VOLUME_UP;
        case KeyEvent.KEYCODE_MEDIA_NEXT:
            return VK_MEDIA_NEXT_TRACK;
        case KeyEvent.KEYCODE_MEDIA_PREVIOUS:
            return VK_MEDIA_PREV_TRACK;
        case KeyEvent.KEYCODE_MEDIA_STOP:
            return VK_MEDIA_STOP;
        case KeyEvent.KEYCODE_MEDIA_PAUSE:
            return VK_MEDIA_PLAY_PAUSE;
            // Colon key.
        case KeyEvent.KEYCODE_SEMICOLON:
            return VK_OEM_1;
        case KeyEvent.KEYCODE_COMMA:
            return VK_OEM_COMMA;
        case KeyEvent.KEYCODE_MINUS:
            return VK_OEM_MINUS;
        case KeyEvent.KEYCODE_EQUALS:
            return VK_OEM_PLUS;
        case KeyEvent.KEYCODE_PERIOD:
            return VK_OEM_PERIOD;
        case KeyEvent.KEYCODE_SLASH:
            return VK_OEM_2;
        case KeyEvent.KEYCODE_LEFT_BRACKET:
            return VK_OEM_4;
        case KeyEvent.KEYCODE_BACKSLASH:
            return VK_OEM_5;
        case KeyEvent.KEYCODE_RIGHT_BRACKET:
            return VK_OEM_6;
        case KeyEvent.KEYCODE_MUTE:
        case KeyEvent.KEYCODE_VOLUME_MUTE:
            return VK_VOLUME_MUTE;
        case KeyEvent.KEYCODE_ESCAPE:
            return VK_ESCAPE;
        case KeyEvent.KEYCODE_MEDIA_PLAY:
        case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
            return VK_MEDIA_PLAY_PAUSE;
        case KeyEvent.KEYCODE_MOVE_END:
            return VK_END;
        case KeyEvent.KEYCODE_ALT_LEFT:
            return VK_LMENU;
        case KeyEvent.KEYCODE_ALT_RIGHT:
            return VK_RMENU;
        case KeyEvent.KEYCODE_GRAVE:
            return VK_OEM_3;
        case KeyEvent.KEYCODE_APOSTROPHE:
            return VK_OEM_3;
        case KeyEvent.KEYCODE_MEDIA_REWIND:
            return VK_OEM_103;
        case KeyEvent.KEYCODE_MEDIA_FAST_FORWARD:
            return VK_OEM_104;
        case KeyEvent.KEYCODE_PAGE_UP:
            return VK_PRIOR;
        case KeyEvent.KEYCODE_PAGE_DOWN:
            return VK_NEXT;
        case KeyEvent.KEYCODE_FORWARD_DEL:
            return VK_DELETE;
        case KeyEvent.KEYCODE_CTRL_LEFT:
            return VK_LCONTROL;
        case KeyEvent.KEYCODE_CTRL_RIGHT:
            return VK_RCONTROL;
        case KeyEvent.KEYCODE_CAPS_LOCK:
            return VK_CAPITAL;
        case KeyEvent.KEYCODE_SCROLL_LOCK:
            return VK_SCROLL;
        case KeyEvent.KEYCODE_META_LEFT:
            return VK_LWIN;
        case KeyEvent.KEYCODE_META_RIGHT:
            return VK_RWIN;
        case KeyEvent.KEYCODE_BREAK:
            return VK_PAUSE;
        case KeyEvent.KEYCODE_INSERT:
            return VK_INSERT;
        case KeyEvent.KEYCODE_F1:
            return VK_F1;
        case KeyEvent.KEYCODE_F2:
            return VK_F2;
        case KeyEvent.KEYCODE_F3:
            return VK_F3;
        case KeyEvent.KEYCODE_F4:
            return VK_F4;
        case KeyEvent.KEYCODE_F5:
            return VK_F5;
        case KeyEvent.KEYCODE_F6:
            return VK_F6;
        case KeyEvent.KEYCODE_F7:
            return VK_F7;
        case KeyEvent.KEYCODE_F8:
            return VK_F8;
        case KeyEvent.KEYCODE_F9:
            return VK_F9;
        case KeyEvent.KEYCODE_F10:
            return VK_F10;
        case KeyEvent.KEYCODE_F11:
            return VK_F11;
        case KeyEvent.KEYCODE_F12:
            return VK_F12;
        case KeyEvent.KEYCODE_NUM_LOCK:
            return VK_NUMLOCK;
        case KeyEvent.KEYCODE_NUMPAD_0:
            return VK_NUMPAD0;
        case KeyEvent.KEYCODE_NUMPAD_1:
            return VK_NUMPAD1;
        case KeyEvent.KEYCODE_NUMPAD_2:
            return VK_NUMPAD2;
        case KeyEvent.KEYCODE_NUMPAD_3:
            return VK_NUMPAD3;
        case KeyEvent.KEYCODE_NUMPAD_4:
            return VK_NUMPAD4;
        case KeyEvent.KEYCODE_NUMPAD_5:
            return VK_NUMPAD5;
        case KeyEvent.KEYCODE_NUMPAD_6:
            return VK_NUMPAD6;
        case KeyEvent.KEYCODE_NUMPAD_7:
            return VK_NUMPAD7;
        case KeyEvent.KEYCODE_NUMPAD_8:
            return VK_NUMPAD8;
        case KeyEvent.KEYCODE_NUMPAD_9:
            return VK_NUMPAD9;
        case KeyEvent.KEYCODE_NUMPAD_DIVIDE:
            return VK_DIVIDE;
        case KeyEvent.KEYCODE_NUMPAD_MULTIPLY:
            return VK_MULTIPLY;
        case KeyEvent.KEYCODE_NUMPAD_SUBTRACT:
            return VK_SUBTRACT;
        case KeyEvent.KEYCODE_NUMPAD_ADD:
            return VK_ADD;
        case KeyEvent.KEYCODE_NUMPAD_DOT:
            return VK_DECIMAL;
        case KeyEvent.KEYCODE_CHANNEL_UP:
            return VK_PRIOR;
        case KeyEvent.KEYCODE_CHANNEL_DOWN:
            return VK_NEXT;
        default:
            return VK_UNKNOWN;
        }
    }

    public static int toKeyEventKeyCode(int virtualKeyCode) {
        // Does not provide all key codes, and does not handle all keys.
        switch (virtualKeyCode) {
        case VK_DELETE:
            return KeyEvent.KEYCODE_FORWARD_DEL;
        case VK_TAB:
            return KeyEvent.KEYCODE_TAB;
        case VK_CLEAR:
            return KeyEvent.KEYCODE_CLEAR;
        case VK_RETURN:
            return KeyEvent.KEYCODE_ENTER;
        case VK_LSHIFT:
            return KeyEvent.KEYCODE_SHIFT_LEFT;
        case VK_RSHIFT:
            return KeyEvent.KEYCODE_SHIFT_RIGHT;
        case VK_BACK:
            return KeyEvent.KEYCODE_BACK;
        case VK_SPACE:
            return KeyEvent.KEYCODE_SPACE;
        case VK_HOME:
            return KeyEvent.KEYCODE_MOVE_HOME;
        case VK_LEFT:
            return KeyEvent.KEYCODE_DPAD_LEFT;
        case VK_UP:
            return KeyEvent.KEYCODE_DPAD_UP;
        case VK_RIGHT:
            return KeyEvent.KEYCODE_DPAD_RIGHT;
        case VK_DOWN:
            return KeyEvent.KEYCODE_DPAD_DOWN;
        case VK_0:
            return KeyEvent.KEYCODE_0;
        case VK_1:
            return KeyEvent.KEYCODE_1;
        case VK_2:
            return KeyEvent.KEYCODE_2;
        case VK_3:
            return KeyEvent.KEYCODE_3;
        case VK_4:
            return KeyEvent.KEYCODE_4;
        case VK_5:
            return KeyEvent.KEYCODE_5;
        case VK_6:
            return KeyEvent.KEYCODE_6;
        case VK_7:
            return KeyEvent.KEYCODE_7;
        case VK_8:
            return KeyEvent.KEYCODE_8;
        case VK_9:
            return KeyEvent.KEYCODE_9;
        case VK_A:
            return KeyEvent.KEYCODE_A;
        case VK_B:
            return KeyEvent.KEYCODE_B;
        case VK_C:
            return KeyEvent.KEYCODE_C;
        case VK_D:
            return KeyEvent.KEYCODE_D;
        case VK_E:
            return KeyEvent.KEYCODE_E;
        case VK_F:
            return KeyEvent.KEYCODE_F;
        case VK_G:
            return KeyEvent.KEYCODE_G;
        case VK_H:
            return KeyEvent.KEYCODE_H;
        case VK_I:
            return KeyEvent.KEYCODE_I;
        case VK_J:
            return KeyEvent.KEYCODE_J;
        case VK_K:
            return KeyEvent.KEYCODE_K;
        case VK_L:
            return KeyEvent.KEYCODE_L;
        case VK_M:
            return KeyEvent.KEYCODE_M;
        case VK_N:
            return KeyEvent.KEYCODE_N;
        case VK_O:
            return KeyEvent.KEYCODE_O;
        case VK_P:
            return KeyEvent.KEYCODE_P;
        case VK_Q:
            return KeyEvent.KEYCODE_Q;
        case VK_R:
            return KeyEvent.KEYCODE_R;
        case VK_S:
            return KeyEvent.KEYCODE_S;
        case VK_T:
            return KeyEvent.KEYCODE_T;
        case VK_U:
            return KeyEvent.KEYCODE_U;
        case VK_V:
            return KeyEvent.KEYCODE_V;
        case VK_W:
            return KeyEvent.KEYCODE_W;
        case VK_X:
            return KeyEvent.KEYCODE_X;
        case VK_Y:
            return KeyEvent.KEYCODE_Y;
        case VK_Z:
            return KeyEvent.KEYCODE_Z;
        case VK_VOLUME_DOWN:
            return KeyEvent.KEYCODE_VOLUME_DOWN;
        case VK_VOLUME_UP:
            return KeyEvent.KEYCODE_VOLUME_UP;
        case VK_MEDIA_NEXT_TRACK:
            return KeyEvent.KEYCODE_MEDIA_NEXT;
        case VK_MEDIA_PREV_TRACK:
            return KeyEvent.KEYCODE_MEDIA_PREVIOUS;
        case VK_MEDIA_STOP:
            return KeyEvent.KEYCODE_MEDIA_STOP;
        case VK_OEM_1:
            return KeyEvent.KEYCODE_SEMICOLON;
        case VK_OEM_COMMA:
            return KeyEvent.KEYCODE_COMMA;
        case VK_OEM_MINUS:
            return KeyEvent.KEYCODE_MINUS;
        case VK_OEM_PLUS:
            return KeyEvent.KEYCODE_EQUALS;
        case VK_OEM_PERIOD:
            return KeyEvent.KEYCODE_PERIOD;
        case VK_OEM_2:
            return KeyEvent.KEYCODE_SLASH;
        case VK_OEM_4:
            return KeyEvent.KEYCODE_LEFT_BRACKET;
        case VK_OEM_5:
            return KeyEvent.KEYCODE_BACKSLASH;
        case VK_OEM_6:
            return KeyEvent.KEYCODE_RIGHT_BRACKET;
        case VK_VOLUME_MUTE:
            return KeyEvent.KEYCODE_VOLUME_MUTE;
        case VK_ESCAPE:
            return KeyEvent.KEYCODE_ESCAPE;
        case VK_MEDIA_PLAY_PAUSE:
            return KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE;
        case VK_END:
            return KeyEvent.KEYCODE_MOVE_END;
        case VK_LMENU:
            return KeyEvent.KEYCODE_ALT_LEFT;
        case VK_RMENU:
            return KeyEvent.KEYCODE_ALT_RIGHT;
        case VK_OEM_3:
            return KeyEvent.KEYCODE_APOSTROPHE;
        case VK_OEM_103:
            return KeyEvent.KEYCODE_MEDIA_REWIND;
        case VK_OEM_104:
            return KeyEvent.KEYCODE_MEDIA_FAST_FORWARD;
        case VK_PRIOR:
            return KeyEvent.KEYCODE_PAGE_UP;
        case VK_NEXT:
            return KeyEvent.KEYCODE_PAGE_DOWN;
        case VK_LCONTROL:
            return KeyEvent.KEYCODE_CTRL_LEFT;
        case VK_RCONTROL:
            return KeyEvent.KEYCODE_CTRL_RIGHT;
        case VK_CAPITAL:
            return KeyEvent.KEYCODE_CAPS_LOCK;
        case VK_SCROLL:
            return KeyEvent.KEYCODE_SCROLL_LOCK;
        case VK_LWIN:
            return KeyEvent.KEYCODE_META_LEFT;
        case VK_RWIN:
            return KeyEvent.KEYCODE_META_RIGHT;
        case VK_PAUSE:
            return KeyEvent.KEYCODE_BREAK;
        case VK_INSERT:
            return KeyEvent.KEYCODE_INSERT;
        case VK_F1:
            return KeyEvent.KEYCODE_F1;
        case VK_F2:
            return KeyEvent.KEYCODE_F2;
        case VK_F3:
            return KeyEvent.KEYCODE_F3;
        case VK_F4:
            return KeyEvent.KEYCODE_F4;
        case VK_F5:
            return KeyEvent.KEYCODE_F5;
        case VK_F6:
            return KeyEvent.KEYCODE_F6;
        case VK_F7:
            return KeyEvent.KEYCODE_F7;
        case VK_F8:
            return KeyEvent.KEYCODE_F8;
        case VK_F9:
            return KeyEvent.KEYCODE_F9;
        case VK_F10:
            return KeyEvent.KEYCODE_F10;
        case VK_F11:
            return KeyEvent.KEYCODE_F11;
        case VK_F12:
            return KeyEvent.KEYCODE_F12;
        case VK_NUMLOCK:
            return KeyEvent.KEYCODE_NUM_LOCK;
        case VK_NUMPAD0:
            return KeyEvent.KEYCODE_NUMPAD_0;
        case VK_NUMPAD1:
            return KeyEvent.KEYCODE_NUMPAD_1;
        case VK_NUMPAD2:
            return KeyEvent.KEYCODE_NUMPAD_2;
        case VK_NUMPAD3:
            return KeyEvent.KEYCODE_NUMPAD_3;
        case VK_NUMPAD4:
            return KeyEvent.KEYCODE_NUMPAD_4;
        case VK_NUMPAD5:
            return KeyEvent.KEYCODE_NUMPAD_5;
        case VK_NUMPAD6:
            return KeyEvent.KEYCODE_NUMPAD_6;
        case VK_NUMPAD7:
            return KeyEvent.KEYCODE_NUMPAD_7;
        case VK_NUMPAD8:
            return KeyEvent.KEYCODE_NUMPAD_8;
        case VK_NUMPAD9:
            return KeyEvent.KEYCODE_NUMPAD_9;
        case VK_DIVIDE:
            return KeyEvent.KEYCODE_NUMPAD_DIVIDE;
        case VK_MULTIPLY:
            return KeyEvent.KEYCODE_NUMPAD_MULTIPLY;
        case VK_SUBTRACT:
            return KeyEvent.KEYCODE_NUMPAD_SUBTRACT;
        case VK_ADD:
            return KeyEvent.KEYCODE_NUMPAD_ADD;
        case VK_DECIMAL:
            return KeyEvent.KEYCODE_NUMPAD_DOT;
        default:
            return KeyEvent.KEYCODE_UNKNOWN;
        }
    }

    public static String getKeyIdentifier(int keyCode) {
        int virtualKeyCode = toVirtualKeyCode(keyCode);
        switch (virtualKeyCode) {
        case VK_MENU:
            return "Alt";
        case VK_CONTROL:
            return "Control";
        case VK_SHIFT:
            return "Shift";
        case VK_CAPITAL:
            return "CapsLock";
        case VK_LWIN:
        case VK_RWIN:
            return "Win";
        case VK_CLEAR:
            return "Clear";
        case VK_DOWN:
            return "Down";
        case VK_END:
            return "End";
        case VK_RETURN:
            return "Enter";
        case VK_EXECUTE:
            return "Execute";
        case VK_F1:
            return "F1";
        case VK_F2:
            return "F2";
        case VK_F3:
            return "F3";
        case VK_F4:
            return "F4";
        case VK_F5:
            return "F5";
        case VK_F6:
            return "F6";
        case VK_F7:
            return "F7";
        case VK_F8:
            return "F8";
        case VK_F9:
            return "F9";
        case VK_F10:
            return "F10";
        case VK_F11:
            return "F11";
        case VK_F12:
            return "F12";
        case VK_F13:
            return "F13";
        case VK_F14:
            return "F14";
        case VK_F15:
            return "F15";
        case VK_F16:
            return "F16";
        case VK_F17:
            return "F17";
        case VK_F18:
            return "F18";
        case VK_F19:
            return "F19";
        case VK_F20:
            return "F20";
        case VK_F21:
            return "F21";
        case VK_F22:
            return "F22";
        case VK_F23:
            return "F23";
        case VK_F24:
            return "F24";
        case VK_HELP:
            return "Help";
        case VK_HOME:
            return "Home";
        case VK_INSERT:
            return "Insert";
        case VK_LEFT:
            return "Left";
        case VK_NEXT:
            return "PageDown";
        case VK_PRIOR:
            return "PageUp";
        case VK_PAUSE:
            return "Pause";
        case VK_SNAPSHOT:
            return "PrintScreen";
        case VK_RIGHT:
            return "Right";
        case VK_SCROLL:
            return "Scroll";
        case VK_SELECT:
            return "Select";
        case VK_UP:
            return "Up";
        case VK_DELETE:
            return "U+007F"; // Standard says that DEL becomes U+007F.
        case VK_MEDIA_NEXT_TRACK:
            return "MediaNextTrack";
        case VK_MEDIA_PREV_TRACK:
            return "MediaPreviousTrack";
        case VK_MEDIA_STOP:
            return "MediaStop";
        case VK_MEDIA_PLAY_PAUSE:
            return "MediaPlayPause";
        case VK_VOLUME_MUTE:
            return "VolumeMute";
        case VK_VOLUME_DOWN:
            return "VolumeDown";
        case VK_VOLUME_UP:
            return "VolumeUp";
        default:
            return "";
        }
    }
}
