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
#ifndef PT_VERTEX_H
#define PT_VERTEX_H

#define PT_VERTEX_POSITION 1
#define PT_VERTEX_NORMAL   2
#define PT_VERTEX_TEXCOORD 4
#define PT_VERTEX_COLOR    8

typedef struct pt_vertex {
	pt_vec3    position;
	pt_vec3    normal;
	pt_vec3    tangent;
	pt_vec3    bitangent;
	pt_vec2    texcoord;
	pt_color4b color;

	pt_vec3    screenspace;
} pt_vertex;

#endif