
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Math/Matrix4f.h"

#include "Renderer/PerspectiveCamera.h"
#include "Renderer/OrthographicCamera.h"

#include "Window/Window.h"
#include "Window/MouseEvent.h"
#include "Window/KeyEvent.h"
#include "Window/WindowEvent.h"

#include "Renderer.h"

namespace E15
{
	void RenderScene()
	{
		Logl::Window window(800, 600);
		Logl::Renderer renderer(&window, false);
		renderer.EnableDepthTest();
		renderer.EnableBlend();

		float vertices[] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
		};


		Logl::VertexArray vao;
		Logl::VertexBuffer vbo(vertices, sizeof(vertices), { {GL_FLOAT, 3}, {GL_FLOAT, 2} });
		vao.AddVertexBuffer(vbo);

		Logl::Shader shader("asserts/shaders/depth_testing_vs.glsl", "asserts/shaders/blending_fs.glsl");

		Logl::Texture2D texture("asserts/textures/blending_transparent_window.png");
		texture.Bind(0);

		shader.Use();
		shader.SetInt("texture1", 0);

		Logl::RenderObject object(vao, shader, nullptr, true);

		Logl::vec3 cubePositions[10] = {
		  Logl::vec3(0.0f,  0.0f,  0.0f),
		  Logl::vec3(2.0f,  5.0f, -15.0f),
		  Logl::vec3(-1.5f, -2.2f, -2.5f),
		  Logl::vec3(-3.8f, -2.0f, -12.3f),
		  Logl::vec3(2.4f, -0.4f, -3.5f),
		  Logl::vec3(-1.7f,  3.0f, -7.5f),
		  Logl::vec3(1.3f, -2.0f, -2.5f),
		  Logl::vec3(1.5f,  2.0f, -2.5f),
		  Logl::vec3(1.5f,  0.2f, -1.5f),
		  Logl::vec3(-1.3f,  1.0f, -1.5f)
		};
		for (int i = 0; i < 10; i++)
		{
			auto model = Logl::mat4::Translate(cubePositions[i]);
			float angle = 20.0f * i;
			model = model * Logl::mat4::Rotate(Logl::Radians(angle), Logl::vec3(1.0f, 0.3f, 0.5f));
			object.AddModel(model);
		}

		renderer.AddObject(object);

		renderer.Render(Logl::vec3(1.0f));
	}
}
