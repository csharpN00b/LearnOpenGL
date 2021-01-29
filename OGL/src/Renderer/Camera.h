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
		Camera(Frustum frustum, vec3 position, vec3 worldUp, EulerAngle eulerAngle)
			: m_frustum(frustum), m_position(position), m_EulerAngle(eulerAngle), 
			m_speed(2.5f), m_sensitivity(0.1f)
		{
			m_front = m_EulerAngle.GetFront();
			m_right = CrossProduct(m_front, worldUp);
			m_up = CrossProduct(m_right, m_front);
		}

		virtual ~Camera() {}

		virtual mat4 GetViewMatrix() const = 0;
		virtual mat4 GetProjectionMatrix() const = 0;

		virtual void Move(MoveDirection direction, float deltaTime) = 0;
		virtual void Turn(float xoffset, float yoffset) = 0;
		virtual void Scale(float yoffset) = 0;

		void SetMoveSpeed(float speed) { m_speed = speed; }
		void SetSensitivity(float sensitivity) { m_sensitivity = sensitivity; }

	protected:
		Frustum m_frustum;

	protected:
		vec3 m_position;
		vec3 m_up;
		vec3 m_front;
		vec3 m_right;

		EulerAngle m_EulerAngle;

	protected:
		float m_speed;
		float m_sensitivity;

	};
}