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

#ifndef PT_MATRIX4_H
#define PT_MATRIX4_H

#include <immintrin.h>

typedef struct pt_mat4 {
	union {
		pt_scalar m[16];
		__m128 row[4];
	};
} pt_mat4;

void pt_mat4_identity(pt_mat4* mat);

void pt_mat4_scaling(pt_mat4* out, pt_scalar x, pt_scalar y, pt_scalar z);

void pt_mat4_rotation_x(pt_mat4* out, pt_scalar angle);

void pt_mat4_rotation_y(pt_mat4* out, pt_scalar angle);

void pt_mat4_rotation_z(pt_mat4* out, pt_scalar angle);

void pt_mat4_translation(pt_mat4* out, pt_scalar x, pt_scalar y, pt_scalar z);

void pt_mat4_perspective(pt_mat4* out, pt_scalar fovY, pt_scalar aspect, pt_scalar minZ, pt_scalar maxZ);

void pt_mat4_multiply(pt_mat4* out, pt_mat4* a, pt_mat4* b);

void pt_mat4_multiply_vec4(pt_vec4* out, pt_mat4* a, pt_vec4* b);

void pt_mat4_transpose(pt_mat4* out, pt_mat4* mat);

#endif