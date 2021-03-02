#pragma once

#include <glad/glad.h>

#include <vector>

#include "Window/Window.h"
#include "Window/MouseEvent.h"
#include "Window/KeyEvent.h"
#include "Window/WindowEvent.h"

#include "Renderer/Camera.h"

#include "RenderObject.h"

namespace Logl
{
	class Renderer
	{
	public:
		Renderer(Window* window, bool bOrtho);

		~Renderer();

		void EnableDepthTest(GLenum func = GL_LESS);

		void EnableBlend(GLenum sfactor = GL_SRC_ALPHA, GLenum dfactor = GL_ONE_MINUS_SRC_ALPHA);

		void SetCameraPos(vec3 pos);

		vec3 GetCameraPos() const;

		void AddObject(RenderObject& obj);

		void Render(vec3 backgroudColor = vec3(0.2f, 0.3f, 0.3f));

		void RenderScreen(vec3 backgroudColor = vec3(0.2f, 0.3f, 0.3f));

		void processInput();

	public:
		void OnEvent(Event& event);

		bool OnMouseMove(MouseMovedEvent event);
		bool OnMouseScroll(MouseScrolledEvent event);
		bool OnKeyPress(KeyPressedEvent event);
		bool OnKeyRelease(KeyReleasedEvent event);
		bool OnMouseButtonPress(MouseButtonPressedEvent event);
		bool OnMouseButtonRelease(MouseButtonReleasedEvent event);
		bool OnWindowResize(WindowResizeEvent event);

	private:
		Window* m_Window;
		Camera* m_Camera;
		bool m_EnableTransparent;
		bool m_Running;

	private:
		struct RealTimeState
		{
			float deltaTime; // time between current frame and last frame
			float lastFrame;

			bool firstMouse;
			float lastX;
			float lastY;

			RealTimeState()
				: deltaTime(0.0f), lastFrame(0.0f), firstMouse(true), lastX(0.0f), lastY(0.0f)
			{}

			void UpdateTime(float currentFrame)
			{
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;
			}

			std::pair<float, float> UpdatePos(float xpos, float ypos)
			{
				if (firstMouse)
				{
					lastX = xpos;
					lastY = ypos;
					firstMouse = false;
					return { 0.0f, 0.0f };
				}

				float xoffset = xpos - lastX;
				float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
				lastX = xpos;
				lastY = ypos;

				return { xoffset, yoffset };
			}
		};

		RealTimeState m_State;

	private:
		std::vector<RenderObject*> m_Objects;
		std::vector<TransparentModel> m_TransparentList;
	};
}