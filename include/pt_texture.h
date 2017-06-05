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
#ifndef PT_TEXTURE_H
#define PT_TEXTURE_H

typedef struct pt_texture {
	pt_color4b* data;
	int width;
	int height;
} pt_texture;

void pt_texture_load(pt_texture* texture, const char* fileName);

void pt_texture_sample(pt_texture* texture, pt_scalar u, pt_scalar v, pt_color4b* out);

void pt_texture_release(pt_texture* texture);

#endif