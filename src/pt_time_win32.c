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

static
LARGE_INTEGER get_frequency() {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	return frequency;
}

static
unsigned long long get_current_time(LARGE_INTEGER frequency) {
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	return 1000000 * time.QuadPart / frequency.QuadPart;
}

double pt_get_time(void) {
	LARGE_INTEGER frequency = get_frequency();
	//mutex.lock();

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	//mutex.unlock();

	return ((1000000 * time.QuadPart / frequency.QuadPart) * 0.000001);
}

void pt_sleep(unsigned int ms) {
	Sleep(ms);
}

#endif