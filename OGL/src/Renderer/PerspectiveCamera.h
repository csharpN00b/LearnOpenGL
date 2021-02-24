#pragma once

#include "Camera.h"

namespace Logl
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(Frustum frustum, vec3 position, vec3 worldUp = vec3(0.0f, 1.0f, 0.0f));

		virtual CameraType Type() const override;

		mat4 GetViewMatrix() const override;

		mat4 GetProjectionMatrix() const override;

		virtual void Move(MoveDirection direction, float deltaTime) override;

		virtual void Turn(float xoffset, float yoffset) override;

		virtual void Scale(float yoffset) override;

		virtual void Update() override;

		virtual void Reset() override;

		virtual void SetMoveSpeed(float speed) override { m_MovementSpeed = speed; }
		virtual void SetRotateSensitivity(float sensitivity) override { m_MouseSensitivity = sensitivity; }

		virtual void SetFrustum(float left, float right, float bottom, float top) override;

	protected:
		float m_MovementSpeed;
		float m_MouseSensitivity;
	};
}