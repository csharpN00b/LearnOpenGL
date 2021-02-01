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
			: Camera(frustum, position, worldUp, eulerAngle), 
			m_bRotate(false), m_bMove(false), m_RotateSensitivity(0.1f), m_MoveSensitivity(0.01f), m_Scale(1.0f)
		{
			Update();
		}

		virtual mat4 GetViewMatrix() const override
		{
			return mat4::LookAt(m_position, m_position + m_front, m_up);
		}

		virtual mat4 GetProjectionMatrix() const override
		{
			return m_frustum.GetProjectionMatrix();
		}

		virtual void Turn(float xoffset, float yoffset) override
		{
			if (m_bRotate)
			{
				xoffset *= m_RotateSensitivity;
				yoffset *= m_RotateSensitivity;

				m_EulerAngle.yaw += xoffset;
				m_EulerAngle.pitch += yoffset;

				if (m_EulerAngle.pitch > 89.0f)
					m_EulerAngle.pitch = 89.0f;
				if (m_EulerAngle.pitch < -89.0f)
					m_EulerAngle.pitch = -89.0f;
				
				auto mag = VectorMag(m_position);
				auto target = m_position + mag * m_front;
				Update();
				m_position = target - mag * m_front;
			}
			else if (m_bMove)
			{
				auto vector = xoffset * m_MoveSensitivity * m_right + yoffset * m_MoveSensitivity * m_up;
				m_position -= vector;
			}
		}

		virtual void Scale(float yoffset, float xpos, float ypos, float sW, float sH) override
		{
			float scale = 1.0f;
			if (yoffset < 0.0f)
				scale = 2.0f;
			else if (yoffset > 0.0f)
				scale = 0.5f;

			auto w = m_frustum.GetWidth();
			auto h = m_frustum.GetHeight();
			float k = -1.0f;

			m_frustum.left *= scale;
			m_frustum.right *= scale;
			m_frustum.bottom *= scale;
			m_frustum.top *= scale;
			if (scale < 1.0f)
			{
				w = m_frustum.GetWidth();
				h = m_frustum.GetHeight();
				k = 1.0f;
			}

			auto x = w * (xpos - sW / 2.0f) / sW;
			auto y = h * (sH / 2.0f - ypos) / sH;
			//auto vector = vec3(k * x, k * y, 0.0f);
			auto vector = k * x * m_right + k * y * m_up;
			m_position += vector;

			m_Scale *= scale;
		}

		virtual void Update() override
		{
			m_front = m_EulerAngle.GetFront();
			m_right = CrossProduct(m_front, m_worldUp).normalize();
			m_up = CrossProduct(m_right, m_front).normalize();
		}

		virtual void SetRotate(bool bInOut) override { m_bRotate = bInOut; }
		virtual void SetMove(bool bInOut) override { m_bMove = bInOut; }

		virtual void SetRotateSensitivity(float sensitivity) override { m_RotateSensitivity = sensitivity; }
		virtual void SetMoveSensitivity(float sensitivity) override { m_MoveSensitivity = sensitivity; }

	protected:
		bool m_bRotate;
		bool m_bMove;

	protected:
		float m_RotateSensitivity;
		float m_MoveSensitivity;

	protected:
		float m_Scale;
	};
}