#pragma once

#include "../Math/EulerAngle.h"
#include "../Math/Matrix4f.h"

namespace Logl
{
	struct OrthoFrustum
	{
		float left;
		float right;
		float bottom;
		float top;
		float near;
		float far;

		OrthoFrustum(float left_, float right_, float bottom_, float top_, float near_ = 0.1f, float far_ = 100.0f)
			: left(left_), right(right_), bottom(bottom_), top(top_), near(near_), far(far_)
		{}

		void Scale(float k)
		{
			left *= k;
			right *= k;
			bottom *= k;
			top *= k;
		}

		mat4 GetProjectionMatrix() const { return mat4::Ortho(left, right, bottom, top, near, far); }
	};

	class OrthoGraphicCamera
	{
	public:
		OrthoGraphicCamera(OrthoFrustum frustum, vec3 position, 
			vec3 worldUp = vec3(0.0f, 1.0f, 0.0f), 
			EulerAngle eulerAngle = EulerAngle(-90.0f, 0.0f))
			: m_frustum(frustum), m_position(position), m_EulerAngle(eulerAngle), m_bRotate(false)
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
			if (m_bRotate)
			{

			}
		}

		void OnMouseScroll(float yoffset)
		{
			if (yoffset < 0.0f)
			{
				m_frustum.Scale(2.0f);
			}

			if (yoffset > 0.0f)
			{
				m_frustum.Scale(0.5f);
			}
		}

		void OnKeyPress(int key)
		{

		}

		void SetRotateState(bool state) { m_bRotate = state; }

	private:
		vec3 m_position;
		vec3 m_up;
		vec3 m_front;
		vec3 m_right;

		EulerAngle m_EulerAngle;

		OrthoFrustum m_frustum;

		bool m_bRotate;
	};
}