#include "Renderer.h"

#define DESERT 1
#define FACTORY 0
#define HORROR 0
#define BIOCHEMICAL_LAB 0

namespace E12_
{
    void RenderScene()
    {
        using namespace Logl;

        Window window(800, 600);
        bool bUseOrthoCamera = false;
        Renderer renderer(&window, bUseOrthoCamera);
        if (!bUseOrthoCamera)
            renderer.SetCameraPos(vec3(0.0f, 0.0f, 3.0f));

#pragma region Light Enviroment

        struct LightEnv
        {
            vec3 dirLightA;
            vec3 dirLightD;
            vec3 dirLightS;

            vec3 spotlightA;
            vec3 spotlightD;
            vec3 spotlightS;

            float linear1;    // point light
            float quadratic1; // point light

            float linear2;    // spotlight
            float quadratic2; // spotlight

            float cutOff;
            float outerCutOff;
        };

#if DESERT
        vec3 env(0.75f, 0.52f, 0.3f);

        vec3 pointLightColors[] = {
            vec3(1.0f, 0.6f, 0.0f),
            vec3(1.0f, 0.0f, 0.0f),
            vec3(1.0f, 1.0, 0.0),
            vec3(0.2f, 0.2f, 1.0f)
        };

        LightEnv lightEnv = {
           vec3(0.3f, 0.24f, 0.14f),
           vec3(0.7f, 0.42f, 0.26f),
           vec3(0.5f),

           vec3(0.0f),
           vec3(0.8f, 0.8f, 0.0f),
           vec3(0.8f, 0.8f, 0.0f),

           0.09f,
           0.032f,

           0.09f,
           0.032f,

           12.5f,
           13.0f
        };

#elif FACTORY
        vec3 env(0.1f, 0.1f, 0.1f);

        vec3 pointLightColors[] = {
            vec3(0.2f, 0.2f, 0.6f),
            vec3(0.3f, 0.3f, 0.7f),
            vec3(0.0f, 0.0f, 0.3f),
            vec3(0.4f, 0.4f, 0.4f)
        };

        LightEnv lightEnv = {
           vec3(0.05f, 0.05f, 0.1f),
           vec3(0.2f, 0.2f, 0.7),
           vec3(0.7f),

           vec3(0.0f),
           vec3(1.0f),
           vec3(1.0f),

           0.09f,
           0.032f,

           0.009f,
           0.0032f,

           10.0f,
           12.5f
        };

#elif HORROR
        vec3 env(0.0f, 0.0f, 0.0f);

        vec3 pointLightColors[] = {
            vec3(0.1f, 0.1f, 0.1f),
            vec3(0.1f, 0.1f, 0.1f),
            vec3(0.1f, 0.1f, 0.1f),
            vec3(0.3f, 0.1f, 0.1f)
        };

        LightEnv lightEnv = {
           vec3(0.0f),
           vec3(0.05f),
           vec3(0.2f),

           vec3(0.0f),
           vec3(1.0f),
           vec3(1.0f),

           0.14f,
           0.07f,

           0.09f,
           0.032f,

           10.0f,
           15.0f
        };

#elif BIOCHEMICAL_LAB
        vec3 env(0.9f, 0.9f, 0.9f);

        vec3 pointLightColors[] = {
            vec3(0.4f, 0.7f, 0.1f),
            vec3(0.4f, 0.7f, 0.1f),
            vec3(0.4f, 0.7f, 0.1f),
            vec3(0.4f, 0.7f, 0.1f)
        };

        LightEnv lightEnv = {
           vec3(0.5f),
           vec3(1.0f),
           vec3(1.0f),

           vec3(0.0f),
           vec3(0.0f, 1.0f, 0.0f),
           vec3(0.0f, 1.0f, 0.0f),

           0.07,
           0.017,

           0.07f,
           0.017f,

           7.0f,
           10.0f
        };

#else
        vec3 env(0.1f, 0.1f, 0.1f);

        vec3 pointLightColors[] = {
            vec3(1.0f, 1.0f, 1.0f),
            vec3(1.0f, 1.0f, 1.0f),
            vec3(1.0f, 1.0f, 1.0f),
            vec3(1.0f, 1.0f, 1.0f)
        };

        LightEnv lightEnv = {
            vec3(0.2f),
            vec3(0.5f),
            vec3(1.0f),

            vec3(0.2f),
            vec3(0.5f),
            vec3(1.0f),

            0.09f,
            0.032f,

            0.09f,
            0.032f,

            12.5f,
            17.5f
        };
#endif

#pragma endregion


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

        vec3 pointLightPositions[4] = {
           vec3(0.7f,  0.2f,  2.0f),
           vec3(2.3f, -3.3f, -4.0f),
           vec3(-4.0f,  2.0f, -12.0f),
           vec3(0.0f,  0.0f, -3.0f)
        };

        VertexArray lightVao;
        VertexBuffer lightVbo(lightVertices, sizeof(lightVertices), { {GL_FLOAT, 3} });
        lightVao.AddVertexBuffer(lightVbo);

        Shader lightShader("asserts/shaders/mvp_color.glsl");
        RenderObject light(lightVao, lightShader, nullptr);
        renderer.AddObject(light);

        for (int i = 0; i < 4; i++)
        {
            auto model = mat4::Translate(pointLightPositions[i]);
            model = model * mat4::Scale(0.2f);
            light.AddModel(model, pointLightColors[i]);
        }
        

        float cubeVertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
        };

        vec3 cubePositions[10] = {
        vec3(0.0f,  0.0f,  0.0f),
        vec3(2.0f,  5.0f, -15.0f),
        vec3(-1.5f, -2.2f, -2.5f),
        vec3(-3.8f, -2.0f, -12.3f),
        vec3(2.4f, -0.4f, -3.5f),
        vec3(-1.7f,  3.0f, -7.5f),
        vec3(1.3f, -2.0f, -2.5f),
        vec3(1.5f,  2.0f, -2.5f),
        vec3(1.5f,  0.2f, -1.5f),
        vec3(-1.3f,  1.0f, -1.5f)
        };

        VertexArray vao;
        VertexBuffer vbo(cubeVertices, sizeof(cubeVertices), { {GL_FLOAT, 3}, {GL_FLOAT, 3}, {GL_FLOAT, 2} });
        vao.AddVertexBuffer(vbo);

        Shader shader("asserts/shaders/lighting_maps_vs.glsl", "asserts/shaders/multiple_lights_fs.glsl");
        RenderObject cube(vao, shader, nullptr);
        renderer.AddObject(cube);

        for (int i = 0; i < 10; i++)
        {
            auto model = mat4::Translate(cubePositions[i]);
            float angle = 20.0f * i;
            model = model * mat4::Rotate(Radians(angle), vec3(1.0f, 0.3f, 0.5f));
            cube.AddModel(model);
        }

        Texture2D diffuseMap("asserts/textures/container2.png");
        Texture2D specularMap("asserts/textures/container2_specular.png");
        cube.AddTexture(&diffuseMap);
        cube.AddTexture(&specularMap);

       

        cube.dynamicUniform = [](Shader* shader, float time, Camera* camera)
        {
            shader->SetFloat3("viewPos", camera->GetPosition());

            shader->SetFloat3("spotlight.position", camera->GetPosition());
            shader->SetFloat3("spotlight.direction", camera->GetFront());
        };

        shader.Use();
        shader.SetInt("material.diffuse", 0);
        shader.SetInt("material.specular", 1);
        shader.SetFloat("material.shininess", 32.0f);

        // Directional Light
        shader.SetFloat3("dirLight.direction", vec3(-0.2f, -1.0f, -0.3f));
        shader.SetFloat3("dirLight.ambient", lightEnv.dirLightA);
        shader.SetFloat3("dirLight.diffuse", lightEnv.dirLightD);
        shader.SetFloat3("dirLight.specular", lightEnv.dirLightS);

        // Spotlight
        shader.SetFloat("spotlight.cutOff", cos(Radians(lightEnv.cutOff)));
        shader.SetFloat("spotlight.outerCutOff", cos(Radians(lightEnv.outerCutOff)));
        shader.SetFloat("spotlight.constant", 1.0f);
        shader.SetFloat("spotlight.linear", lightEnv.linear2);
        shader.SetFloat("spotlight.quadratic", lightEnv.quadratic2);

        shader.SetFloat3("spotlight.ambient", lightEnv.spotlightA);
        shader.SetFloat3("spotlight.diffuse", lightEnv.spotlightD);
        shader.SetFloat3("spotlight.specular", lightEnv.spotlightS);

        // Point Lights
        for (int i = 0; i < 4; i++)
        {
            shader.SetFloat3(fmt::format("pointLights[{}].position", i), pointLightPositions[i]);

            shader.SetFloat(fmt::format("pointLights[{}].constant", i), 1.0f);
            shader.SetFloat(fmt::format("pointLights[{}].linear", i), lightEnv.linear1);
            shader.SetFloat(fmt::format("pointLights[{}].quadratic", i), lightEnv.quadratic1);

            shader.SetFloat3(fmt::format("pointLights[{}].ambient", i), pointLightColors[i] * 0.1);
            shader.SetFloat3(fmt::format("pointLights[{}].diffuse", i), pointLightColors[i] /** 0.5*/);
            shader.SetFloat3(fmt::format("pointLights[{}].specular", i), pointLightColors[i]);
        }


        renderer.EnableDepthTest();
        renderer.Render(env);
    }
}