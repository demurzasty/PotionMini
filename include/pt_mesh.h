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
#ifndef PT_MESH_H
#define PT_MESH_H

typedef struct pt_mesh {
	pt_vertex* vertices;
	unsigned int vertexCount;
	unsigned int vertexType;

	unsigned int* indices;
	unsigned int indexCount;
} pt_mesh;

void pt_mesh_create_sphere(pt_mesh* mesh, int slices, int stacks, pt_scalar radius);

void pt_mesh_create_plane(pt_mesh* mesh, float width, float height);

void pt_mesh_release(pt_mesh* mesh);

#endif