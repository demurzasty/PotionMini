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
#if 0

#define _CRT_SECURE_NO_WARNINGS
#include <potion.h>

#include <stdio.h>
#include <math.h>

#include <string.h>

void load_head(pt_mesh* mesh);
void load_house(pt_mesh* mesh);

static struct {
	pt_scalar rotation_y;
	pt_mesh mesh;
	pt_texture texture;
} head;
static pt_mesh plane;

void prepare_camera(void) {
	int width, height;
	pt_win_get_size(&width, &height);

	pt_mat4 matrix, rotation;
	pt_mat4_rotation_x(&rotation, PT_PI * 0.125f);
	pt_mat4_translation(&matrix, 0.0f, -0.75f, -1.0f);
	pt_mat4_multiply(&matrix, &rotation, &matrix);
	pt_renderer_set_view(&matrix);

	pt_mat4_perspective(&matrix, 45.0f, width / (pt_scalar)height, 0.01f, 100.0f);
	pt_renderer_set_proj(&matrix);
}

int init(void) {
	if (!pt_init(640, 480)) {
		return 0;
	}

	head.rotation_y = 0.0f;
	load_house(&head.mesh);
	pt_texture_load(&head.texture, "data/textures/house.png");

	pt_mesh_create_plane(&plane, 1.0f, 1.0f);

	prepare_camera();
	return 1;
}

int release(void) {

	pt_mesh_release(&plane);

	pt_texture_release(&head.texture);
	pt_mesh_release(&head.mesh);
	pt_release();
	return 0;
}

void update(float delta_time) {
	head.rotation_y += delta_time;
}

void render(void) {
	pt_renderer_clear();

	pt_mat4 world;
	pt_mat4_rotation_y(&world, head.rotation_y);
	pt_renderer_set_world(&world);

	pt_renderer_set_texture(&head.texture);
	pt_renderer_draw_mesh(&head.mesh);

	pt_mat4_identity(&world);
	pt_renderer_set_world(&world);

	pt_renderer_set_texture(0);
	pt_renderer_draw_mesh(&plane);

	pt_renderer_present();
}

void run(void) {
	double lastTime, currTime, acc = 0.0;

	lastTime = pt_get_time();
	while (pt_win_is_open()) {
		currTime = pt_get_time();
		acc += (currTime - lastTime);
		lastTime = currTime;

		pt_win_poll_events();

		while (acc >= 1.0 / 60.0) {
			update(1.0f / 60.0f);
			acc -= 1.0 / 60.0;
		}

		render();

		pt_sleep(1);
	}
}

int main(int argc, char** argv) {
	if (init()) {
		run();
		release();
	}
	return 0;
}

#endif