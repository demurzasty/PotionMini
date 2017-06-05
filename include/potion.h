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
#ifndef POTION_H
#define POTION_H

#ifdef _WIN32
#	define PT_WINDOWS
#else
#	define PT_UNIX
#endif

#define PT_SSE 

#define PT_SAFE_FREE(x) do { if (x) { free(x); x = 0; } } while (0)

#include "pt_math.h"
#include "pt_vec2.h"
#include "pt_vec3.h"
#include "pt_vec4.h"
#include "pt_mat4.h"
#include "pt_color.h"
#include "pt_vertex.h"
#include "pt_mesh.h"
#include "pt_texture.h"
#include "pt_time.h"
#include "pt_thread.h"
#include "pt_mutex.h"
#include "pt_key.h"
#include "pt_window.h"
#include "pt_renderer.h"

int pt_init(int width, int height);

void pt_release(void);

#endif