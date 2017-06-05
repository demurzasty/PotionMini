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
#ifndef PT_VECTOR4_H
#define PT_VECTOR4_H

typedef struct pt_vec4 {
	pt_scalar x;
	pt_scalar y;
	pt_scalar z;
	pt_scalar w;
} pt_vec4;

void pt_vec4_assign(pt_vec4* out, pt_scalar x, pt_scalar y, pt_scalar z, pt_scalar w);

void pt_vec4_assign_vec3(pt_vec4* out, pt_vec3* v, pt_scalar w);

void pt_vec4_neg(pt_vec4* out, pt_vec4* v);

void pt_vec4_add(pt_vec4* out, pt_vec4* a, pt_vec4* b);

void pt_vec4_subtract(pt_vec4* out, pt_vec4* a, pt_vec4* b);

void pt_vec4_multiply(pt_vec4* out, pt_vec4* a, pt_vec4* b);

void pt_vec4_divide(pt_vec4* out, pt_vec4* a, pt_vec4* b);

void pt_vec4_add_scalar(pt_vec4* out, pt_vec4* a, pt_scalar b);

void pt_vec4_subtract_scalar(pt_vec4* out, pt_vec4* a, pt_scalar b);

void pt_vec4_multiply_scalar(pt_vec4* out, pt_vec4* a, pt_scalar b);

void pt_vec4_divide_scalar(pt_vec4* out, pt_vec4* a, pt_scalar b);

pt_scalar pt_vec4_length(pt_vec4* v);

void pt_vec4_normalize(pt_vec4* out, pt_vec4* v);

void pt_vec4_direction(pt_vec4* out, pt_vec4* a, pt_vec4* b);

pt_scalar pt_vec4_distance(pt_vec4* a, pt_vec4* b);

pt_scalar pt_vec4_dot(pt_vec4* a, pt_vec4* b);

void pt_vec4_cross(pt_vec4* out, pt_vec4* a, pt_vec4* b);

#endif