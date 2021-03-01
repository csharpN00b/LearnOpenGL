#pragma once

#include "Vector3f.h"

namespace Logl
{
	struct vec4
	{
		union
		{
			struct
			{
				float x, y, z, w;
			};
			struct
			{
				float r, g, b, a;
			};
		};

		vec4(float nx, float ny, float nz, float nw)
			: x(nx), y(ny), z(nz), w(nw)
		{
		}

		explicit vec4(const vec3& vector)
			: x(vector.x), y(vector.y), z(vector.z), w(1.0f)
		{
		}
	};


	inline float operator*(const vec4 v1, const vec4& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

}