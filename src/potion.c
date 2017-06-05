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

int pt_init(int width, int height) {
	if (!pt_win_init(width, height)) {
		return 0;
	}

	if (!pt_renderer_init(width, height)) {
		return 0;
	}

	return 1;
}

void pt_release(void) {
	pt_renderer_release();
	pt_win_release();
}