#pragma once

#include <glad/glad.h>

#include <vector>

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

	struct RenderObject
	{
		typedef void(*DynamicUniform)(Shader* shader, float time, Camera* camera);

		VertexArray* vao;
		Shader* shader;
		DynamicUniform dynamicUniform;
		std::vector<mat4> models;
		std::vector<Texture2D*> textures;

		RenderObject(VertexArray& pvao, Shader& pshader, DynamicUniform func)
			: vao(&pvao), shader(&pshader), dynamicUniform(func)
		{
		}

		RenderObject(VertexArray& pvao, Shader& pshader, mat4 model)
			: vao(&pvao), shader(&pshader), dynamicUniform(nullptr)
		{
			models.emplace_back(model);
		}

		void AddModel(const mat4& mat) { models.emplace_back(mat); }

		void AddTexture(Texture2D* texture) { textures.emplace_back(texture); }
	};


	class Renderer
	{
	public:
		Renderer(Window* window, bool bOrtho);

		~Renderer();

		void EnableDepthTest();

		void SetCameraPos(vec3 pos);

		vec3 GetCameraPos() const;

		void AddObject(RenderObject& obj);

		void Render(vec3 backgroudColor = vec3(0.2f, 0.3f, 0.3f));

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
		std::vector<RenderObject*> m_Objects;
	};
}