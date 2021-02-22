#include "OrthographicCamera.h"

#include "Math/Matrix4f.h"

namespace Logl
{
	OrthographicCamera::OrthographicCamera(Frustum frustum, vec3 position, vec3 worldUp)
		: Camera(frustum, position, worldUp),
		m_Scale(1.0f)
	{
		Update();
	}

	CameraType OrthographicCamera::Type() const { return CameraType::OrthographicCamera; }

	mat4 OrthographicCamera::GetViewMatrix() const
	{
		return mat4::LookAt(m_position, m_position + m_front, m_up);
	}

	mat4 OrthographicCamera::GetProjectionMatrix() const
	{
		return mat4::Ortho(m_Scale * m_frustum.left,
			m_Scale * m_frustum.right,
			m_Scale * m_frustum.bottom,
			m_Scale * m_frustum.top,
			m_frustum.near, m_frustum.far);
	}

	void OrthographicCamera::Move(float xold, float yold, float xnew, float ynew)
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

	void OrthographicCamera::Turn(float xoffset, float yoffset)
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

			auto center = vec3(0.0f, 0.0f, (m_frustum.near + m_frustum.far) * 0.5f);
			auto transform = Inverse(GetViewMatrix());
			auto ptWorld = transform * center;

			auto distance = Distance(m_position, ptWorld);
			auto target = m_position + distance * m_front;
			Update();
			m_position = target - distance * m_front;
		}
	}

	void OrthographicCamera::Scale(float yoffset, float xpos, float ypos)
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

	void OrthographicCamera::Update()
	{
		m_front = m_EulerAngle.GetFront();
		m_right = CrossProduct(m_front, m_worldUp).normalize();
		m_up = CrossProduct(m_right, m_front).normalize();
	}

	void OrthographicCamera::Reset()
	{
		m_EulerAngle.yaw = -90.0f;
		m_EulerAngle.pitch = 0.0f;
		m_EulerAngle.roll = 0.0f;
		m_position = m_initialPostion;
		m_Scale = 1.0f;
		Update();
	}

	void OrthographicCamera::SetFrustum(float left, float right, float bottom, float top)
	{
		m_frustum.left = left;
		m_frustum.right = right;
		m_frustum.bottom = bottom;
		m_frustum.top = top;
	}

	void OrthographicCamera::SetViewport(int x, int y, int width, int height)
	{
		m_ViewportMatrix = mat4::Viewport(x, y, width, height);
	}


	//=====================================================================================================================


	OrthoCamera::OrthoCamera(Frustum frustum, vec3 position, vec3 worldUp)
				: Camera(frustum, position, worldUp),
				m_bRotate(false), m_bMove(false), m_RotateSensitivity(0.1f), m_MoveSensitivity(0.01f)
			{
				Update();
			}

	mat4 OrthoCamera::GetViewMatrix() const
	{
		return mat4::LookAt(m_position, m_position + m_front, m_up);
	}

	mat4 OrthoCamera::GetProjectionMatrix() const
	{
		return mat4::Ortho(m_frustum.left, m_frustum.right, m_frustum.bottom, m_frustum.top,
			m_frustum.near, m_frustum.far);
	}

	void OrthoCamera::Move(float xoffset, float yoffset)
	{
		if (m_bMove)
		{
			auto vector = xoffset * m_MoveSensitivity * m_right + yoffset * m_MoveSensitivity * m_up;
			m_position -= vector;
		}
	}

	void OrthoCamera::Turn(float xoffset, float yoffset)
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

	void OrthoCamera::Scale(float yoffset, float xpos, float ypos, float sW, float sH)
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

	void OrthoCamera::Update()
	{
		m_front = m_EulerAngle.GetFront();
		m_right = CrossProduct(m_front, m_worldUp).normalize();
		m_up = CrossProduct(m_right, m_front).normalize();
	}
}