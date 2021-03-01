#include "Renderer.h"

namespace E14
{
    using namespace Logl;

    void RenderScene()
    {
        Window window(800, 600);
        Renderer renderer(&window, false);
        renderer.EnableDepthTest();
        renderer.EnableBlend();

        float cubeVertices[] = {
            // positions          // texture Coords
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };

        float planeVertices[] = {
            // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
             5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

             5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
            -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
             5.0f, -0.5f, -5.0f,  2.0f, 2.0f
        };

        float vegetationVertices[] = {
            // positions         // texture Coords
            0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.0f,  0.5f,  0.0f,  0.0f,  1.0f,

            0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
            1.0f,  0.5f,  0.0f,  1.0f,  1.0f
        };

        vec3 vegetationPositions[5] = {
            vec3(-1.5f, 0.0f, -0.48f),
            vec3(1.5f, 0.0f, 0.51f),
            vec3(0.0f, 0.0f, 0.7f),
            vec3(-0.3f, 0.0f, -2.3f),
            vec3(0.5f, 0.0f, -0.6f)
        };

        VertexArray cubeVao, planeVao, vegetationVao;
        VertexBuffer cubeVbo(cubeVertices, sizeof(cubeVertices), { {GL_FLOAT, 3}, {GL_FLOAT, 2} });
        cubeVao.AddVertexBuffer(cubeVbo);
        VertexBuffer planeVbo(planeVertices, sizeof(planeVertices), { {GL_FLOAT, 3}, {GL_FLOAT, 2} });
        planeVao.AddVertexBuffer(planeVbo);
        VertexBuffer vegetationVbo(vegetationVertices, sizeof(vegetationVertices), { {GL_FLOAT, 3}, {GL_FLOAT, 2} });
        vegetationVao.AddVertexBuffer(vegetationVbo);


        Shader shader("asserts/shaders/depth_testing_vs.glsl", "asserts/shaders/depth_testing_fs.glsl");
        Shader shader1("asserts/shaders/depth_testing_vs.glsl", "asserts/shaders/depth_testing_fs.glsl"); 
        Shader shaderTransparent("asserts/shaders/depth_testing_vs.glsl", "asserts/shaders/blending_fs.glsl");

        RenderObject cube(cubeVao, shader, nullptr);
        RenderObject plane(planeVao, shader1, mat4::Translate(vec3(0.0f, -0.001f, 0.0f)));
        RenderObject vegetation(vegetationVao, shaderTransparent, nullptr, true);

        cube.AddModel(mat4::Translate(vec3(-1.0f, 0.0f, -1.0f)));
        cube.AddModel(mat4::Translate(vec3(2.0f, 0.0f, 0.0f)));

        for (int i = 0; i < 5; i++)
        {
            vegetation.AddModel(mat4::Translate(vegetationPositions[i]));
        }

        Texture2D cubeTexture("asserts/textures/marble.jpg");
        Texture2D floorTexture("asserts/textures/metal.png");
        Texture2D grassTexture("asserts/textures/grass.png");
        Texture2D windowTexture("asserts/textures/blending_transparent_window.png");
        cubeTexture.Bind(0);
        floorTexture.Bind(1);
        grassTexture.Bind(2); // grass
        //windowTexture.Bind(2);  // window

        shader.Use();
        shader.SetUniform("texture1", 0);
        shader1.Use();
        shader1.SetUniform("texture1", 1);
        shaderTransparent.Use();
        shaderTransparent.SetUniform("texture1", 2);

        renderer.AddObject(cube);
        renderer.AddObject(plane);
        renderer.AddObject(vegetation);

        renderer.Render(vec3(0.1));
    }
}