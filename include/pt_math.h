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
#ifndef PT_MATH_H
#define PT_MATH_H

#define PT_PI 3.14159265359

#define PT_MAX(x, y) ( ( (x) > (y) ) ? (x) : (y) )
#define PT_MIN(x, y) ( ( (x) < (y) ) ? (x) : (y) )
#define PT_CLAMP(x, a, b) PT_MIN(b, PT_MAX(x, a))
#define PT_SATURATE(x) PT_CLAMP(x, (pt_scalar)0.0f, (pt_scalar)1.0f)
#define PT_LERP(x, y, t) ( (1 - (t)) * (x) + (t) * (y) )

#define PT_ABS(x) ( (x < 0) ? (-x) : (x) )

typedef float pt_scalar;

#endif