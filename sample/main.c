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
#define _CRT_SECURE_NO_WARNINGS
#include <potion.h>

#include <stdio.h>
#include <math.h>

#include <string.h>

#define CONSOLE_SIZE 16

static pt_vertex vertices[3];
static pt_mesh mesh;
static pt_texture texture, font, normal_map;
static char command[256];
static char console[CONSOLE_SIZE][256];
static int console_active = 0;

void load_head(pt_mesh* mesh);
void load_house(pt_mesh* mesh);

static pt_vertex triangle[3];

void store_command(void) {
	int i;
	for (i = 1; i < CONSOLE_SIZE; i++) {
		strcpy(console[i - 1], console[i]);
	}
	strcpy(console[CONSOLE_SIZE - 1], command);
}

void parse_command(void) {
	store_command();

	char* p = strtok(command, " ");
	if (p != NULL) {
		if (strcmp(p, "set") == 0) {
			p = strtok(NULL, " ");
			if (strcmp(p, "size") == 0) {
				int width, height;

				p = strtok(NULL, " ");
				sscanf(p, "%d", &width);

				p = strtok(NULL, " ");
				sscanf(p, "%d", &height);

				pt_win_set_size(width, height);
				pt_renderer_set_size(width, height);

				pt_mat4 proj;
				pt_mat4_perspective(&proj, 45.0f, 4.0f / 3.0f, 0.001f, 100.0f);
				pt_renderer_set_proj(&proj);
			} else if (strcmp(p, "ambient") == 0) {
				pt_scalar ambient;

				p = strtok(NULL, " ");
				if (p) {
					sscanf(p, "%f", &ambient);

					pt_renderer_set_ambient(ambient);
				}
			}
		} else if (strcmp(p, "mesh") == 0) {
			p = strtok(NULL, " ");

			if (p == NULL) {
				printf("incorrect command\n");
			} else if (strcmp(p, "house") == 0) {
				pt_mesh_release(&mesh);
				load_house(&mesh);

				pt_texture_release(&texture);
				pt_texture_load(&texture, "data/textures/house.png");
				pt_renderer_set_normal_map(0);
			} else if (strcmp(p, "head") == 0) {
				pt_mesh_release(&mesh);
				load_head(&mesh);

				pt_texture_release(&texture);
				pt_texture_load(&texture, "data/textures/head.png");
				pt_renderer_set_normal_map(&normal_map);
			} else if (strcmp(p, "sphere") == 0) {
				p = strtok(NULL, " ");
				int stacks = 8, slices = 8;
				float radius = 0.25f;
				if (p) {
					sscanf(p, "%d", &stacks);

					p = strtok(NULL, " ");
					sscanf(p, "%d", &slices);

					p = strtok(NULL, " ");
					sscanf(p, "%f", &radius);
				}

				pt_mesh_release(&mesh);
				pt_mesh_create_sphere(&mesh, stacks, slices, radius);

				pt_texture_release(&texture);
				pt_texture_load(&texture, "data/textures/brick.png");
				pt_renderer_set_normal_map(0);
			} else {
				printf("incorrect command\n");
			}

		} else if (strcmp(p, "enable") == 0) {
			p = strtok(NULL, " ");

			if (p == NULL) {
				printf("incorrect command\n");
			} else if (strcmp(p, "wireframe") == 0) {
				pt_renderer_enable(PT_RENDERER_WIREFRAME);
			} else if (strcmp(p, "multithreading") == 0) {
				pt_renderer_enable(PT_RENDERER_MULTITHREADED);
			} else if (strcmp(p, "cullface") == 0) {
				pt_renderer_enable(PT_RENDERER_CULL_FACE);
			} else if (strcmp(p, "texturing") == 0) {
				pt_renderer_enable(PT_RENDERER_TEXTURING);
			} else if (strcmp(p, "shading") == 0) {
				pt_renderer_enable(PT_RENDERER_SHADING);
			} else if (strcmp(p, "depthtest") == 0) {
				pt_renderer_enable(PT_RENDERER_DEPTH_TEST);
			} else {
				printf("incorrect command\n");
			}

		} else if (strcmp(p, "disable") == 0) {
			p = strtok(NULL, " ");

			if (strcmp(p, "wireframe") == 0) {
				pt_renderer_disable(PT_RENDERER_WIREFRAME);
			} else if (strcmp(p, "multithreading") == 0) {
				pt_renderer_disable(PT_RENDERER_MULTITHREADED);
			} else if (strcmp(p, "cullface") == 0) {
				pt_renderer_disable(PT_RENDERER_CULL_FACE);
			} else if (strcmp(p, "texturing") == 0) {
				pt_renderer_disable(PT_RENDERER_TEXTURING);
			} else if (strcmp(p, "shading") == 0) {
				pt_renderer_disable(PT_RENDERER_SHADING);
			} else if (strcmp(p, "depthtest") == 0) {
				pt_renderer_disable(PT_RENDERER_DEPTH_TEST);
			} else {
				printf("incorrect command\n");
			}
		}


	}

	command[0] = 0;
}

void key_down(int key) {
	switch (key) {
	case PT_KEY_BACKSPACE:
		if (console_active && command[0] != 0) {
			command[strlen(command) - 1] = 0;
		}
		break;
	case PT_KEY_RETURN:
		if (console_active && command[0] != 0) {
			parse_command();
		}
		break;
	case PT_KEY_TILDE:
		console_active = !console_active;
		break;
	case PT_KEY_UP:
		if (console[CONSOLE_SIZE - 1][0]) {
			strcpy(command, console[CONSOLE_SIZE - 1]);
		}
		break;
	}
}

void key_up(int key) {

}

void text_input(const char* text) {
	if (console_active && text[0] != '`') {
		strcat(command, text);
	}
}

int main(int argc, char** argv) {
	double lastTime, currTime, acc = 0.0;
	
	pt_scalar x = 0.0;
	pt_mat4 world;
	pt_mat4 view;
	pt_mat4 proj;

	pt_mat4_perspective(&proj, 45.0f, 4.0f / 3.0f, 0.001f, 100.0f);
	pt_mat4_translation(&view, 0.0f, -0.2f, -0.1f);


	double timer = 0.0;
	int fps = 0;
	char fps_string[32];
	sprintf(fps_string, "FPS: 0");

	if (!pt_init(800, 600)) {
		return 0;
	}

	memset(console, 0, sizeof(console));
	command[0] = 0;

	pt_win_set_key_down_callback(key_down);
	pt_win_set_key_up_callback(key_up);
	pt_win_set_text_callback(text_input);

	pt_renderer_set_proj(&proj);
	pt_renderer_set_view(&view);

	pt_vec3_assign(&vertices[0].position, 0.0f, 0.0f, 0.0f);
	pt_vec3_assign(&vertices[1].position, 1.0f, 0.0f, 0.0f);
	pt_vec3_assign(&vertices[2].position, 0.0f, 1.0f, 0.0f);

	//pt_mesh_create_sphere(&mesh, 32, 32, 0.5f);
	load_head(&mesh);
	//load_house(&mesh);
	pt_texture_load(&texture, "data/textures/head.png");
	pt_texture_load(&font, "data/textures/courier_new.png");

	pt_texture_load(&normal_map, "data/textures/head_norm.png");
	
	pt_renderer_set_texture(&texture);
	pt_renderer_set_normal_map(&normal_map);

	lastTime = pt_get_time();

	//pt_renderer_enable(PT_RENDERER_MULTITHREADED);
	//pt_renderer_enable(PT_RENDERER_WIREFRAME);

	while (pt_win_is_open()) {
		currTime = pt_get_time();
		acc += (currTime - lastTime);
		lastTime = currTime;

		while (acc > 1.0 / 60.0) {
			x += 1.0f / 60.0f;

			timer += 1.0 / 60.0;

			acc -= 1.0 / 60.0;
		}

		vertices[2].position.y = sinf(x);

		pt_mat4_rotation_y(&world, x);

		//pt_mat4_translation(&world, 0.0f, 0.0f, sinf(x * 0.25f) * 1.0f);

		pt_renderer_set_proj(&proj);
		pt_renderer_set_view(&view);
		pt_renderer_set_world(&world);

		pt_win_poll_events();


		if (pt_win_has_focus()) {
			pt_renderer_clear();
			pt_renderer_draw_mesh(&mesh);
		}

		pt_renderer_disable(PT_RENDERER_DEPTH_TEST);
		pt_renderer_draw_text(&font, fps_string, 0, 0);

		if (console_active) {

			int height = CONSOLE_SIZE * 16;

			int i;
			for (i = 0; i < CONSOLE_SIZE; i++) {
				pt_renderer_draw_text(&font, console[i], 0, i * 16);
			}

			pt_renderer_draw_text(&font, command, 0, CONSOLE_SIZE * 16);
			pt_renderer_draw_text(&font, "_", strlen(command) * 8, CONSOLE_SIZE * 16);

		}
		pt_renderer_enable(PT_RENDERER_DEPTH_TEST);

		pt_renderer_present();

		pt_sleep(1);

		if (timer > 1.0) {
			sprintf(fps_string, "FPS: %d", fps);
			fps = 0;
			timer = 0.0;
		}

		fps++;
	}

	pt_texture_release(&texture);
	pt_texture_release(&font);
	pt_mesh_release(&mesh);
	pt_release();
	return 0;
}