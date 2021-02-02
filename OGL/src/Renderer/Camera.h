#pragma once

#include "Math/EulerAngle.h"
#include "Frustum.h"


namespace Logl
{
	enum class MoveDirection
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class Camera
	{
	public:
		Camera(Frustum frustum, vec3 position, vec3 worldUp)
			: m_frustum(frustum), m_position(position), m_worldUp(worldUp), m_EulerAngle(-90.0f, 0.0f)
		{
		}

		virtual ~Camera() {}

		virtual mat4 GetViewMatrix() const = 0;
		virtual mat4 GetProjectionMatrix() const = 0;

		virtual void Move(MoveDirection direction, float deltaTime) {}
		virtual void Move(float xoffset, float yoffset) {}
		virtual void Move(float xold, float yold, float xnew, float ynew) {}

		virtual void Turn(float xoffset, float yoffset) {}

		virtual void Scale(float yoffset) {}
		virtual void Scale(float yoffset, float xpos, float ypos) {}
		virtual void Scale(float yoffset, float xpos, float ypos, float sW, float sH) {}

		virtual void Update() {}

		virtual void Reset() {}

		virtual void SetRotate(bool bInOut) { }
		virtual void SetMove(bool bInOut) { }

		virtual void SetMoveSpeed(float speed) {}
		virtual void SetRotateSensitivity(float sensitivity) {}
		virtual void SetMoveSensitivity(float sensitivity) {}

		virtual void SetViewport(int x, int y, int width, int height) {}

	protected:
		Frustum m_frustum;

	protected:
		vec3 m_position;
		vec3 m_up;
		vec3 m_front;
		vec3 m_right;

		vec3 m_worldUp;

		EulerAngle m_EulerAngle;
	};
}