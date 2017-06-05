/*
	Copyright 2015 Mariusz Dzikowski

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#ifndef PT_KEY_H_
#define PT_KEY_H_

enum {
	PT_KEY_UNKNOWN = -1, ///< Unhandled key
	PT_KEY_A = 0,        ///< The A key
	PT_KEY_B,            ///< The B key
	PT_KEY_C,            ///< The C key
	PT_KEY_D,            ///< The D key
	PT_KEY_E,            ///< The E key
	PT_KEY_F,            ///< The F key
	PT_KEY_G,            ///< The G key
	PT_KEY_H,            ///< The H key
	PT_KEY_I,            ///< The I key
	PT_KEY_J,            ///< The J key
	PT_KEY_K,            ///< The K key
	PT_KEY_L,            ///< The L key
	PT_KEY_M,            ///< The M key
	PT_KEY_N,            ///< The N key
	PT_KEY_O,            ///< The O key
	PT_KEY_P,            ///< The P key
	PT_KEY_Q,            ///< The Q key
	PT_KEY_R,            ///< The R key
	PT_KEY_S,            ///< The S key
	PT_KEY_T,            ///< The T key
	PT_KEY_U,            ///< The U key
	PT_KEY_V,            ///< The V key
	PT_KEY_W,            ///< The W key
	PT_KEY_X,            ///< The X key
	PT_KEY_Y,            ///< THE Y KEY
	PT_KEY_Z,            ///< THE Z KEY
	PT_KEY_NUM0,         ///< THE 0 KEY
	PT_KEY_NUM1,         ///< THE 1 KEY
	PT_KEY_NUM2,         ///< THE 2 KEY
	PT_KEY_NUM3,         ///< THE 3 KEY
	PT_KEY_NUM4,         ///< THE 4 KEY
	PT_KEY_NUM5,         ///< THE 5 KEY
	PT_KEY_NUM6,         ///< THE 6 KEY
	PT_KEY_NUM7,         ///< THE 7 KEY
	PT_KEY_NUM8,         ///< THE 8 KEY
	PT_KEY_NUM9,         ///< THE 9 KEY
	PT_KEY_ESCAPE,       ///< THE ESCAPE KEY
	PT_KEY_LCONTROL,     ///< THE LEFT CONTROL KEY
	PT_KEY_LSHIFT,       ///< THE LEFT SHIFT KEY
	PT_KEY_LALT,         ///< THE LEFT ALT KEY
	PT_KEY_LSYSTEM,      ///< THE LEFT OS SPECIFIC KEY: WINDOW (WINDOWS AND LINUX), APPLE (MACOS X), ...
	PT_KEY_RCONTROL,     ///< THE RIGHT CONTROL KEY
	PT_KEY_RSHIFT,       ///< THE RIGHT SHIFT KEY
	PT_KEY_RALT,         ///< THE RIGHT ALT KEY
	PT_KEY_RSYSTEM,      ///< THE RIGHT OS SPECIFIC KEY: WINDOW (WINDOWS AND LINUX), APPLE (MACOS X), ...
	PT_KEY_MENU,         ///< THE MENU KEY
	PT_KEY_LBRACKET,     ///< THE [ KEY
	PT_KEY_RBRACKET,     ///< THE ] KEY
	PT_KEY_SEMICOLON,    ///< THE ; KEY
	PT_KEY_COMMA,        ///< THE , KEY
	PT_KEY_PERIOD,       ///< THE . KEY
	PT_KEY_QUOTE,        ///< THE ' KEY
	PT_KEY_SLASH,        ///< THE / KEY
	PT_KEY_BACKSLASH,    ///< THE \ KEY
	PT_KEY_TILDE,        ///< THE ~ KEY
	PT_KEY_EQUAL,        ///< THE = KEY
	PT_KEY_DASH,         ///< THE - KEY
	PT_KEY_SPACE,        ///< THE SPACE KEY
	PT_KEY_RETURN,       ///< THE RETURN KEY
	PT_KEY_BACKSPACE,    ///< THE BACKSPACE KEY
	PT_KEY_TAB,          ///< THE TABULATION KEY
	PT_KEY_PAGEUP,       ///< THE PAGE UP KEY
	PT_KEY_PAGEDOWN,     ///< THE PAGE DOWN KEY
	PT_KEY_END,          ///< THE END KEY
	PT_KEY_HOME,         ///< THE HOME KEY
	PT_KEY_INSERT,       ///< THE INSERT KEY
	PT_KEY_DELETE,       ///< THE DELETE KEY
	PT_KEY_ADD,          ///< THE + KEY
	PT_KEY_SUBTRACT,     ///< THE - KEY
	PT_KEY_MULTIPLY,     ///< THE * KEY
	PT_KEY_DIVIDE,       ///< THE / KEY
	PT_KEY_LEFT,         ///< LEFT ARROW
	PT_KEY_RIGHT,        ///< RIGHT ARROW
	PT_KEY_UP,           ///< UP ARROW
	PT_KEY_DOWN,         ///< DOWN ARROW
	PT_KEY_NUMPAD0,      ///< THE NUMPAD 0 KEY
	PT_KEY_NUMPAD1,      ///< THE NUMPAD 1 KEY
	PT_KEY_NUMPAD2,      ///< THE NUMPAD 2 KEY
	PT_KEY_NUMPAD3,      ///< THE NUMPAD 3 KEY
	PT_KEY_NUMPAD4,      ///< THE NUMPAD 4 KEY
	PT_KEY_NUMPAD5,      ///< THE NUMPAD 5 KEY
	PT_KEY_NUMPAD6,      ///< THE NUMPAD 6 KEY
	PT_KEY_NUMPAD7,      ///< THE NUMPAD 7 KEY
	PT_KEY_NUMPAD8,      ///< THE NUMPAD 8 KEY
	PT_KEY_NUMPAD9,      ///< THE NUMPAD 9 KEY
	PT_KEY_F1,           ///< THE F1 KEY
	PT_KEY_F2,           ///< THE F2 KEY
	PT_KEY_F3,           ///< THE F3 KEY
	PT_KEY_F4,           ///< THE F4 KEY
	PT_KEY_F5,           ///< THE F5 KEY
	PT_KEY_F6,           ///< THE F6 KEY
	PT_KEY_F7,           ///< THE F7 KEY
	PT_KEY_F8,           ///< THE F8 KEY
	PT_KEY_F9,           ///< THE F9 KEY
	PT_KEY_F10,          ///< THE F10 KEY
	PT_KEY_F11,          ///< THE F11 KEY
	PT_KEY_F12,          ///< THE F12 KEY
	PT_KEY_F13,          ///< THE F13 KEY
	PT_KEY_F14,          ///< THE F14 KEY
	PT_KEY_F15,          ///< THE F15 KEY
	PT_KEY_PAUSE,        ///< THE PAUSE KEY

	PT_KEY_KEYCOUNT      ///< KEEP LAST -- THE TOTAL NUMBER OF KEYBOARD KEYS
};

#endif