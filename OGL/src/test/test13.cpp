#include "Renderer.h"

namespace E13
{
	using namespace Logl;

	void RenderScene()
	{
		Window window(800, 600);
		Renderer renderer(&window, false);

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

        VertexArray cubeVao, planeVao;
        VertexBuffer cubeVbo(cubeVertices, sizeof(cubeVertices), { {GL_FLOAT, 3}, {GL_FLOAT, 2} });
        cubeVao.AddVertexBuffer(cubeVbo);
        VertexBuffer planeVbo(planeVertices, sizeof(planeVertices), { {GL_FLOAT, 3}, {GL_FLOAT, 2} });
        planeVao.AddVertexBuffer(planeVbo);

#if 0
        // share the same shader, add texture to RenderObject (switch texture bind in render loop)

        Shader shader("asserts/shaders/depth_testing_vs.glsl", "asserts/shaders/depth_testing_fs.glsl");
        RenderObject cube(cubeVao, shader, nullptr);
        //RenderObject plane(planeVao, shader, mat4());
        RenderObject plane(planeVao, shader, mat4::Translate(vec3(0.0f, -0.001f, 0.0f))); // prevent z-fighting

        cube.AddModel(mat4::Translate(vec3(-1.0f, 0.0f, -1.0f)));
        cube.AddModel(mat4::Translate(vec3(2.0f, 0.0f, 0.0f)));

        Texture2D cubeTexture("asserts/textures/marble.jpg");
        Texture2D floorTexture("asserts/textures/metal.png");
        cube.AddTexture(&cubeTexture);
        plane.AddTexture(&floorTexture);

        shader.Use();
        shader.SetUniform("texture1", 0);
#else
        // use two shaders, bind textures to different texture units (bind before the loop starts)

        Shader shader("asserts/shaders/depth_testing_vs.glsl", "asserts/shaders/depth_testing_fs.glsl");
        Shader shader1("asserts/shaders/depth_testing_vs.glsl", "asserts/shaders/depth_testing_fs.glsl"); // use another Shader
        RenderObject cube(cubeVao, shader, nullptr);
        //RenderObject plane(planeVao, shader1, mat4());
        RenderObject plane(planeVao, shader1, mat4::Translate(vec3(0.0f, -0.001f, 0.0f))); // prevent z-fighting

        cube.AddModel(mat4::Translate(vec3(-1.0f, 0.0f, -1.0f)));
        cube.AddModel(mat4::Translate(vec3(2.0f, 0.0f, 0.0f)));

        Texture2D cubeTexture("asserts/textures/marble.jpg");
        Texture2D floorTexture("asserts/textures/metal.png");
        cubeTexture.Bind(0);
        floorTexture.Bind(1);

        shader.Use();
        shader.SetUniform("texture1", 0);
        shader1.Use();
        shader1.SetUniform("texture1", 1);
#endif

        renderer.AddObject(cube);
        renderer.AddObject(plane);

        renderer.EnableDepthTest();
        //glDepthFunc(GL_ALWAYS); // default depth function: GL_LESS
        renderer.Render(vec3(0.1));

	}
}