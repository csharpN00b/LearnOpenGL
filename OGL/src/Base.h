#pragma once

namespace Logl
{
#define PI 3.14159f

	inline bool IsEqual(float a, float b) { return fabs(a - b) < 0.0001f; }
}