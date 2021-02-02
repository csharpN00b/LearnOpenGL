#pragma once

#include "Math/Matrix4f.h"

#include <functional>

namespace Logl
{
	struct Frustum
	{
		float near;
		float far;
		union
		{
			struct
			{
				float left, right, bottom, top;
			};
			struct
			{
				float fov, ratio, _a, _b;
			};
		};

		std::function<mat4()> GetProjectionMatrix;

		Frustum(float left_, float right_, float bottom_, float top_, float near_, float far_)
			: near(near_), far(far_), left(left_), right(right_), bottom(bottom_), top(top_) 
		{
		}

		Frustum(float ratio_, float fov_, float near_, float far_)
			: near(near_), far(far_), fov(fov_), ratio(ratio_), _a(0.0f), _b(0.0f)
		{
		}
	};


}