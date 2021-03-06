#include "PerspectiveCamera.h"

namespace Logl
{
	PerspectiveCamera::PerspectiveCamera(Frustum frustum, vec3 position, vec3 worldUp)
		: Camera(frustum, position, worldUp),
		m_MovementSpeed(2.5f), m_MouseSensitivity(0.1f)
	{
		Update();
	}

	CameraType PerspectiveCamera::Type() const { return CameraType::PerspectiveCamera; }

	mat4 PerspectiveCamera::GetViewMatrix() const
	{
		return mat4::LookAt(m_position, m_position + m_front, m_up);
	}

	mat4 PerspectiveCamera::GetProjectionMatrix() const
	{
		return mat4::Perspective(Radians(m_frustum.fov), m_frustum.ratio, m_frustum.near, m_frustum.far);
	}

	void PerspectiveCamera::Move(MoveDirection direction, float deltaTime)
	{
		float dis = m_MovementSpeed * deltaTime;
		if (direction == MoveDirection::FORWARD)
			m_position += dis * m_front;
		if (direction == MoveDirection::BACKWARD)
			m_position -= dis * m_front;
		if (direction == MoveDirection::LEFT)
			m_position -= dis * m_right;
		if (direction == MoveDirection::RIGHT)
			m_position += dis * m_right;
	}

	void PerspectiveCamera::Turn(float xoffset, float yoffset)
	{
		xoffset *= m_MouseSensitivity;
		yoffset *= m_MouseSensitivity;

		m_EulerAngle.yaw += xoffset;
		m_EulerAngle.pitch += yoffset;

		if (m_EulerAngle.pitch > 89.0f)
			m_EulerAngle.pitch = 89.0f;
		if (m_EulerAngle.pitch < -89.0f)
			m_EulerAngle.pitch = -89.0f;

		Update();
	}

	void PerspectiveCamera::Scale(float yoffset)
	{
		if (m_frustum.fov >= 1.0f && m_frustum.fov <= 45.0f)
			m_frustum.fov -= yoffset;
		if (m_frustum.fov <= 1.0f)
			m_frustum.fov = 1.0f;
		if (m_frustum.fov >= 45.0f)
			m_frustum.fov = 45.0f;
	}

	void PerspectiveCamera::Update()
	{
		m_front = m_EulerAngle.GetFront();
		m_right = cross(m_front, m_worldUp).normalize();
		m_up = cross(m_right, m_front).normalize();
	}

	void PerspectiveCamera::Reset()
	{
		m_EulerAngle.yaw = -90.0f;
		m_EulerAngle.pitch = 0.0f;
		m_EulerAngle.roll = 0.0f;
		m_position = m_initialPostion;
		Update();
	}

	void PerspectiveCamera::SetFrustum(float left, float right, float bottom, float top)
	{
		m_frustum.ratio = (right - left) / (top - bottom);
		// TODO : modify fov to scale?
	}

}