#pragma once

#include "../Math/EulerAngle.h"
#include "../Math/Matrix4f.h"

namespace Logl
{
	enum class Camera_Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	// Default camera values
	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;

	struct PerspectiveFrustum
	{
		float ratio;
		float fov;
		float near;
		float far;

		PerspectiveFrustum(float ratio_, float fov_ = 45.0f, float near_ = 0.1f, float far_ = 100.0f)
			: ratio(ratio_), fov(fov_), near(near_), far(far_)
		{}

		mat4 GetProjectionMatrix() const { return mat4::Perspective(Radians(fov), ratio, near, far); }
	};


	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(PerspectiveFrustum frustum, vec3 position, 
			vec3 worldUp = vec3(0.0f, 1.0f, 0.0f), 
			EulerAngle eulerAngle = EulerAngle(-90.0f, 0.0f))
			: m_frustum(frustum), m_position(position), m_EulerAngle(eulerAngle)
		{
			m_front = m_EulerAngle.GetFront();
			m_right = CrossProduct(m_front, worldUp);
			m_up = CrossProduct(m_right, m_front);
		}

		mat4 GetViewMatrix() const
		{
			return mat4::LookAt(m_position, m_position + m_front, m_up);
		}

		mat4 GetProjectionMatrix() const
		{
			return m_frustum.GetProjectionMatrix();
		}

		void OnMouseMove(float xoffset, float yoffset)
		{
			xoffset *= SENSITIVITY;
			yoffset *= SENSITIVITY;

			m_EulerAngle.yaw += xoffset;
			m_EulerAngle.pitch += yoffset;

			if (m_EulerAngle.pitch > 89.0f)
				m_EulerAngle.pitch = 89.0f;
			if (m_EulerAngle.pitch < -89.0f)
				m_EulerAngle.pitch = -89.0f;

			m_front = m_EulerAngle.GetFront();
		}

		void OnMouseScroll(float yoffset)
		{
			if (m_frustum.fov >= 1.0f && m_frustum.fov <= 45.0f)
				m_frustum.fov -= yoffset;
			if (m_frustum.fov <= 1.0f)
				m_frustum.fov = 1.0f;
			if (m_frustum.fov >= 45.0f)
				m_frustum.fov = 45.0f;
		}

		void OnKeyPress(int key)
		{

		}

		void Move(Camera_Movement direction, float deltaTime)
		{
			float dis = SPEED * deltaTime;
			if (direction == Camera_Movement::FORWARD)
				m_position += dis * m_front;
			if (direction == Camera_Movement::BACKWARD)
				m_position -= dis * m_front;
			if (direction == Camera_Movement::LEFT)
				m_position -= dis * m_right;
			if (direction == Camera_Movement::RIGHT)
				m_position += dis * m_right;
		}

	private:
		vec3 m_position;
		vec3 m_up;
		vec3 m_front;
		vec3 m_right;

		EulerAngle m_EulerAngle;

		PerspectiveFrustum m_frustum;
	};
}