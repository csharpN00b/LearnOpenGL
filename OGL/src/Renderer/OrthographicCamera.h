#pragma once

#include "Math/EulerAngle.h"
#include "Math/Matrix4f.h"

namespace Logl
{
	class OrthoGraphicCamera : public Camera
	{
	public:
		OrthoGraphicCamera(Frustum frustum, vec3 position, 
			vec3 worldUp = vec3(0.0f, 1.0f, 0.0f), 
			EulerAngle eulerAngle = EulerAngle(-90.0f, 0.0f))
			: Camera(frustum, position, worldUp, eulerAngle)
		{
		}

		virtual mat4 GetViewMatrix() const override
		{
			return mat4::LookAt(m_position, m_position + m_front, m_up);
		}

		virtual mat4 GetProjectionMatrix() const override
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

		}

		virtual void Scale(float yoffset) override
		{
			float k = 1.0f;
			if (yoffset < 0.0f)
				k = 2.0f;
			else if (yoffset > 0.0f)
				k = 0.5f;

			m_frustum.left *= k;
			m_frustum.right *= k;
			m_frustum.bottom *= k;
			m_frustum.top *= k;
		}
	};
}