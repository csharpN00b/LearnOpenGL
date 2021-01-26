#pragma once

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
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;

	struct ViewParam
	{
		float ratio;
		float fov;
		float near;
		float far;

		ViewParam(float nRatio, float nFov = 45.0f, float zNear = 0.1f, float zFar = 100.0f)
			: ratio(nRatio), fov(nFov), near(zNear), far(zFar)
		{}

		mat4 GetProjectionMatrix() const { return mat4::Perspective(Radians(fov), ratio, near, far); }
	};


	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(ViewParam view, vec3 pos, vec3 up = vec3(0.0f, 1.0f, 0.0f), float pitch = PITCH, float yaw = YAW)
			: m_view(view), m_position(pos), m_worldUp(up), m_pitch(pitch), m_yaw(yaw)
		{
			vec3 front;
			front.x = cos(Radians(yaw)) * cos(Radians(pitch));
			front.y = sin(Radians(pitch));
			front.z = sin(Radians(yaw)) * cos(Radians(pitch));
			m_front = front.normalize();
			m_right = CrossProduct(m_front, m_worldUp);
			m_up = CrossProduct(m_right, m_front);

		}

		mat4 GetProjectionMatrix() const
		{
			return m_view.GetProjectionMatrix();
		}

		mat4 GetViewMatrix() const
		{
			return mat4::LookAt(m_position, m_position + m_front, m_up);
		}

		void OnMouseMove(double xoffset, double yoffset)
		{
			xoffset *= SENSITIVITY;
			yoffset *= SENSITIVITY;

			m_yaw += xoffset;
			m_pitch += yoffset;

			if (m_pitch > 89.0f)
				m_pitch = 89.0f;
			if (m_pitch < -89.0f)
				m_pitch = -89.0f;

			vec3 front;
			front.x = cos(Radians(m_yaw)) * cos(Radians(m_pitch));
			front.y = sin(Radians(m_pitch));
			front.z = sin(Radians(m_yaw)) * cos(Radians(m_pitch));
			m_front = front.normalize();
		}

		void OnMouseScroll(double yoffset)
		{
			if (m_view.fov >= 1.0f && m_view.fov <= 45.0f)
				m_view.fov -= yoffset;
			if (m_view.fov <= 1.0f)
				m_view.fov = 1.0f;
			if (m_view.fov >= 45.0f)
				m_view.fov = 45.0f;
		}

		void OnKeyPress(Camera_Movement direction, float deltaTime)
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
		vec3 m_worldUp;
		vec3 m_front;
		vec3 m_right;

		// Euler Angle
		float m_pitch;
		float m_yaw;

		ViewParam m_view;
	};
}