#pragma once

#include "Camera.h"

namespace Logl
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(Frustum frustum, vec3 position, vec3 worldUp = vec3(0.0f, 1.0f, 0.0f))
			: Camera(frustum, position, worldUp), 
			m_speed(2.5f), m_sensitivity(0.1f)
		{
			Update();
		}

		mat4 GetViewMatrix() const override
		{
			return mat4::LookAt(m_position, m_position + m_front, m_up);
		}

		mat4 GetProjectionMatrix() const override
		{
			return mat4::Perspective(Radians(m_frustum.fov), m_frustum.ratio, m_frustum.near, m_frustum.far);
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

			Update();
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

		virtual void Update() override
		{
			m_front = m_EulerAngle.GetFront();
			m_right = CrossProduct(m_front, m_worldUp).normalize();
			m_up = CrossProduct(m_right, m_front).normalize();
		}

		virtual void SetMoveSpeed(float speed) override { m_speed = speed; }
		virtual void SetRotateSensitivity(float sensitivity) override { m_sensitivity = sensitivity; }

	protected:
		float m_speed;
		float m_sensitivity;
	};
}