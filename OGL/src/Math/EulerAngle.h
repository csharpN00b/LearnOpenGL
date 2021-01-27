#pragma once

#include "Vector3f.h"

namespace Logl
{
	struct EulerAngle
	{
		float yaw;
		float pitch;
		float roll;

		EulerAngle()
			: yaw(0.0f), pitch(0.0f), roll(0.0f)
		{}

		EulerAngle(float yaw_, float pitch_, float roll_ = 0.0f)
			: yaw(yaw_), pitch(pitch_), roll(roll_)
		{}

		vec3 GetFront() const
		{
			vec3 front;
			front.x = cos(Radians(yaw)) * cos(Radians(pitch));
			front.y = sin(Radians(pitch));
			front.z = sin(Radians(yaw)) * cos(Radians(pitch));
			return front.normalize();
		}
	};
}