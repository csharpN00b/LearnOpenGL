#pragma once

namespace Logl
{

	inline bool IsEqual(float a, float b) { return fabs(a - b) < 0.0001f; }
}