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

pt_thread* pt_thread_create(pt_thread_function function, void* param) {
	return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)function, param, 0, NULL);
}

void pt_thread_join(pt_thread* thread) {
	WaitForSingleObject(thread, INFINITE);
}

void pt_thread_terminate(pt_thread* thread) {
	TerminateThread(thread, 0);
}

void pt_thread_close(pt_thread* thread) {
	CloseHandle(thread);
}

int pt_thread_hardware_concurrency(void) {
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return (int)sysinfo.dwNumberOfProcessors;
}

#endif