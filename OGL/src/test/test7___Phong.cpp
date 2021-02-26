#include "Renderer.h"

namespace E7
{
	void RenderScene()
	{
		using namespace Logl;

		Window window(800, 600);
        bool bUseOrthoCamera = false;
		Renderer renderer(&window, bUseOrthoCamera);
        if (!bUseOrthoCamera)
            renderer.SetCameraPos(vec3(0.0f, 0.0f, 3.0f));

        float lightVertices[] = {
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

        vec3 lightPos(1.2f, 1.0f, 2.0f);
        VertexArray lightVao;
        VertexBuffer lightVbo(lightVertices, sizeof(lightVertices), { {GL_FLOAT, 3} });
        lightVao.AddVertexBuffer(lightVbo);

        Shader lightShader("asserts/shaders/light_vs.glsl", "asserts/shaders/light_fs.glsl");

        auto lightModel = mat4::Translate(lightPos);
        lightModel = lightModel * mat4::Scale(0.2f);

        RenderObject light(lightVao, lightShader, lightModel);
        renderer.AddObject(light);


        float vertices[] = {
          -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
           0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

          -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
           0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
          -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
          -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

          -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
          -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

           0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
           0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
           0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
           0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
           0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
           0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

          -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
           0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
           0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
           0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
          -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
          -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

          -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
           0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };
        VertexArray vao;
        VertexBuffer vbo(vertices, sizeof(vertices), { {GL_FLOAT, 3}, {GL_FLOAT, 3} });
        vao.AddVertexBuffer(vbo);

        Shader shader("asserts/shaders/phong_vs.glsl", "asserts/shaders/phong_fs.glsl");
        RenderObject obj(vao, shader, mat4());
        obj.dynamicUniform = [](Shader* shader, float time, Camera* camera)
        {
            shader->SetUniform("viewPos", camera->GetPosition());
        };
        renderer.AddObject(obj);

        shader.Use();
        shader.SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
        shader.SetUniform("lightPos", lightPos);
        shader.SetUniform3f("objectColor", 1.0f, 0.5f, 0.31f);

        renderer.EnableDepthTest();
        renderer.Render(vec3(0.1f, 0.1f, 0.1f));
	}
}