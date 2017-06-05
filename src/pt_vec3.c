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

void pt_vec3_assign(pt_vec3* out, pt_scalar x, pt_scalar y, pt_scalar z) {
	out->x = x;
	out->y = y;
	out->z = z;
}

void pt_vec3_neg(pt_vec3* out, pt_vec3* v) {
	out->x = -v->x;
	out->y = -v->y;
	out->z = -v->z;
}

void pt_vec3_add(pt_vec3* out, pt_vec3* a, pt_vec3* b) {
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
}

void pt_vec3_subtract(pt_vec3* out, pt_vec3* a, pt_vec3* b) {
	out->x = a->x - b->x;
	out->y = a->y - b->y;
	out->z = a->z - b->z;
}

void pt_vec3_multiply(pt_vec3* out, pt_vec3* a, pt_vec3* b) {
	out->x = a->x * b->x;
	out->y = a->y * b->y;
	out->z = a->z * b->z;
}

void pt_vec3_divide(pt_vec3* out, pt_vec3* a, pt_vec3* b) {
	out->x = a->x / b->x;
	out->y = a->y / b->y;
	out->z = a->z / b->z;
}

void pt_vec3_add_scalar(pt_vec3* out, pt_vec3* a, pt_scalar b) {
	out->x = a->x + b;
	out->y = a->y + b;
	out->z = a->z + b;
}

void pt_vec3_subtract_scalar(pt_vec3* out, pt_vec3* a, pt_scalar b) {
	out->x = a->x - b;
	out->y = a->y - b;
	out->z = a->z - b;
}

void pt_vec3_multiply_scalar(pt_vec3* out, pt_vec3* a, pt_scalar b) {
	out->x = a->x * b;
	out->y = a->y * b;
	out->z = a->z * b;
}

void pt_vec3_divide_scalar(pt_vec3* out, pt_vec3* a, pt_scalar b) {
	out->x = a->x / b;
	out->y = a->y / b;
	out->z = a->z / b;
}

pt_scalar pt_vec3_length(pt_vec3* v) {
	return (pt_scalar)sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

void pt_vec3_normalize(pt_vec3* out, pt_vec3* v) {
	pt_scalar length = (pt_scalar)1.0 / pt_vec3_length(v);

	out->x = v->x * length;
	out->y = v->y * length;
	out->z = v->z * length;
}

void pt_vec3_direction(pt_vec3* out, pt_vec3* a, pt_vec3* b) {
	pt_vec3_subtract(out, b, a);
	pt_vec3_normalize(out, out);
}

pt_scalar pt_vec3_distance(pt_vec3* a, pt_vec3* b) {
	pt_vec3 out;
	pt_vec3_subtract(&out, b, a);
	return pt_vec3_length(&out);
}

pt_scalar pt_vec3_dot(pt_vec3* a, pt_vec3* b) {
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

void pt_vec3_cross(pt_vec3* out, pt_vec3* a, pt_vec3* b) {
	out->x = a->y * b->z - a->z * b->y;
	out->y = a->z * b->x - a->x * b->z;
	out->z = a->x * b->y - a->y * b->x;
}