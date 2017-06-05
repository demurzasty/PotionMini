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
#ifndef PT_THREAD_H
#define PT_THREAD_H

#ifdef PT_WINDOWS
typedef void pt_thread;
#else
typedef struct pt_thread pt_thread;
#endif

typedef (__stdcall *pt_thread_function)(void*);

pt_thread* pt_thread_create(pt_thread_function function, void* param);

void pt_thread_join(pt_thread* thread);

void pt_thread_terminate(pt_thread* thread);

void pt_thread_close(pt_thread* thread);

int pt_thread_hardware_concurrency(void);

#endif