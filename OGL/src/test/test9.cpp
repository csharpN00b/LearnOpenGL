#include "Renderer.h"

#define LIGHT_MOVING 0
#define LIGHT_COLOR_CHANGING 0
#define CYAN_PLASTIC 1

namespace E9
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

#if LIGHT_MOVING
        auto dynamicUniform = [](Shader* shader, float time, Camera* camera)
        {
            vec3 lightPos(1.2f, 1.0f, 2.0f);
            lightPos.x = 1.0f + sin(time) * 2.0f;
            lightPos.y = sin(time / 2.0f) * 1.0f;

            auto lightModel = mat4::Translate(lightPos);
            lightModel = lightModel * mat4::Scale(0.2f);
            shader->SetUniform("model", lightModel.ValuePtr());
        };

        RenderObject light(lightVao, lightShader, dynamicUniform);
#else
        auto lightModel = mat4::Translate(lightPos);
        lightModel = lightModel * mat4::Scale(0.2f);

        RenderObject light(lightVao, lightShader, lightModel);
#endif

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

        Shader shader("asserts/shaders/phong_vs.glsl", "asserts/shaders/phong_material_fs.glsl");
        RenderObject obj(vao, shader, mat4());
        renderer.AddObject(obj);

        obj.dynamicUniform = [](Shader* shader, float time, Camera* camera)
        {
            shader->SetUniform("viewPos", camera->GetPosition());

#if LIGHT_COLOR_CHANGING
            vec3 lightColor;
            lightColor.x = sin(time * 2.0f);
            lightColor.y = sin(time * 0.7f);
            lightColor.z = sin(time * 1.3f);

            vec3 diffuseColor = lightColor * vec3(0.5f);
            vec3 ambientColor = diffuseColor * vec3(0.2f);

            shader->SetUniform("light.ambient", ambientColor);
            shader->SetUniform("light.diffuse", diffuseColor);
            shader->SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
#endif

#if LIGHT_MOVING 
            vec3 lightPos(1.2f, 1.0f, 2.0f);
            lightPos.x = 1.0f + sin(time) * 2.0f;
            lightPos.y = sin(time / 2.0f) * 1.0f;
            shader->SetUniform("light.position", lightPos);

            shader->SetUniform("viewPos", camera->GetPosition());
#endif
        };

        shader.Use();
        shader.SetUniform("light.position", lightPos);
      
#if CYAN_PLASTIC

        // light properties
        shader.SetUniform3f("light.ambient", 1.0f, 1.0f, 1.0f); // note that all light colors are set at full intensity
        shader.SetUniform3f("light.diffuse", 1.0f, 1.0f, 1.0f);
        shader.SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);

        // material properties
        shader.SetUniform3f("material.ambient", 0.0f, 0.1f, 0.06f);
        shader.SetUniform3f("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
        shader.SetUniform3f("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
        shader.SetUniform("material.shininess", 32.0f);

#else
        shader.SetUniform3f("light.ambient", 0.2f, 0.2f, 0.2f); // decrease the influence
        shader.SetUniform3f("light.diffuse", 0.5f, 0.5f, 0.5f); // low influence
        shader.SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);

        shader.SetUniform3f("material.ambient", 1.0f, 0.5f, 0.31f);
        shader.SetUniform3f("material.diffuse", 1.0f, 0.5f, 0.31f);
        shader.SetUniform3f("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
        shader.SetUniform("material.shininess", 32.0f);
#endif

        renderer.EnableDepthTest();
        renderer.Render(vec3(0.1f, 0.1f, 0.1f));
    }
}