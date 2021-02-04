#pragma once

#include <glad/glad.h>

#include "Window/Window.h"
#include "Window/MouseEvent.h"
#include "Window/KeyEvent.h"
#include "Window/WindowEvent.h"

#include "Renderer/Camera.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"

namespace Logl
{
	class Renderer
	{
	public:
		typedef void(*DrawCallFunc)(int);
		
		Renderer(Window* window, bool bOrtho);

		~Renderer();

		void EnableDepthTest();

		void SetShader(Shader& shader);
		void SetVexterArray(VertexArray& vao);

		void Render();

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
		Window* m_window;
		Camera* m_Camera;
		bool m_Running;

	private:
		struct TmpParam
		{
			float deltaTime; // time between current frame and last frame
			float lastFrame;

			bool firstMouse;
			float lastX;
			float lastY;

			TmpParam()
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

		TmpParam m_tmpParam;

	private:
		Shader* m_Shader;
		VertexArray* m_Vao;
	};
}