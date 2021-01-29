#pragma once

#include "Camera.h"

namespace Logl
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(Frustum frustum, vec3 position, 
			vec3 worldUp = vec3(0.0f, 1.0f, 0.0f), 
			EulerAngle eulerAngle = EulerAngle(-90.0f, 0.0f))
			: Camera(frustum, position, worldUp, eulerAngle)
		{
			m_front = m_EulerAngle.GetFront();
			m_right = CrossProduct(m_front, worldUp);
			m_up = CrossProduct(m_right, m_front);
		}

		mat4 GetViewMatrix() const override
		{
			return mat4::LookAt(m_position, m_position + m_front, m_up);
		}

		mat4 GetProjectionMatrix() const override
		{
			return m_frustum.GetProjectionMatrix();
		}

		virtual void Move(MoveDirection direction, float deltaTime) override
		{
			float dis = m_speed * deltaTime;
			if (direction == MoveDirection::FORWARD)
				m_position += dis * m_front;
			if (direction == MoveDirection::BACKWARD)
				m_position -= dis * m_front;
			if (direction == MoveDirection::LEFT)
				m_position -= dis * m_right;
			if (direction == MoveDirection::RIGHT)
				m_position += dis * m_right;
		}

		virtual void Turn(float xoffset, float yoffset) override
		{
			xoffset *= m_sensitivity;
			yoffset *= m_sensitivity;

			m_EulerAngle.yaw += xoffset;
			m_EulerAngle.pitch += yoffset;

			if (m_EulerAngle.pitch > 89.0f)
				m_EulerAngle.pitch = 89.0f;
			if (m_EulerAngle.pitch < -89.0f)
				m_EulerAngle.pitch = -89.0f;

			m_front = m_EulerAngle.GetFront();
		}

		virtual void Scale(float yoffset) override
		{
			if (m_frustum.fov >= 1.0f && m_frustum.fov <= 45.0f)
				m_frustum.fov -= yoffset;
			if (m_frustum.fov <= 1.0f)
				m_frustum.fov = 1.0f;
			if (m_frustum.fov >= 45.0f)
				m_frustum.fov = 45.0f;
		}
	};
}