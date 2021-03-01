#include "Renderer.h"

void TestBlend()
{
	Logl::Window window(800, 600);
	Logl::Renderer renderer(&window, true);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,

		 0.5f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
	};

	Logl::VertexArray vao;
	Logl::VertexBuffer vbo(vertices, sizeof(vertices), { {GL_FLOAT, 3} });
	vao.AddVertexBuffer(vbo);

	Logl::Shader shader("asserts/shaders/object_vs.glsl", "asserts/shaders/object_rgba_fs.glsl");
	Logl::RenderObject object(vao, shader, nullptr);
	object.AddModel(Logl::mat4(), Logl::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//object.AddModel(Logl::mat4(), Logl::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	//object.AddModel(Logl::mat4(), Logl::vec4(0.0f, 1.0f, 0.0f, 0.6f));
	object.AddModel(Logl::mat4::Translate(Logl::vec3(0.3f, -0.3f, 0.0f)), Logl::vec4(0.0f, 1.0f, 0.0f, 0.6f));

	renderer.AddObject(object);

	renderer.EnableBlend();
	renderer.Render(Logl::vec3(1.0f));
}

void TestRender()
{
	TestBlend();
}
