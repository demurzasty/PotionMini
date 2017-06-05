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

#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <assert.h>
#include <math.h>

#define RB_RENDER_SCANLINE    0
#define RB_RENDER_BARYCENTRIC 1
#define RB_RENDER_BRASENHAM   2

#define RB_RENDER_METHOD RB_RENDER_BARYCENTRIC

#define swap_int(a, b) (a ^= b, b ^= a, a ^= b)

static struct Framebuffer {
	pt_color4b*     color;
	pt_color4b*     backbuffer;
	unsigned short* depth;
	int             width;
	int             height;
	int             pixelCount;
} framebuffer;

static unsigned int render_flags = PT_RENDERER_DEFAULT;
static pt_mat4 world;
static pt_mat4 view;
static pt_mat4 proj;
static pt_mat4 proj_view;

static pt_vec3 light_dir;

static pt_texture* texture = 0;
static pt_texture* normal_map = 0;
static pt_texture* shadow_map = 0;
static pt_scalar ambient = 0.1f;

static int         threadCount = 0;
static pt_thread** threads = 0;
static int*        thread_states = 0;
static pt_mutex*   mutex = 0;

static pt_mesh* current_mesh = 0;
static int running = 0;
static int tileSize = 0;

static
void pt_renderer_draw_triangle_backend(pt_vertex* v0, pt_vertex* v1, pt_vertex* v2, unsigned int flags, int tile);

static
void pt_renderer_draw_mesh_backend(pt_mesh* mesh, int tile);

static 
int __stdcall background(void* param) {
	int num = (int)param;

	while (running) {

		if (current_mesh && thread_states[num] == 0) {
			thread_states[num] = 1;
			pt_renderer_draw_mesh_backend(current_mesh, num + 1);
			thread_states[num] = 2;
		}

		pt_sleep(1);
	}

	return 1;
}

int pt_renderer_init(int width, int height) {
	memset(&framebuffer, 0, sizeof(framebuffer));

	pt_renderer_set_size(width, height);

	pt_mat4_identity(&world);
	pt_mat4_identity(&view);
	pt_mat4_identity(&proj);

	running = 1;

	mutex = pt_mutex_create();

	int processors = pt_thread_hardware_concurrency();
	threadCount = (processors - 1);
	if (threadCount > 0) {
		tileSize = framebuffer.height / processors;

		threads = malloc(threadCount * sizeof(pt_thread*));
		thread_states = malloc(threadCount * sizeof(int));

		int i;
		for (i = 0; i < threadCount; i++) {
			thread_states[i] = 0;
			threads[i] = pt_thread_create(&background, (void*)i);
		}
	}

	pt_vec3_assign(&light_dir, -1.0f, -1.0f, 1.0f);
	pt_vec3_normalize(&light_dir, &light_dir);

	return 1;
}

void pt_renderer_set_size(int width, int height) {
	PT_SAFE_FREE(framebuffer.color);
	PT_SAFE_FREE(framebuffer.backbuffer);
	PT_SAFE_FREE(framebuffer.depth);

	framebuffer.color = malloc(width * height * sizeof(pt_color4b));
	framebuffer.backbuffer = malloc(width * height * sizeof(pt_color4b));
	framebuffer.depth = malloc(width * height * sizeof(unsigned short));
	framebuffer.width = width;
	framebuffer.height = height;
	framebuffer.pixelCount = width * height;
}

void pt_renderer_release(void) {
	running = 0;

	int i;
	for (i = 0; i < threadCount; i++) {
		pt_thread_join(threads[i]);
		pt_thread_close(threads[i]);
	}
	PT_SAFE_FREE(threads);
	PT_SAFE_FREE(thread_states);

	pt_mutex_destroy(mutex);

	free(framebuffer.depth);
	free(framebuffer.color);
	free(framebuffer.backbuffer);
}

void pt_renderer_clear(void) {
	memset(framebuffer.color, 0, framebuffer.pixelCount * sizeof(pt_color4b));
	memset(framebuffer.depth, 0, framebuffer.pixelCount * sizeof(*framebuffer.depth));
}

void pt_renderer_put_pixel(int x, int y, int z, int r, int g, int b, int a) {
	pt_color4b* pixel;
	int index;
	int depth;

	if (x < 0 || y < 0 || x >= framebuffer.width || y >= framebuffer.height || z < 0 || z >= 0xFFFF) {
		return;
	}

	index = y * framebuffer.width + x;

	depth = framebuffer.depth[index];

	if ((render_flags & PT_RENDERER_DEPTH_TEST) && z < depth) {
		return;
	}

	framebuffer.depth[index] = z;

	pixel = &framebuffer.color[index];

	pixel->red   = r;
	pixel->green = g;
	pixel->blue  = b;
	pixel->alpha = a;
}

void pt_renderer_draw_line(int x1, int y1, int z1, int x2, int y2, int z2) {
	int d, dx, dy, ai, bi, xi, yi;
	int x = x1, y = y1;

	if (x1 < x2) {
		xi = 1;
		dx = x2 - x1;
	}
	else {
		xi = -1;
		dx = x1 - x2;
	}

	if (y1 < y2) {
		yi = 1;
		dy = y2 - y1;
	}
	else {
		yi = -1;
		dy = y1 - y2;
	}

	pt_renderer_put_pixel(x, y, z1, 255, 255, 255, 255);

	if (dx > dy) {
		ai = (dy - dx) << 1;
		bi = dy << 1;
		d = bi - dx;

		while (x != x2) {
			if (d >= 0) {
				x += xi;
				y += yi;
				d += ai;
			}
			else {
				d += bi;
				x += xi;
			}
			pt_renderer_put_pixel(x, y, z1, 255, 255, 255, 255);
		}
	}
	else {
		ai = (dx - dy) << 1;
		bi = dx << 1;
		d = bi - dy;

		while (y != y2) {
			if (d >= 0) {
				x += xi;
				y += yi;
				d += ai;
			}
			else {
				d += bi;
				y += yi;
			}
			pt_renderer_put_pixel(x, y, z1, 255, 255, 255, 255);
		}
	}
}

void pt_renderer_draw_horizontal_line(int y, int x1, int z1, int x2, int z2) {
	int x;

	if (x1 > x2) {
		swap_int(x1, x2);
	}

	x1 = PT_MIN(framebuffer.width, PT_MAX(x1, 0));
	x2 = PT_MIN(framebuffer.width, PT_MAX(x2, 0));

	for (x = x1; x < x2; x++) {
		pt_renderer_put_pixel(x, y, z1, 255, 255, 255, 255);
	}
}

#define edge(x0, y0, x1, y1, x2, y2) ( (x1 - x0) * -(y2 - y0) - -(y1 - y0) * (x2 - x0) )

#define counterclockwise(x0, y0, x1, y1, x2, y2)  (((x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0)) >= 0)
#define counterclockwisef(x0, y0, x1, y1, x2, y2) (((x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0)) >= 0.0f)

static
void pt_renderer_draw_triangle_backend(pt_vertex* v0, pt_vertex* v1, pt_vertex* v2, unsigned int flags, int tile) {
	pt_vertex vertices[3];
	vertices[0] = *v0;
	vertices[1] = *v1;
	vertices[2] = *v2;
	
	pt_mat4 matrix, tbn;
	// pt_mat4_multiply(&matrix, &proj, &view);
	pt_mat4_multiply(&matrix, &proj_view, &world);

	pt_vec4 v4a, v4b, v4c;
	pt_vec4_assign_vec3(&v4a, &vertices[0].position, 1.0f);
	pt_vec4_assign_vec3(&v4b, &vertices[1].position, 1.0f);
	pt_vec4_assign_vec3(&v4c, &vertices[2].position, 1.0f);

	pt_mat4_multiply_vec4(&v4a, &matrix, &v4a);
	pt_mat4_multiply_vec4(&v4b, &matrix, &v4b);
	pt_mat4_multiply_vec4(&v4c, &matrix, &v4c);

	v4a.w = 1.0f / v4a.w;
	v4b.w = 1.0f / v4b.w;
	v4c.w = 1.0f / v4c.w;

	pt_vec3_assign(&vertices[0].position, v4a.x * v4a.w, v4a.y * v4a.w, v4a.z * v4a.w);
	pt_vec3_assign(&vertices[1].position, v4b.x * v4b.w, v4b.y * v4b.w, v4b.z * v4b.w);
	pt_vec3_assign(&vertices[2].position, v4c.x * v4c.w, v4c.y * v4c.w, v4c.z * v4c.w);
	
	if ((PT_ABS(vertices[0].position.x) > 1.0f || PT_ABS(vertices[0].position.y) > 1.0f || PT_ABS(vertices[0].position.z) > 1.0f) &&
		(PT_ABS(vertices[1].position.x) > 1.0f || PT_ABS(vertices[1].position.y) > 1.0f || PT_ABS(vertices[1].position.z) > 1.0f) &&
		(PT_ABS(vertices[2].position.x) > 1.0f || PT_ABS(vertices[2].position.y) > 1.0f || PT_ABS(vertices[2].position.z) > 1.0f)) {
		return;
	}

	int x0 = (int)((vertices[0].position.x * 0.5f + 0.5f) * framebuffer.width);
	int y0 = (int)((1.0f - (vertices[0].position.y * 0.5f + 0.5f)) * framebuffer.height);

	int x1 = (int)((vertices[1].position.x * 0.5f + 0.5f) * framebuffer.width);
	int y1 = (int)((1.0f - (vertices[1].position.y * 0.5f + 0.5f)) * framebuffer.height);

	int x2 = (int)((vertices[2].position.x * 0.5f + 0.5f) * framebuffer.width);
	int y2 = (int)((1.0f - (vertices[2].position.y * 0.5f + 0.5f)) * framebuffer.height);

	if ((render_flags & PT_RENDERER_CULL_FACE) && counterclockwise(x0, y0, x1, y1, x2, y2)) {
		return;
	}

	if (render_flags & PT_RENDERER_WIREFRAME) {
		pt_renderer_draw_line(x0, y0, 0, x1, y1, 0);
		pt_renderer_draw_line(x1, y1, 0, x2, y2, 0);
		pt_renderer_draw_line(x2, y2, 0, x0, y0, 0);
		return;
	}

	int startY = tile * tileSize;
	int endY   = startY + tileSize;

	int maxX = PT_MAX(x0, PT_MAX(x1, x2));
	int minX = PT_MIN(x0, PT_MIN(x1, x2));
	int maxY = PT_MAX(y0, PT_MAX(y1, y2));
	int minY = PT_MIN(y0, PT_MIN(y1, y2));

	minX = PT_MAX(minX, 0);
	minY = PT_MAX(minY, 0);

	maxX = PT_MIN(maxX, framebuffer.width - 1);
	maxY = PT_MIN(maxY, framebuffer.height - 1);

	if (render_flags & PT_RENDERER_MULTITHREADED) {
		minY = PT_MAX(minY, startY);
		maxY = PT_MIN(maxY, endY);
	}


	if (maxY < minY || maxX < minX) {
		return;
	}

	vertices[0].position.z = 1.0f - (vertices[0].position.z * 0.5f + 0.5f);
	vertices[1].position.z = 1.0f - (vertices[1].position.z * 0.5f + 0.5f);
	vertices[2].position.z = 1.0f - (vertices[2].position.z * 0.5f + 0.5f);

	if ((render_flags & PT_RENDERER_SHADING) && flags & PT_VERTEX_NORMAL) {
		pt_vec4_assign_vec3(&v4a, &vertices[0].normal, 0.0f);
		pt_vec4_assign_vec3(&v4b, &vertices[1].normal, 0.0f);
		pt_vec4_assign_vec3(&v4c, &vertices[2].normal, 0.0f);

		pt_mat4_multiply_vec4(&v4a, &matrix, &v4a);
		pt_mat4_multiply_vec4(&v4b, &matrix, &v4b);
		pt_mat4_multiply_vec4(&v4c, &matrix, &v4c);

		vertices[0].normal = *(pt_vec3*)&v4a;
		vertices[1].normal = *(pt_vec3*)&v4b;
		vertices[2].normal = *(pt_vec3*)&v4c;

		pt_vec3_normalize(&vertices[0].normal, &vertices[0].normal);
		pt_vec3_normalize(&vertices[1].normal, &vertices[1].normal);
		pt_vec3_normalize(&vertices[2].normal, &vertices[2].normal);
	}

	int area = edge(x0, y0, x1, y1, x2, y2);
	pt_scalar inv_area = 1.0f / area;

	int w0, w1;
	float a, b, c;
	int x, y;
	pt_scalar u, v;
	pt_color4b color;
	pt_vec3 normal;

	pt_vec3 output;

	int depth, index;

	pt_scalar z, ndotl;
	for (y = minY; y <= maxY; ++y) {
		index = y * framebuffer.width;
		for (x = minX; x <= maxX; ++x) {
			w0 = edge(x1, y1, x2, y2, x, y);
			w1 = edge(x2, y2, x0, y0, x, y);

			a = w0 * inv_area;
			b = w1 * inv_area;
			c = (1.0f - a - b);

			if (a < -FLT_EPSILON || b < -FLT_EPSILON || c < -FLT_EPSILON) {
				continue;
			}

			z = vertices[0].position.z * a + vertices[1].position.z * b + vertices[2].position.z * c;
			depth = (int)(z * 0xFFFF);

			if (render_flags & PT_RENDERER_DEPTH_TEST) {
				if (index >= 0 && index < framebuffer.pixelCount && depth < framebuffer.depth[index + x]) {
					continue;
				}
			}

			output.x = output.y = output.z = 1.0f;

			if ((render_flags & PT_RENDERER_SHADING) && (flags & PT_VERTEX_NORMAL)) {
				normal.x = vertices[0].normal.x * a + vertices[1].normal.x * b + vertices[2].normal.x * c;
				normal.y = vertices[0].normal.y * a + vertices[1].normal.y * b + vertices[2].normal.y * c;
				normal.z = vertices[0].normal.z * a + vertices[1].normal.z * b + vertices[2].normal.z * c;


				pt_vec3_neg(&normal, &normal);
				ndotl = pt_vec3_dot(&normal, &light_dir);
				ndotl = PT_MAX(ndotl, 0.0f);

				pt_vec3_multiply_scalar(&output, &output, ndotl);
			}

			if ((render_flags & PT_RENDERER_TEXTURING) && (flags & PT_VERTEX_TEXCOORD) && texture) {

				u = vertices[0].texcoord.x * a + vertices[1].texcoord.x * b + vertices[2].texcoord.x * c;
				v = vertices[0].texcoord.y * a + vertices[1].texcoord.y * b + vertices[2].texcoord.y * c;

				pt_texture_sample(texture, u, v, &color);

				output.x *= color.red * 0.00392156f;
				output.y *= color.green * 0.00392156f;
				output.z *= color.blue * 0.00392156f;
			}

			pt_vec3_multiply_scalar(&output, &output, 1.0f - ambient);
			pt_vec3_add_scalar(&output, &output, ambient);

			color.red = (unsigned char)(output.x * 255);
			color.green = (unsigned char)(output.y * 255);
			color.blue = (unsigned char)(output.z * 255);


			pt_renderer_put_pixel(x, y, depth, color.red, color.green, color.blue, 255);
		}
	}
}

void pt_renderer_draw_triangle(pt_vertex* v0, pt_vertex* v1, pt_vertex* v2, unsigned int flags) {
	pt_renderer_draw_triangle_backend(v0, v1, v2, flags, 0);
}

static
void pt_renderer_draw_mesh_backend(pt_mesh* mesh, int tile) {
	unsigned int i;
	if (mesh->indices) {
		for (i = 0; i < mesh->indexCount; i += 3) {
			pt_renderer_draw_triangle_backend(&mesh->vertices[mesh->indices[i]],
				&mesh->vertices[mesh->indices[i + 1]],
				&mesh->vertices[mesh->indices[i + 2]],
				mesh->vertexType, tile);
		}
	} else {
		for (i = 0; i < mesh->vertexCount; i += 3) {
			pt_renderer_draw_triangle_backend(&mesh->vertices[i],
				&mesh->vertices[i + 1],
				&mesh->vertices[i + 2],
				mesh->vertexType, tile);
		}
	}
}

void pt_renderer_draw_mesh(pt_mesh* mesh) {
	int i;

	if (render_flags & PT_RENDERER_MULTITHREADED) {
		current_mesh = mesh;

		/* wait until every thread start working */
		for (i = 0; i < threadCount; i++) {
			while (thread_states[i] == 0);
		}
	}

	pt_renderer_draw_mesh_backend(mesh, 0);


	if (render_flags & PT_RENDERER_MULTITHREADED) {
		current_mesh = 0;

		for (i = 0; i < threadCount; i++) {
			while (thread_states[i] != 2);
			thread_states[i] = 0;
		}
	}
}

static
pt_vec3 get_pixel(int x, int y) {
	pt_vec3 out;
	if (x < 0 || y < 0 || x >= framebuffer.width || y >= framebuffer.height) {
		out.x = out.y = out.z = 0;
	} else {
		pt_color4b* color = &framebuffer.color[y * framebuffer.width + x];
		out.x = color->red   / 255.0f;
		out.y = color->green / 255.0f;
		out.z = color->blue  / 255.0f;
	}
	return out;
}

void pt_renderer_draw_text(pt_texture* font, const char* text, int x, int y) {
	int w = (font->width >> 4);
	int s = (w >> 1);
	int h = (font->height >> 4);
	int i, px, py, index, cx, cy;
	pt_color4b color;

	for (i = 0; text[i]; i++) {
		cy = (text[i] >> 4) * w;
		cx = (text[i] & 0xF) * h;

		for (px = 0; px < w; px++) {
			for (py = 0; py < h; py++) {
				index = (cy + py) * font->width + (cx + px);
				color = font->data[index];

				if (color.red == 255 && color.green == 0 && color.blue == 255) {
					continue;
				}

				pt_renderer_put_pixel(x + px, y + py, 0, color.blue, color.green, color.red, 255);
			}
		}

		x += s;
	}
}

void pt_renderer_present(void) {
	pt_win_update(framebuffer.color);
}


void pt_renderer_enable(unsigned int f) {
	render_flags |= f;
}

void pt_renderer_disable(unsigned int f) {
	render_flags &= ~f;
}

void pt_renderer_set_world(pt_mat4* w) {
	world = *w;
}

void pt_renderer_set_view(pt_mat4* v) {
	view = *v;
	pt_mat4_multiply(&proj_view, &proj, &view);
}

void pt_renderer_set_proj(pt_mat4* p) {
	proj = *p;
	pt_mat4_multiply(&proj_view, &proj, &view);
}

void pt_renderer_set_ambient(pt_scalar s) {
	ambient = PT_MIN(PT_MAX(s, 0.0f), 1.0f);
}

void pt_renderer_set_texture(pt_texture* t) {
	texture = t;
}

void pt_renderer_set_normal_map(pt_texture* t) {
	normal_map = t;
}

void pt_renderer_set_light_direction(pt_scalar x, pt_scalar y, pt_scalar z) {
	pt_vec3_assign(&light_dir, x, y, z);
	pt_vec3_normalize(&light_dir, &light_dir);
}