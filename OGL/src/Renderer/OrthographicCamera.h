#pragma once

#include "Math/EulerAngle.h"
#include "Math/Matrix4f.h"

namespace Logl
{
	class OrthoGraphicCamera : public Camera
	{
	public:
		OrthoGraphicCamera(Frustum frustum, vec3 position, vec3 worldUp = vec3(0.0f, 1.0f, 0.0f))
			: Camera(frustum, position, worldUp), 
			m_initialPostion(position), m_Scale(1.0f)
		{
			Update();
		}

		virtual mat4 GetViewMatrix() const override
		{
			return mat4::LookAt(m_position, m_position + m_front, m_up);
		}

		virtual mat4 GetProjectionMatrix() const override
		{
			return mat4::Ortho(m_Scale * m_frustum.left, m_Scale * m_frustum.right, m_Scale * m_frustum.bottom, m_Scale * m_frustum.top,
				m_frustum.near, m_frustum.far);
		}

		virtual void Move(float xold, float yold, float xnew, float ynew) override
		{
			if (m_bMove)
			{
				auto transform = Inverse(m_ViewportMatrix * GetProjectionMatrix() * GetViewMatrix());
				auto ptOld = transform * vec3(xold, yold, 0.0f);
				auto p2 = transform * vec3(xnew, ynew, 0.0f);
				auto vector = vec3(p2.x - ptOld.x, p2.y - ptOld.y, p2.z - ptOld.z);
				m_position -= vector;
			}
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
				
				auto center = vec3(0.0f, 0.0f, (m_frustum.near + m_frustum.far) * 0.5);
				auto transform = Inverse(GetViewMatrix());
				auto ptWorld = transform * center;

				auto distance = Distance(m_position, ptWorld);
				auto target = m_position + distance * m_front;
				Update();
				m_position = target - distance * m_front;
			}
		}

		virtual void Scale(float yoffset, float xpos, float ypos)
		{
			float scale = 1.0f;
			if (yoffset < 0.0f)
				scale = 2.0f;
			else if (yoffset > 0.0f)
				scale = 0.5f;

			// window coordinates <----(height - y)---- viewport coordinates = vp*p*v*model*postion

			auto transform = Inverse(m_ViewportMatrix * GetProjectionMatrix() * GetViewMatrix());
			auto ptOld = transform * vec3(xpos, ypos, 0.0f);

			m_Scale *= scale;

			transform = Inverse(m_ViewportMatrix * GetProjectionMatrix() * GetViewMatrix());
			auto ptNew = transform * vec3(xpos, ypos, 0.0f);
			auto vector = vec3(ptNew.x - ptOld.x, ptNew.y - ptOld.y, ptNew.z - ptOld.z);
			m_position -= vector;

		}

		virtual void Update() override
		{
			m_front = m_EulerAngle.GetFront();
			m_right = CrossProduct(m_front, m_worldUp).normalize();
			m_up = CrossProduct(m_right, m_front).normalize();
		}

		virtual void Reset() override
		{
			m_EulerAngle.yaw = -90.0f;
			m_EulerAngle.pitch = 0.0f;
			m_EulerAngle.roll = 0.0f;
			m_position = m_initialPostion;
			m_Scale = 1.0f;
			Update();
		}

		virtual void SetRotate(bool bInOut) override { m_bRotate = bInOut; }
		virtual void SetMove(bool bInOut) override { m_bMove = bInOut; }

		virtual void SetRotateSensitivity(float sensitivity) override { m_RotateSensitivity = sensitivity; }
		virtual void SetMoveSensitivity(float sensitivity) override { m_MoveSensitivity = sensitivity; }

		virtual void SetViewport(int x, int y, int width, int height) override 
		{
			m_frustum.left = -width * 0.5f * 0.01;
			m_frustum.right = width * 0.5f * 0.01;
			m_frustum.bottom = -height * 0.5f * 0.01;
			m_frustum.top = height * 0.5f * 0.01;

			m_ViewportMatrix = mat4::Viewport(x, y, width, height);
		}

	protected:
		bool m_bRotate = false;
		bool m_bMove = false;

		float m_RotateSensitivity = 0.1f;
		float m_MoveSensitivity = 0.01f;

		float m_Scale;
		mat4 m_ViewportMatrix;

	protected:
		vec3 m_initialPostion;
	};



	class OrthoCamera : public Camera
	{
	public:
		OrthoCamera(Frustum frustum, vec3 position, vec3 worldUp = vec3(0.0f, 1.0f, 0.0f))
			: Camera(frustum, position, worldUp),
			m_bRotate(false), m_bMove(false), m_RotateSensitivity(0.1f), m_MoveSensitivity(0.01f)
		{
			Update();
		}

		virtual mat4 GetViewMatrix() const override
		{
			return mat4::LookAt(m_position, m_position + m_front, m_up);
		}

		virtual mat4 GetProjectionMatrix() const override
		{
			return mat4::Ortho(m_frustum.left, m_frustum.right, m_frustum.bottom, m_frustum.top,
				m_frustum.near, m_frustum.far);
		}

		virtual void Move(float xoffset, float yoffset) override
		{
			if (m_bMove)
			{
				auto vector = xoffset * m_MoveSensitivity * m_right + yoffset * m_MoveSensitivity * m_up;
				m_position -= vector;
			}
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
		}

		virtual void Scale(float yoffset, float xpos, float ypos, float sW, float sH) override
		{
			float scale = 1.0f;
			if (yoffset < 0.0f)
				scale = 2.0f;
			else if (yoffset > 0.0f)
				scale = 0.5f;

			auto w = m_frustum.right - m_frustum.left;
			auto h = m_frustum.top - m_frustum.bottom;
			float k = -1.0f;

			m_frustum.left *= scale;
			m_frustum.right *= scale;
			m_frustum.bottom *= scale;
			m_frustum.top *= scale;
			if (scale < 1.0f)
			{
				w = m_frustum.right - m_frustum.left;
				h = m_frustum.top - m_frustum.bottom;
				k = 1.0f;
			}

			auto x = w * (xpos - sW / 2.0f) / sW;
			auto y = h * (sH / 2.0f - ypos) / sH;
			//auto vector = vec3(k * x, k * y, 0.0f);
			auto vector = k * x * m_right + k * y * m_up;
			m_position += vector;
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
	};
}