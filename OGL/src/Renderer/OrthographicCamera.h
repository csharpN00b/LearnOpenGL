#pragma once

#include "Camera.h"

namespace Logl
{
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(Frustum frustum, vec3 position, vec3 worldUp = vec3(0.0f, 1.0f, 0.0f));

		virtual CameraType Type() const override;

		virtual mat4 GetViewMatrix() const override;

		virtual mat4 GetProjectionMatrix() const override;

		virtual void Move(float xold, float yold, float xnew, float ynew) override;

		virtual void Turn(float xoffset, float yoffset) override;

		virtual void Scale(float yoffset, float xpos, float ypos);

		virtual void Update() override;

		virtual void Reset() override;

		virtual void SetRotate(bool bInOut) override { m_bRotate = bInOut; }
		virtual void SetMove(bool bInOut) override { m_bMove = bInOut; }

		virtual void SetRotateSensitivity(float sensitivity) override { m_RotateSensitivity = sensitivity; }
		virtual void SetMoveSensitivity(float sensitivity) override { m_MoveSensitivity = sensitivity; }

		virtual void SetFrustum(float left, float right, float bottom, float top) override;

		virtual void SetViewport(int x, int y, int width, int height) override;

	protected:
		bool m_bRotate = false;
		bool m_bMove = false;

		float m_RotateSensitivity = 0.1f;
		float m_MoveSensitivity = 0.01f;

		float m_Scale;
		mat4 m_ViewportMatrix;
	};


	//==============================================================================================================


	class OrthoCamera : public Camera
	{
	public:
		OrthoCamera(Frustum frustum, vec3 position, vec3 worldUp = vec3(0.0f, 1.0f, 0.0f));

		virtual mat4 GetViewMatrix() const override;

		virtual mat4 GetProjectionMatrix() const override;

		virtual void Move(float xoffset, float yoffset) override;

		virtual void Turn(float xoffset, float yoffset) override;

		virtual void Scale(float yoffset, float xpos, float ypos, float sW, float sH) override;

		virtual void Update() override;

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