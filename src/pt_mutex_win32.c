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
#include <stdlib.h>

struct pt_mutex {
	CRITICAL_SECTION section;
};

pt_mutex* pt_mutex_create(void) {
	pt_mutex* mutex = malloc(sizeof(pt_mutex));

	InitializeCriticalSection(&mutex->section);

	return mutex;
}

void pt_mutex_lock(pt_mutex* mutex) {
	EnterCriticalSection(&mutex->section);
}

void pt_mutex_unlock(pt_mutex* mutex) {
	LeaveCriticalSection(&mutex->section);
}

void pt_mutex_destroy(pt_mutex* mutex) {
	DeleteCriticalSection(&mutex->section);
	free(mutex);
}

#endif