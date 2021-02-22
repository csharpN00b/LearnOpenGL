
#include "Renderer.h"

namespace E6
{
	void RenderScene(GLFWwindow*)
	{
		Logl::Window window(800, 600, "Hello OpenGL");
		Logl::Renderer renderer(&window, false);

        float vertices[] = {
           -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
           -0.5f,  0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f,

           -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f,
           -0.5f, -0.5f,  0.5f,

           -0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f,
           -0.5f, -0.5f, -0.5f,
           -0.5f, -0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,

           -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
           -0.5f, -0.5f,  0.5f,
           -0.5f, -0.5f, -0.5f,

           -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f,  0.5f,
           -0.5f,  0.5f, -0.5f,
        };

        Logl::VertexBuffer vbo(vertices, sizeof(vertices), { {GL_FLOAT, 3} });

        // object
		Logl::VertexArray objectVao;
        objectVao.AddVertexBuffer(vbo);

        Logl::Shader objectShader("asserts/shaders/object_vs.glsl", "asserts/shaders/object_fs.glsl");
        objectShader.Use();
        objectShader.SetUniform3f("objectColor", 1.0f, 0.5f, 0.31f);
        objectShader.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f); // 0.0f, 1.0f, 0.0f

        Logl::RenderObject object(objectVao, objectShader, Logl::mat4());
        renderer.AddObject(object);


        // light
        Logl::VertexArray lightVao;
        lightVao.AddVertexBuffer(vbo);

        Logl::Shader lightShader("asserts/shaders/object_vs.glsl", "asserts/shaders/light_fs.glsl");

        auto lightModel = Logl::mat4::Translate(Logl::vec3(1.2f, 1.0f, 2.0f));
        lightModel = lightModel * Logl::mat4::Scale(0.2f);

        Logl::RenderObject light(lightVao, lightShader, lightModel);
        renderer.AddObject(light);


		renderer.Render(Logl::vec3(0.1f, 0.1f, 0.1f));
	}
}
