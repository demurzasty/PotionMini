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
#ifndef PT_VECTOR3_H
#define PT_VECTOR3_H

typedef struct pt_vec3 {
	pt_scalar x;
	pt_scalar y;
	pt_scalar z;
} pt_vec3;

void pt_vec3_assign(pt_vec3* out, pt_scalar x, pt_scalar y, pt_scalar z);

void pt_vec3_neg(pt_vec3* out, pt_vec3* v);

void pt_vec3_add(pt_vec3* out, pt_vec3* a, pt_vec3* b);

void pt_vec3_subtract(pt_vec3* out, pt_vec3* a, pt_vec3* b);

void pt_vec3_multiply(pt_vec3* out, pt_vec3* a, pt_vec3* b);

void pt_vec3_divide(pt_vec3* out, pt_vec3* a, pt_vec3* b);

void pt_vec3_add_scalar(pt_vec3* out, pt_vec3* a, pt_scalar b);

void pt_vec3_subtract_scalar(pt_vec3* out, pt_vec3* a, pt_scalar b);

void pt_vec3_multiply_scalar(pt_vec3* out, pt_vec3* a, pt_scalar b);

void pt_vec3_divide_scalar(pt_vec3* out, pt_vec3* a, pt_scalar b);

pt_scalar pt_vec3_length(pt_vec3* v);

void pt_vec3_normalize(pt_vec3* out, pt_vec3* v);

void pt_vec3_direction(pt_vec3* out, pt_vec3* a, pt_vec3* b);

pt_scalar pt_vec3_distance(pt_vec3* a, pt_vec3* b);

pt_scalar pt_vec3_dot(pt_vec3* a, pt_vec3* b);

void pt_vec3_cross(pt_vec3* out, pt_vec3* a, pt_vec3* b);


#endif