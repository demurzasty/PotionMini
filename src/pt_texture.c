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

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

void pt_texture_load(pt_texture* texture, const char* fileName) {
	PT_SAFE_FREE(texture->data);

	int c;
	texture->data = (pt_color4b*)stbi_load(fileName, &texture->width, &texture->height, &c, 4);
}

void pt_texture_sample(pt_texture* texture, pt_scalar u, pt_scalar v, pt_color4b* out) {
	u *= texture->width;
	v *= texture->height;

	int x = (int)u;
	int y = (int)v;

	if (x < 0 || x >= texture->width || y < 0 || y >= texture->height) {
		out->red = 0;
		out->green = 0;
		out->blue = 0;
		out->alpha = 255;
		return;
	}

	/*
	while (x < 0) {
		x += texture->width;
	}

	while (y < 0) {
		y += texture->height;
	}

	x %= texture->width;
	y %= texture->height;
	*/

	pt_color4b* color = &texture->data[y * texture->width + x];

	out->red = color->blue;
	out->green = color->green;
	out->blue = color->red;
	out->alpha = color->alpha;
}

void pt_texture_release(pt_texture* texture) {
	PT_SAFE_FREE(texture->data);
}