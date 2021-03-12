#pragma once

#include <cmath>

namespace Logl
{
	const float PI = 3.1415926f;
	const float RPA = 0.0174533f; // PI/180
	const float APR = 57.295780f; // 180/PI

	inline bool IsEqual(float a, float b) { return fabs(a - b) < 0.00001f; }

	inline bool IsZero(float val) { return val < 0.00001f && val> -0.00001f; }

	inline float Radians(float angle) { return angle * RPA; }

	inline float Angle(float radian) { return radian * APR; }
}