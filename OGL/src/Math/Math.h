#pragma once

#include <cmath>

namespace Logl
{
#ifdef PI
#undef PI
#undef RPA
#undef APR
#endif

#define PI 3.1415926f
#define RPA 0.0174533f
#define APR 57.295780f

	inline bool IsEqual(float a, float b) { return fabs(a - b) < 0.00001f; }

	inline bool IsZero(float val) { return val < 0.00001f && val> -0.00001f; }

	inline float Radians(float angle) { return angle * RPA; }

	inline float Angle(float radian) { return radian * APR; }
}