
#include "Renderer.h"

namespace E5
{
	void RenderScene()
	{
		Logl::Window window(800, 600, "Hello OpenGL");
		Logl::Renderer renderer(&window, true);

		float vertices[] =
		{
			// positions         // colors          // texture coords
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f
		};

		unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0,
		};

		Logl::VertexArray vao;
		Logl::VertexBuffer vbo(vertices, sizeof(vertices), { {GL_FLOAT, 3}, {GL_FLOAT, 3}, {GL_FLOAT, 2} });
		Logl::IndexBuffer ibo(indices, sizeof(indices));

		vao.AddVertexBuffer(vbo);
		vao.SetIndexBuffer(ibo);

		Logl::Shader shader("asserts/shaders/color_mvp.glsl");
		Logl::RenderObject object(vao, shader, Logl::mat4());
		renderer.AddObject(object);

		renderer.Render();
	}
}
