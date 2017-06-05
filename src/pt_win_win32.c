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
#include <potion.h>

#ifdef PT_WINDOWS

#include <Windows.h>

static HWND hwnd = 0;
static HDC hdc = 0;
static LRESULT CALLBACK windowProc(HWND, UINT, WPARAM, LPARAM);
static int open = 0;

static pt_key_callback key_down = 0;
static pt_key_callback key_up = 0;
static pt_text_callback text_input = 0;

int pt_win_init(int width, int height) {
	WNDCLASS windowClass;
	windowClass.style = 0;
	windowClass.lpfnWndProc = windowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = "RabBit";
	RegisterClass(&windowClass);

	HDC screenDC = GetDC(NULL);
	int left = (GetDeviceCaps(screenDC, HORZRES) - width) / 2;
	int top = (GetDeviceCaps(screenDC, VERTRES) - height) / 2;
	ReleaseDC(NULL, screenDC);

	RECT rect;
	rect.left = rect.top = 0;
	rect.right = width;
	rect.bottom = height;

	DWORD style = WS_VISIBLE | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

	AdjustWindowRect(&rect, style, FALSE);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	hwnd = CreateWindow("RabBit", "PotionMini", style, left, top, width, height, NULL, NULL, GetModuleHandle(NULL), NULL);
	if (!hwnd) {
		return 0;
	}

	hdc = GetDC(hwnd);
	return open = 1;
}

int pt_win_is_open(void) {
	return open;
}

void pt_win_release(void) {
	if (hwnd) {
		ReleaseDC(hwnd, hdc);
		DestroyWindow(hwnd);
	}
	UnregisterClass("RabBit", GetModuleHandle(NULL));
	open = 0;
}

int rb_window_is_open(void) {
	return open;
}

void pt_win_close(void) {
	open = 0;
}

void pt_win_poll_events(void) {
	MSG message;
	while (PeekMessage(&message, hwnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

int pt_win_has_focus(void) {
	return GetFocus() == hwnd;
}

void pt_win_get_size(int* width, int* height) {
	RECT rect;
	GetClientRect(hwnd, &rect);

	if (width) {
		*width = rect.right - rect.left;
	}

	if (height) {
		*height = rect.bottom - rect.top;
	}
}

void pt_win_set_size(int width, int height) {
	RECT rect;
	rect.left = rect.top = 0;
	rect.right = width;
	rect.bottom = height; 
	AdjustWindowRect(&rect, GetWindowLong(hwnd, GWL_STYLE), FALSE);

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	SetWindowPos(hwnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
}

void pt_win_update(pt_color4b* pixels) {
	int width, height;
	pt_win_get_size(&width, &height);

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	SetDIBitsToDevice(hdc, 0, 0, width, height, 0, 0, 0, height, pixels, &bmi, DIB_RGB_COLORS);
}

void pt_win_set_key_down_callback(pt_key_callback callback) {
	key_down = callback;
}

void pt_win_set_key_up_callback(pt_key_callback callback) {
	key_up = callback;
}

void pt_win_set_text_callback(pt_text_callback callback) {
	text_input = callback;
}

static 
int to_pt_key(WPARAM  key, LPARAM flags) {
	switch (key) {
		case VK_SHIFT: {
			UINT lShift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
			UINT scancode = (UINT)((flags & (0xFF << 16)) >> 16);
			return (scancode == lShift) ? PT_KEY_LSHIFT : PT_KEY_RSHIFT;
		}

		case VK_MENU: 
			return (HIWORD(flags) & KF_EXTENDED) ? PT_KEY_RALT : PT_KEY_LALT;

		case VK_CONTROL: 
			return (HIWORD(flags) & KF_EXTENDED) ? PT_KEY_RCONTROL : PT_KEY_LCONTROL;

		case VK_LWIN:       return PT_KEY_LSYSTEM;
		case VK_RWIN:       return PT_KEY_RSYSTEM;
		case VK_APPS:       return PT_KEY_MENU;
		case VK_OEM_1:      return PT_KEY_SEMICOLON;
		case VK_OEM_2:      return PT_KEY_SLASH;
		case VK_OEM_PLUS:   return PT_KEY_EQUAL;
		case VK_OEM_MINUS:  return PT_KEY_DASH;
		case VK_OEM_4:      return PT_KEY_LBRACKET;
		case VK_OEM_6:      return PT_KEY_RBRACKET;
		case VK_OEM_COMMA:  return PT_KEY_COMMA;
		case VK_OEM_PERIOD: return PT_KEY_PERIOD;
		case VK_OEM_7:      return PT_KEY_QUOTE;
		case VK_OEM_5:      return PT_KEY_BACKSLASH;
		case VK_OEM_3:      return PT_KEY_TILDE;
		case VK_ESCAPE:     return PT_KEY_ESCAPE;
		case VK_SPACE:      return PT_KEY_SPACE;
		case VK_RETURN:     return PT_KEY_RETURN;
		case VK_BACK:       return PT_KEY_BACKSPACE;
		case VK_TAB:        return PT_KEY_TAB;
		case VK_PRIOR:      return PT_KEY_PAGEUP;
		case VK_NEXT:       return PT_KEY_PAGEDOWN;
		case VK_END:        return PT_KEY_END;
		case VK_HOME:       return PT_KEY_HOME;
		case VK_INSERT:     return PT_KEY_INSERT;
		case VK_DELETE:     return PT_KEY_DELETE;
		case VK_ADD:        return PT_KEY_ADD;
		case VK_SUBTRACT:   return PT_KEY_SUBTRACT;
		case VK_MULTIPLY:   return PT_KEY_MULTIPLY;
		case VK_DIVIDE:     return PT_KEY_DIVIDE;
		case VK_PAUSE:      return PT_KEY_PAUSE;
		case VK_F1:         return PT_KEY_F1;
		case VK_F2:         return PT_KEY_F2;
		case VK_F3:         return PT_KEY_F3;
		case VK_F4:         return PT_KEY_F4;
		case VK_F5:         return PT_KEY_F5;
		case VK_F6:         return PT_KEY_F6;
		case VK_F7:         return PT_KEY_F7;
		case VK_F8:         return PT_KEY_F8;
		case VK_F9:         return PT_KEY_F9;
		case VK_F10:        return PT_KEY_F10;
		case VK_F11:        return PT_KEY_F11;
		case VK_F12:        return PT_KEY_F12;
		case VK_F13:        return PT_KEY_F13;
		case VK_F14:        return PT_KEY_F14;
		case VK_F15:        return PT_KEY_F15;
		case VK_LEFT:       return PT_KEY_LEFT;
		case VK_RIGHT:      return PT_KEY_RIGHT;
		case VK_UP:         return PT_KEY_UP;
		case VK_DOWN:       return PT_KEY_DOWN;
		case VK_NUMPAD0:    return PT_KEY_NUMPAD0;
		case VK_NUMPAD1:    return PT_KEY_NUMPAD1;
		case VK_NUMPAD2:    return PT_KEY_NUMPAD2;
		case VK_NUMPAD3:    return PT_KEY_NUMPAD3;
		case VK_NUMPAD4:    return PT_KEY_NUMPAD4;
		case VK_NUMPAD5:    return PT_KEY_NUMPAD5;
		case VK_NUMPAD6:    return PT_KEY_NUMPAD6;
		case VK_NUMPAD7:    return PT_KEY_NUMPAD7;
		case VK_NUMPAD8:    return PT_KEY_NUMPAD8;
		case VK_NUMPAD9:    return PT_KEY_NUMPAD9;
		case 'A':           return PT_KEY_A;
		case 'Z':           return PT_KEY_Z;
		case 'E':           return PT_KEY_E;
		case 'R':           return PT_KEY_R;
		case 'T':           return PT_KEY_T;
		case 'Y':           return PT_KEY_Y;
		case 'U':           return PT_KEY_U;
		case 'I':           return PT_KEY_I;
		case 'O':           return PT_KEY_O;
		case 'P':           return PT_KEY_P;
		case 'Q':           return PT_KEY_Q;
		case 'S':           return PT_KEY_S;
		case 'D':           return PT_KEY_D;
		case 'F':           return PT_KEY_F;
		case 'G':           return PT_KEY_G;
		case 'H':           return PT_KEY_H;
		case 'J':           return PT_KEY_J;
		case 'K':           return PT_KEY_K;
		case 'L':           return PT_KEY_L;
		case 'M':           return PT_KEY_M;
		case 'W':           return PT_KEY_W;
		case 'X':           return PT_KEY_X;
		case 'C':           return PT_KEY_C;
		case 'V':           return PT_KEY_V;
		case 'B':           return PT_KEY_B;
		case 'N':           return PT_KEY_N;
		case '0':           return PT_KEY_NUM0;
		case '1':           return PT_KEY_NUM1;
		case '2':           return PT_KEY_NUM2;
		case '3':           return PT_KEY_NUM3;
		case '4':           return PT_KEY_NUM4;
		case '5':           return PT_KEY_NUM5;
		case '6':           return PT_KEY_NUM6;
		case '7':           return PT_KEY_NUM7;
		case '8':           return PT_KEY_NUM8;
		case '9':           return PT_KEY_NUM9;
	}

	return PT_KEY_UNKNOWN;
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_CLOSE) {
		return open = 0;
	}

	switch (msg) {
		case WM_KEYUP:
		case WM_SYSKEYUP: {
			if (key_up) {
				key_up(to_pt_key(wParam, lParam));
			}
			break;
		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN: {
			if (key_down) {
				key_down(to_pt_key(wParam, lParam));
			}
			break;
		}
		
		case WM_CHAR: {
			//if ((lParam & (1 << 30)) == 0) {
				char text[4];

				/* Convert to UTF-8 and send it on... */
				if (wParam <= 0x7F) {
					text[0] = (char)wParam;
					text[1] = '\0';
				} else if (wParam <= 0x7FF) {
					text[0] = 0xC0 | (char)((wParam >> 6) & 0x1F);
					text[1] = 0x80 | (char)(wParam & 0x3F);
					text[2] = '\0';
				} else {
					text[0] = 0xE0 | (char)((wParam >> 12) & 0x0F);
					text[1] = 0x80 | (char)((wParam >> 6) & 0x3F);
					text[2] = 0x80 | (char)(wParam & 0x3F);
					text[3] = '\0';
				}
				if ((unsigned char)*text < ' ' || *text == 127) {
					break;
				}
				
				if (text_input) {
					text_input(text);
				}
				break;
			//}
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

#endif