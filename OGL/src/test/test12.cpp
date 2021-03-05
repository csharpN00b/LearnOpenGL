#include "Renderer.h"

#define HORROR 0
#define BIOCHEMICAL_LAB 1

#define SPOTLIGHT 0

namespace E12
{
    struct DirectionalLight
    {
        Logl::vec3 direction;

        Logl::vec3 ambient;
        Logl::vec3 diffuse;
        Logl::vec3 specular;
    };

    struct PointLight
    {
        Logl::vec3 position;
        Logl::vec3 color;

        float constant;
        float linear;
        float quadratic;
    };

    struct Spotlight
    {
        Logl::vec3 ambient;
        Logl::vec3 diffuse;
        Logl::vec3 specular;

        float constant;
        float linear;
        float quadratic;

        float cutOff;
        float outerCutOff;
    };

    struct LightEnv
    {
        Logl::vec3 backcolor;
        DirectionalLight directionalLight;
        PointLight pointLight;
        Spotlight spotlight;
    };

    void RenderScene()
    {
        using namespace Logl;

        Window window(800, 600);
        bool bUseOrthoCamera = false;
        Renderer renderer(&window, bUseOrthoCamera);
        if (!bUseOrthoCamera)
            renderer.SetCameraPos(vec3(0.0f, 0.0f, 3.0f));

#pragma region Light Enviroment

#if HORROR
        LightEnv env = {
            vec3(0.0f),
            {
                vec3(-0.2f, -1.0f, -0.3f),
                vec3(0.0f),
                vec3(0.05f),
                vec3(0.2f),
            },
            {
                vec3(1.2f, 1.0f, 2.0f),
                vec3(0.3f, 0.1f, 0.1f),
                1.0,
                0.14f,
                0.07f,
            },
            {
                vec3(0.0f),
                vec3(1.0f),
                vec3(1.0f),
                1.0f,
                0.09f,
                0.032f,
                10.0f,
                15.0f
            }
        };

#elif BIOCHEMICAL_LAB
        LightEnv env = {
            vec3(0.9f),
            {
                vec3(-0.2f, -1.0f, -0.3f),
                vec3(0.5f),
                vec3(1.0f),
                vec3(1.0f),
            },
            {
                vec3(1.2f, 1.0f, 2.0f),
                vec3(0.4f, 0.7f, 0.1f),
                1.0,
                0.07,
                0.017,
            },
            {
                vec3(0.0f),
                vec3(0.0f, 1.0f, 0.0f),
                vec3(0.0f, 1.0f, 0.0f),
                1.0f,
                0.07f,
                0.017f,
                7.0f,
                10.0f
            }
        };

#else
        LightEnv env = {
            vec3(0.1f),
            {
                vec3(-0.2f, -1.0f, -0.3f),
                vec3(0.2f),
                vec3(0.5f),
                vec3(1.0f),
            },
            {
                vec3(1.2f, 1.0f, 2.0f),
                vec3(1.0f, 1.0f, 1.0f),
                1.0,
                0.09f,
                0.032f,
            },
            {
                vec3(0.2f),
                vec3(0.5f),
                vec3(1.0f),
                1.0f,
                0.09f,
                0.032f,
                12.5f,
                17.5f
            }
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

        VertexArray lightVao;
        VertexBuffer lightVbo(lightVertices, sizeof(lightVertices), { {GL_FLOAT, 3} });
        lightVao.AddVertexBuffer(lightVbo);

        Shader lightShader("asserts/shaders/mvp_color.glsl");
        RenderObject light(lightVao, lightShader, mat4::Translate(env.pointLight.position) * mat4::Scale(0.2f));
        renderer.AddObject(light);
        lightShader.Use();
        lightShader.SetUniform("color", env.pointLight.color);


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

        Shader shader("asserts/shaders/LightSceneObject.glsl");
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
            shader->SetUniform("viewPos", camera->GetPosition());

#if SPOTLIGHT
            shader->SetUniform("spotlight.position", camera->GetPosition());
            shader->SetUniform("spotlight.direction", camera->GetFront());
#endif
        };

        shader.Use();
        shader.SetUniform("material.diffuse", 0);
        shader.SetUniform("material.specular", 1);
        shader.SetUniform("material.shininess", 32.0f);

        // Directional Light
        shader.SetUniform3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
        shader.SetUniform("dirLight.ambient", env.directionalLight.ambient);
        shader.SetUniform("dirLight.diffuse", env.directionalLight.diffuse);
        shader.SetUniform("dirLight.specular", env.directionalLight.specular);

#if SPOTLIGHT
        // Spotlight
        shader.SetUniform("spotlight.cutOff", cos(Radians(env.spotlight.cutOff)));
        shader.SetUniform("spotlight.outerCutOff", cos(Radians(env.spotlight.outerCutOff)));
        shader.SetUniform("spotlight.constant", env.spotlight.constant);
        shader.SetUniform("spotlight.linear", env.spotlight.linear);
        shader.SetUniform("spotlight.quadratic", env.spotlight.quadratic);

        shader.SetUniform("spotlight.ambient", env.spotlight.ambient);
        shader.SetUniform("spotlight.diffuse", env.spotlight.diffuse);
        shader.SetUniform("spotlight.specular", env.spotlight.specular);
#endif
        // Point Lights
        shader.SetUniform("pointLight.position", env.pointLight.position);

        shader.SetUniform("pointLight.constant", env.pointLight.constant);
        shader.SetUniform("pointLight.linear", env.pointLight.linear);
        shader.SetUniform("pointLight.quadratic", env.pointLight.quadratic);

        shader.SetUniform("pointLight.ambient", env.pointLight.color * 0.1);
        shader.SetUniform("pointLight.diffuse", env.pointLight.color /** 0.5*/);
        shader.SetUniform("pointLight.specular", env.pointLight.color);


        renderer.EnableDepthTest();
        renderer.Render(env.backcolor);
    }
}