
#include "Renderer.h"

namespace E5
{
	void RenderScene(GLFWwindow*)
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

		Logl::VertexBuffer vbo(vertices, sizeof(vertices));
		Logl::IndexBuffer ibo(indices, sizeof(indices));

		Logl::BufferLayout bufferLayout =
		{
			{GL_FLOAT, 3},
			{GL_FLOAT, 3},
			{GL_FLOAT, 2},
		};
		vbo.SetBufferLayout(bufferLayout);

		vao.AddVertexBuffer(vbo);
		vao.SetIndexBuffer(ibo);
		renderer.SetVexterArray(vao);

		Logl::Shader shader("asserts/shaders/basic_vs.glsl", "asserts/shaders/basic_fs.glsl");
		renderer.SetShader(shader);

		renderer.Render();
	}
}
