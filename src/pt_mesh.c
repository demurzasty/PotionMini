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
#include <stdio.h>

#include <math.h>

void pt_mesh_create_sphere(pt_mesh* mesh, int slices, int stacks, pt_scalar radius) {
	pt_scalar phi, theta;
	pt_scalar dphi = (pt_scalar)PT_PI / (pt_scalar)stacks;
	pt_scalar dtheta = (2.0f * (pt_scalar)(PT_PI)) / (pt_scalar)(slices);
	pt_scalar x, y, z, sc;
	int index = 0, stack, slice;

	int k;

	mesh->vertexCount = (slices + 1) * (stacks + 1);
	mesh->vertices = malloc(mesh->vertexCount * sizeof(pt_vertex));

	for (stack = 0; stack <= stacks; stack++) {
		phi = (pt_scalar)(PT_PI) * 0.5f - (pt_scalar)(stack) * dphi;
		y = radius * sinf(phi);
		sc = -radius * cosf(phi);

		for (slice = 0; slice <= slices; slice++) {
			theta = slice * dtheta;
			x = sc * sinf(theta);
			z = sc * cosf(theta);
			pt_vec3_assign(&mesh->vertices[index].position, x, y, z);
			pt_vec3_normalize(&mesh->vertices[index].normal, &mesh->vertices[index].position);

			pt_vec2_assign(&mesh->vertices[index].texcoord, slice / (float)(slices), stack / (float)(stacks));

			index++;
		}
	}

	mesh->indexCount = (stacks * slices * 6);
	mesh->indices = malloc(mesh->indexCount * sizeof(unsigned int));

	index = 0;
	k = slices + 1;

	for (stack = 0; stack < stacks; stack++) {
		for (slice = 0; slice < slices; slice++) {
			mesh->indices[index++] = (stack + 0) * k + slice;
			mesh->indices[index++] = (stack + 1) * k + slice;
			mesh->indices[index++] = (stack + 0) * k + slice + 1;

			mesh->indices[index++] = (stack + 0) * k + slice + 1;
			mesh->indices[index++] = (stack + 1) * k + slice;
			mesh->indices[index++] = (stack + 1) * k + slice + 1;
		}
	}

	mesh->vertexType = PT_VERTEX_POSITION | PT_VERTEX_NORMAL | PT_VERTEX_TEXCOORD;
}

void pt_mesh_create_plane(pt_mesh* mesh, float width, float height) {
	width *= 0.5f;
	height *= 0.5f;

	mesh->vertices = malloc(sizeof(pt_vertex) * 4);
	mesh->vertexCount = 4;

	pt_vec3_assign(&mesh->vertices[0].position, -width, 0, -height);
	pt_vec3_assign(&mesh->vertices[1].position, -width, 0, height);
	pt_vec3_assign(&mesh->vertices[2].position, width, 0, height);
	pt_vec3_assign(&mesh->vertices[3].position, width, 0, -height);

	pt_vec3_assign(&mesh->vertices[0].normal, 0, 1, 0);
	pt_vec3_assign(&mesh->vertices[1].normal, 0, 1, 0);
	pt_vec3_assign(&mesh->vertices[2].normal, 0, 1, 0);
	pt_vec3_assign(&mesh->vertices[3].normal, 0, 1, 0);

	pt_vec2_assign(&mesh->vertices[0].texcoord, 0, 0);
	pt_vec2_assign(&mesh->vertices[1].texcoord, 0, 1);
	pt_vec2_assign(&mesh->vertices[2].texcoord, 1, 1);
	pt_vec2_assign(&mesh->vertices[3].texcoord, 1, 0);

	mesh->indices = malloc(sizeof(int) * 6);
	mesh->indexCount = 6;

	mesh->indices[0] = 0;
	mesh->indices[1] = 1;
	mesh->indices[2] = 2;

	mesh->indices[3] = 2;
	mesh->indices[4] = 3;
	mesh->indices[5] = 0;

	mesh->vertexType = PT_VERTEX_POSITION | PT_VERTEX_NORMAL | PT_VERTEX_TEXCOORD;
}

void pt_mesh_release(pt_mesh* mesh) {
	PT_SAFE_FREE(mesh->vertices);
	PT_SAFE_FREE(mesh->indices);
}