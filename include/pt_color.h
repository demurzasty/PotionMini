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

#ifndef PT_COLOR_H
#define PT_COLOR_H

typedef struct pt_color3b {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
} pt_color3b;


typedef struct pt_color4b {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char alpha;
} pt_color4b;

#endif