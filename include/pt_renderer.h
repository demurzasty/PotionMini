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
#ifndef PT_RENDERER_H
#define PT_RENDERER_H

#define PT_RENDERER_CULL_FACE     1
#define PT_RENDERER_WIREFRAME     2
#define PT_RENDERER_DEPTH_TEST    4
#define PT_RENDERER_MULTITHREADED 8
#define PT_RENDERER_TEXTURING     16
#define PT_RENDERER_SHADING       32

#define PT_RENDERER_DEFAULT (PT_RENDERER_CULL_FACE | PT_RENDERER_DEPTH_TEST | PT_RENDERER_TEXTURING | PT_RENDERER_SHADING)

int pt_renderer_init(int width, int height);

void pt_renderer_set_size(int width, int height);

void pt_renderer_clear(void);

void pt_renderer_put_pixel(int x, int y, int z, int r, int g, int b, int a);

void pt_renderer_draw_line(int x1, int y1, int z1, int x2, int y2, int z2);

void pt_renderer_draw_horizontal_line(int y, int x1, int z1, int x2, int z2);

void pt_renderer_draw_triangle(pt_vertex* v0, pt_vertex* v1, pt_vertex* v2, unsigned int flags);

void pt_renderer_draw_mesh(pt_mesh* mesh);

void pt_renderer_draw_text(pt_texture* font, const char* text, int x, int y);

void pt_renderer_present(void);

void pt_renderer_enable(unsigned int f);

void pt_renderer_disable(unsigned int f);

void pt_renderer_set_world(pt_mat4* w);

void pt_renderer_set_view(pt_mat4* v);

void pt_renderer_set_proj(pt_mat4* p);

void pt_renderer_set_texture(pt_texture* t);

void pt_renderer_set_normal_map(pt_texture* t);

void pt_renderer_set_ambient(pt_scalar s);

void pt_renderer_set_light_direction(pt_scalar x, pt_scalar y, pt_scalar z);

void pt_renderer_release(void);

#endif