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
#include <string.h>

#include <immintrin.h>
#include <intrin.h>
#include <emmintrin.h>

void pt_mat4_identity(pt_mat4* mat) {
	mat->m[0] = 1; mat->m[1] = 0; mat->m[2] = 0; mat->m[3] = 0;
	mat->m[4] = 0; mat->m[5] = 1; mat->m[6] = 0; mat->m[7] = 0;
	mat->m[8] = 0; mat->m[9] = 0; mat->m[10] = 1; mat->m[11] = 0;
	mat->m[12] = 0; mat->m[13] = 0; mat->m[14] = 0; mat->m[15] = 1;
}

void pt_mat4_scaling(pt_mat4* out, pt_scalar x, pt_scalar y, pt_scalar z) {
	out->m[0] = x; out->m[1] = 0; out->m[2] = 0; out->m[3] = 0;
	out->m[4] = 0; out->m[5] = y; out->m[6] = 0; out->m[7] = 0;
	out->m[8] = 0; out->m[9] = 0; out->m[10] = z; out->m[11] = 0;
	out->m[12] = 0; out->m[13] = 0; out->m[14] = 0; out->m[15] = 1;
}

void pt_mat4_rotation_x(pt_mat4* out, pt_scalar angle) {
	out->m[0] = 1; out->m[1] = 0; out->m[2] = 0; out->m[3] = 0;
	out->m[4] = 0; out->m[5] = cosf(angle); out->m[6] = sinf(angle); out->m[7] = 0;
	out->m[8] = 0; out->m[9] = -sinf(angle); out->m[10] = cosf(angle); out->m[11] = 0;
	out->m[12] = 0; out->m[13] = 0; out->m[14] = 0; out->m[15] = 1;
}

void pt_mat4_rotation_y(pt_mat4* out, pt_scalar angle) {
	out->m[0] = cosf(angle); out->m[1] = 0; out->m[2] = -sinf(angle); out->m[3] = 0;
	out->m[4] = 0; out->m[5] = 1; out->m[6] = 0; out->m[7] = 0;
	out->m[8] = sinf(angle); out->m[9] = 0; out->m[10] = cosf(angle); out->m[11] = 0;
	out->m[12] = 0; out->m[13] = 0; out->m[14] = 0; out->m[15] = 1;
}

void pt_mat4_rotation_z(pt_mat4* out, pt_scalar angle) {
	out->m[0] = cosf(angle); out->m[1] = sinf(angle); out->m[2] = 0; out->m[3] = 0;
	out->m[4] = -sinf(angle); out->m[5] = cosf(angle); out->m[6] = 0; out->m[7] = 0;
	out->m[8] = 0; out->m[9] = 0; out->m[10] = 1; out->m[11] = 0;
	out->m[12] = 0; out->m[13] = 0; out->m[14] = 0; out->m[15] = 1;
}

void pt_mat4_translation(pt_mat4* out, pt_scalar x, pt_scalar y, pt_scalar z) {
	out->m[0] = 1; out->m[1] = 0; out->m[2] = 0; out->m[3] = 0;
	out->m[4] = 0; out->m[5] = 1; out->m[6] = 0; out->m[7] = 0;
	out->m[8] = 0; out->m[9] = 0; out->m[10] = 1; out->m[11] = 0;
	out->m[12] = x; out->m[13] = y; out->m[14] = z; out->m[15] = 1;
}

void pt_mat4_perspective(pt_mat4* out, pt_scalar fovY, pt_scalar aspect, pt_scalar minZ, pt_scalar maxZ) {
	pt_scalar r = (fovY * 0.5f) * ((pt_scalar)PT_PI / 180.0f);

	pt_scalar deltaZ = minZ - maxZ;
	pt_scalar s = (pt_scalar)sin(r);
	pt_scalar cotangent = 0;

	if (deltaZ == 0 || s == 0 || aspect == 0) {
		pt_mat4_identity(out);
		return;
	}

	cotangent = (pt_scalar)cos(r) / s;

	out->m[0] = cotangent / aspect; out->m[1] = 0; out->m[2] = 0; out->m[3] = 0;
	out->m[4] = 0; out->m[5] = cotangent; out->m[6] = 0; out->m[7] = 0;
	out->m[8] = 0; out->m[9] = 0; out->m[10] = (maxZ + minZ) / deltaZ; out->m[11] = -1;
	out->m[12] = 0; out->m[13] = 0; out->m[14] = (2 * maxZ * minZ) / deltaZ; out->m[15] = 1;
}


static inline __m128 lincomb_SSE(const __m128 a, const pt_mat4* B)
{
	__m128 result;
	result = _mm_mul_ps(_mm_shuffle_ps(a, a, 0x00), B->row[0]);
	result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0x55), B->row[1]));
	result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0xaa), B->row[2]));
	result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a, a, 0xff), B->row[3]));
	return result;
}


#define SHUFFLE_PARAMS(x,y,z,w) 	((x) | ((y) << 2) | ((z) << 4) | ((w) << 6))

#define _mm_replicate_x_ps(v) 	_mm_shuffle_ps((v), (v), SHUFFLE_PARAMS(0,0,0,0))
#define _mm_replicate_y_ps(v) 	_mm_shuffle_ps((v), (v), SHUFFLE_PARAMS(1,1,1,1))
#define _mm_replicate_z_ps(v) 	_mm_shuffle_ps((v), (v), SHUFFLE_PARAMS(2,2,2,2))
#define _mm_replicate_w_ps(v) 	_mm_shuffle_ps((v), (v), SHUFFLE_PARAMS(3,3,3,3))
#define _mm_madd_ps(a, b, c)    _mm_add_ps(_mm_mul_ps((a), (b)), (c))

void pt_mat4_multiply(pt_mat4* out, pt_mat4* a, pt_mat4* b) {
#ifdef PT_SSE
	__m128 m1_row_0 = _mm_setr_ps(a->m[0], a->m[4], a->m[8], a->m[12]);
	__m128 m1_row_1 = _mm_setr_ps(a->m[1], a->m[5], a->m[9], a->m[13]);
	__m128 m1_row_2 = _mm_setr_ps(a->m[2], a->m[6], a->m[10], a->m[14]);
	__m128 m1_row_3 = _mm_setr_ps(a->m[3], a->m[7], a->m[11], a->m[15]);

	__m128 m2_row_0 = _mm_setr_ps(b->m[0], b->m[4], b->m[8], b->m[12]);
	__m128 m2_row_1 = _mm_setr_ps(b->m[1], b->m[5], b->m[9], b->m[13]);
	__m128 m2_row_2 = _mm_setr_ps(b->m[2], b->m[6], b->m[10], b->m[14]);
	__m128 m2_row_3 = _mm_setr_ps(b->m[3], b->m[7], b->m[11], b->m[15]);

	__m128 out0;
	__m128 out1;
	__m128 out2;
	__m128 out3;

	out0 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_0));
	out1 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_1));
	out2 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_2));
	out3 = _mm_mul_ps(m2_row_0, _mm_replicate_x_ps(m1_row_3));

	out0 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_0), out0);
	out1 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_1), out1);
	out2 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_2), out2);
	out3 = _mm_madd_ps(m2_row_1, _mm_replicate_y_ps(m1_row_3), out3);

	out0 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_0), out0);
	out1 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_1), out1);
	out2 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_2), out2);
	out3 = _mm_madd_ps(m2_row_2, _mm_replicate_z_ps(m1_row_3), out3);

	out0 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_0), out0);
	out1 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_1), out1);
	out2 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_2), out2);
	out3 = _mm_madd_ps(m2_row_3, _mm_replicate_w_ps(m1_row_3), out3);

	_mm_store_ps(&out->m[0], out0);
	_mm_store_ps(&out->m[4], out1);
	_mm_store_ps(&out->m[8], out2);
	_mm_store_ps(&out->m[12], out3);

	pt_mat4_transpose(out, out);
#else
	pt_scalar* m1 = a->m, *m2 = b->m;
	pt_mat4 temp;

	temp.m[0] = m1[0] * m2[0] + m1[4] * m2[1] + m1[8] * m2[2] + m1[12] * m2[3];
	temp.m[1] = m1[1] * m2[0] + m1[5] * m2[1] + m1[9] * m2[2] + m1[13] * m2[3];
	temp.m[2] = m1[2] * m2[0] + m1[6] * m2[1] + m1[10] * m2[2] + m1[14] * m2[3];
	temp.m[3] = m1[3] * m2[0] + m1[7] * m2[1] + m1[11] * m2[2] + m1[15] * m2[3];

	temp.m[4] = m1[0] * m2[4] + m1[4] * m2[5] + m1[8] * m2[6] + m1[12] * m2[7];
	temp.m[5] = m1[1] * m2[4] + m1[5] * m2[5] + m1[9] * m2[6] + m1[13] * m2[7];
	temp.m[6] = m1[2] * m2[4] + m1[6] * m2[5] + m1[10] * m2[6] + m1[14] * m2[7];
	temp.m[7] = m1[3] * m2[4] + m1[7] * m2[5] + m1[11] * m2[6] + m1[15] * m2[7];

	temp.m[8] = m1[0] * m2[8] + m1[4] * m2[9] + m1[8] * m2[10] + m1[12] * m2[11];
	temp.m[9] = m1[1] * m2[8] + m1[5] * m2[9] + m1[9] * m2[10] + m1[13] * m2[11];
	temp.m[10] = m1[2] * m2[8] + m1[6] * m2[9] + m1[10] * m2[10] + m1[14] * m2[11];
	temp.m[11] = m1[3] * m2[8] + m1[7] * m2[9] + m1[11] * m2[10] + m1[15] * m2[11];

	temp.m[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8] * m2[14] + m1[12] * m2[15];
	temp.m[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9] * m2[14] + m1[13] * m2[15];
	temp.m[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
	temp.m[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

	memcpy(out->m, temp.m, sizeof(pt_mat4));
#endif
}

void pt_mat4_multiply_vec4(pt_vec4* out, pt_mat4* a, pt_vec4* b) {
	pt_vec4_assign(out,
		b->x * a->m[0] + b->y * a->m[4] + b->z * a->m[8] + b->w * a->m[12],
		b->x * a->m[1] + b->y * a->m[5] + b->z * a->m[9] + b->w * a->m[13],
		b->x * a->m[2] + b->y * a->m[6] + b->z * a->m[10] + b->w * a->m[14],
		b->x * a->m[3] + b->y * a->m[7] + b->z * a->m[11] + b->w * a->m[15]);
}

void pt_mat4_transpose(pt_mat4* out, pt_mat4* mat) {
	pt_mat4 tmp;
	tmp.m[0] = mat->m[0];  tmp.m[1] = mat->m[4];  tmp.m[2] = mat->m[8]; tmp.m[3] = mat->m[12];
	tmp.m[4] = mat->m[1];  tmp.m[5] = mat->m[5];  tmp.m[6] = mat->m[9]; tmp.m[7] = mat->m[13];
	tmp.m[8] = mat->m[2];  tmp.m[9] = mat->m[6];  tmp.m[10] = mat->m[10]; tmp.m[11] = mat->m[14];
	tmp.m[12] = mat->m[3]; tmp.m[13] = mat->m[7]; tmp.m[14] = mat->m[11]; tmp.m[15] = mat->m[15];
	memcpy(out->m, tmp.m, sizeof(pt_mat4));
}