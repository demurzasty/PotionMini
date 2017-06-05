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
#ifndef PT_VECTOR2_H
#define PT_VECTOR2_H

typedef struct pt_vec2 {
	pt_scalar x;
	pt_scalar y;
} pt_vec2;

void pt_vec2_assign(pt_vec2* out, pt_scalar x, pt_scalar y);

void pt_vec2_neg(pt_vec2* out, pt_vec2* v);

void pt_vec2_add(pt_vec2* out, pt_vec2* a, pt_vec2* b);

void pt_vec2_subtract(pt_vec2* out, pt_vec2* a, pt_vec2* b);

void pt_vec2_multiply(pt_vec2* out, pt_vec2* a, pt_vec2* b);

void pt_vec2_divide(pt_vec2* out, pt_vec2* a, pt_vec2* b);

void pt_vec2_add_scalar(pt_vec2* out, pt_vec2* a, pt_scalar b);

void pt_vec2_subtract_scalar(pt_vec2* out, pt_vec2* a, pt_scalar b);

void pt_vec2_multiply_scalar(pt_vec2* out, pt_vec2* a, pt_scalar b);

void pt_vec2_divide_scalar(pt_vec2* out, pt_vec2* a, pt_scalar b);

pt_scalar pt_vec2_length(pt_vec2* v);

void pt_vec2_normalize(pt_vec2* out, pt_vec2* v);

void pt_vec2_direction(pt_vec2* out, pt_vec2* a, pt_vec2* b);

pt_scalar pt_vec2_distance(pt_vec2* a, pt_vec2* b);

pt_scalar pt_vec2_dot(pt_vec2* a, pt_vec2* b);

pt_scalar pt_vec2_cross(pt_vec2* a, pt_vec2* b);

#endif