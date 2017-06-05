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

#include <math.h>

void pt_vec4_assign(pt_vec4* out, pt_scalar x, pt_scalar y, pt_scalar z, pt_scalar w) {
	out->x = x;
	out->y = y;
	out->z = z;
	out->w = w;
}

void pt_vec4_assign_vec3(pt_vec4* out, pt_vec3* v, pt_scalar w) {
	out->x = v->x;
	out->y = v->y;
	out->z = v->z; 
	out->w = w;
}

void pt_vec4_neg(pt_vec4* out, pt_vec4* v) {
	out->x = -v->x;
	out->y = -v->y;
	out->z = -v->z;
	out->w = -v->w;
}

void pt_vec4_add(pt_vec4* out, pt_vec4* a, pt_vec4* b) {
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
	out->w = a->w + b->w;
}

void pt_vec4_subtract(pt_vec4* out, pt_vec4* a, pt_vec4* b) {
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
	out->w = a->w - b->w;
}

void pt_vec4_multiply(pt_vec4* out, pt_vec4* a, pt_vec4* b) {
	out->x = a->x * b->x;
	out->y = a->y * b->y;
	out->z = a->z * b->z;
	out->w = a->w * b->w;
}

void pt_vec4_divide(pt_vec4* out, pt_vec4* a, pt_vec4* b) {
	out->x = a->x / b->x;
	out->y = a->y / b->y;
	out->z = a->z / b->z;
	out->w = a->w / b->w;
}

void pt_vec4_add_scalar(pt_vec4* out, pt_vec4* a, pt_scalar b) {
	out->x = a->x + b;
	out->y = a->y + b;
	out->z = a->z + b;
	out->w = a->w + b;
}

void pt_vec4_subtract_scalar(pt_vec4* out, pt_vec4* a, pt_scalar b) {
	out->x = a->x - b;
	out->y = a->y - b;
	out->z = a->z - b;
	out->w = a->w - b;
}

void pt_vec4_multiply_scalar(pt_vec4* out, pt_vec4* a, pt_scalar b) {
	out->x = a->x * b;
	out->y = a->y * b;
	out->z = a->z * b;
	out->w = a->w * b;
}

void pt_vec4_divide_scalar(pt_vec4* out, pt_vec4* a, pt_scalar b) {
	out->x = a->x / b;
	out->y = a->y / b;
	out->z = a->z / b;
	out->w = a->w / b;
}

pt_scalar pt_vec4_length(pt_vec4* v) {
	return (pt_scalar)sqrt(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

void pt_vec4_normalize(pt_vec4* out, pt_vec4* v) {
	pt_scalar length = (pt_scalar)1.0 / pt_vec4_length(v);

	out->x = v->x * length;
	out->y = v->y * length;
	out->z = v->z * length;
	out->w = v->w * length;
}

void pt_vec4_direction(pt_vec4* out, pt_vec4* a, pt_vec4* b) {
	pt_vec4_subtract(out, b, a);
	pt_vec4_normalize(out, out);
}

pt_scalar pt_vec4_distance(pt_vec4* a, pt_vec4* b) {
	pt_vec4 out;
	pt_vec4_subtract(&out, b, a);
	return pt_vec4_length(&out);
}

pt_scalar pt_vec4_dot(pt_vec4* a, pt_vec4* b) {
	return a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w;
}

void pt_vec4_cross(pt_vec4* out, pt_vec4* a, pt_vec4* b) {
	out->x = a->y * b->z - a->z * b->y;
	out->y = a->z * b->x - a->x * b->z;
	out->z = a->x * b->y - a->y * b->x;
}