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
#ifndef PT_WINDOW_H
#define PT_WINDOW_H

typedef void (*pt_key_callback)(int key);
typedef void (*pt_text_callback)(const char* text);

int pt_win_init(int width, int height);

int pt_win_is_open(void);

void pt_win_close(void);

void pt_win_poll_events(void);

int pt_win_has_focus(void);

void pt_win_get_size(int* width, int* height);

void pt_win_set_size(int width, int height);

void pt_win_update(pt_color4b* pixels);

void pt_win_set_key_down_callback(pt_key_callback callback);

void pt_win_set_key_up_callback(pt_key_callback callback);

void pt_win_set_text_callback(pt_text_callback callback);

void pt_win_release(void);

#endif