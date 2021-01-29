#if 0

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <unordered_map>

#include "../FMT.h"

#include "../Math/Matrix4f.h"

#include "../Renderer/Shader.h"
#include "../Renderer/PerspectiveCamera.h"
#include "../Renderer/OrthographicCamera.h"

#include "../Window/Window.h"
#include "../Window/MouseEvent.h"
#include "../Window/KeyEvent.h"

#include "../GLFuncs.h"


using namespace Logl;

namespace E4
{
	void OnEvent(Event& event)
	{

	}

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	bool firstMouse;
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;

	// camera
	float w = SCR_WIDTH / 100.0f;
	float h = SCR_HEIGHT / 100.0f;
	float ratio = w / h;

#if 1
	PerspectiveCamera camera(Frustum(ratio, 45.0f, 0.1f, 100.0f), vec3(0.0f, 0.0f, 3.0f));
#else
	OrthoGraphicCamera camera(Frustum(-w / 2.0f, w / 2.0f, -h / 2.0f, h / 2.0f, 0.1f, 100.0f), vec3(0.0f, 0.0f, 3.0f));
#endif


	int CubeData(unsigned int& vao, unsigned int& vbo, unsigned int& ebo)
	{
#if 0
		float vertices[] =
		{
			// positions         // colors          // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,

			-0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, 0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f
		};

		unsigned int indices[] =
		{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			0, 4, 7, 7, 3, 0,
			5, 1, 2, 2, 6, 5,
			4, 5, 1, 1, 0, 4,
			7, 6, 2, 2, 3, 7
		};
#endif

		float vertices[] = {
			-0.5f, -0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f
		};

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return sizeof(vertices) / sizeof(vertices[0]) / 8;
	}

	void RenderScene(Window& window)
	{
		window.SetEventCallback(std::bind(OnEvent, std::placeholders::_1));

		glEnable(GL_DEPTH_TEST);


		// Shaders
		Shader shaderProgram("asserts/shaders/mvp_vs.glsl", "asserts/shaders/mvp_fs.glsl");
		if (!shaderProgram.IsValid())
		{
			__debugbreak();
			return;
		}

		// Vertex
		unsigned int vao{}, vbo{}, ebo{};
		int count = CubeData(vao, vbo, ebo);

		// Texture
		unsigned int texture1 = LoadTexture("asserts/textures/container.jpg", GL_RGB);
		unsigned int texture2 = LoadTexture("asserts/textures/awesomeface.png", GL_RGBA);
		if (texture1 == 0 || texture2 == 0)
		{
			PRINT("Faild to load texture!\n");
			__debugbreak();
			return;
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shaderProgram.Use();
		shaderProgram.SetUniform("texture1", 0);
		shaderProgram.SetUniform("texture2", 1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

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

		// Loop
		while (!glfwWindowShouldClose(window))
		{
			// per-frame time logic
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// render
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shaderProgram.Use();

			//  projection matrix
			auto projection = camera.GetProjectionMatrix();
			shaderProgram.SetUniform("projection", projection.ValuePtr());

			// camera/view transformation
			auto view = camera.GetViewMatrix();
			shaderProgram.SetUniform("view", view.ValuePtr());

			// render boxes
			glBindVertexArray(vao);
			for (int i = 0; i < 10; i++)
			{
				//  model matrix
				auto model = mat4::Translate(cubePositions[i]);
				float angle = 20.0f * i;
				model = model * mat4::Rotate(Radians(angle), vec3(1.0f, 0.3f, 0.5f));
				shaderProgram.SetUniform("model", model.ValuePtr());

				glDrawArrays(GL_TRIANGLES, 0, count);
			}

			window.OnUpdate();
		}

		glDeleteVertexArrays(1, &vao);
	}
}

#endif