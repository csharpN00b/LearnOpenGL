#pragma once

namespace Logl
{
#define PI 3.1415926f
//#define PA 0.0174533f
//#define PR 57.295780f

	inline bool IsEqual(float a, float b) { return fabs(a - b) < 0.0001f; }

	inline float Radians(float angle) { return angle * 0.0174533f; }

	inline float Angle(float radian) { return radian * 57.295780f; }
}