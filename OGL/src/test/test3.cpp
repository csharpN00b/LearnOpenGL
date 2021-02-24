#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <unordered_map>

#include "Core/Base.h"

#include "Math/Matrix4f.h"

#include "Renderer/Shader.h"
#include "Renderer/PerspectiveCamera.h"
#include "Renderer/OrthographicCamera.h"

#include "GLFuncs.h"

#define USE_PERSPECTIVE_CAMERA 1

using namespace Logl;

namespace E3
{
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mousebutton_callback(GLFWwindow* window, int button, int action, int mods);

	void processWindowInput(GLFWwindow* window);

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


#if USE_PERSPECTIVE_CAMERA
	PerspectiveCamera camera(Frustum(ratio, 45.0f, 0.1f, 100.0f), vec3(0.0f, 0.0f, 3.0f));
#else
	OrthographicCamera camera(Frustum(-w/2.0f, w/2.0f, -h/2.0f, h/2.0f, 0.1f, 100.0f), vec3(0.0f, 0.0f, 3.0f));
#endif


	void SetOpenGL(GLFWwindow* window)
	{
		glEnable(GL_DEPTH_TEST);

		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetKeyCallback(window, key_callback);
		glfwSetMouseButtonCallback(window, mousebutton_callback);
	}

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

	void RenderScene(GLFWwindow* window)
	{
		SetOpenGL(window);

#if USE_PERSPECTIVE_CAMERA
#else
		camera.SetViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
#endif

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
			float currentFrame = (float)glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// input
			processWindowInput(window);

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

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &vao);
	}


	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		float xoffset{}, yoffset{};

		if (firstMouse)
		{
			firstMouse = false;
		}
		else
		{
			xoffset = (float)xpos - lastX;
			yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top
		}

#if USE_PERSPECTIVE_CAMERA
		camera.Turn(xoffset, yoffset);
#else
		camera.Turn(xoffset, yoffset);

		float height = static_cast<float>(SCR_HEIGHT);
		camera.Move(lastX, height - lastY, xpos, height - ypos);
#endif

		lastX = (float)xpos;
		lastY = (float)ypos;
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
#if USE_PERSPECTIVE_CAMERA
		camera.Scale((float)yoffset);
#else
		float height = static_cast<float>(SCR_HEIGHT);
		camera.Scale((float)yoffset, lastX, height - lastY);
#endif
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{

	}

	void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
	{
#if USE_PERSPECTIVE_CAMERA
#else
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			if (action == GLFW_PRESS)
				camera.SetRotate(true);
			else if (action == GLFW_RELEASE)
				camera.SetRotate(false);
		}
		
		if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		{
			if (action == GLFW_PRESS)
				camera.SetMove(true);
			else if (action == GLFW_RELEASE)
				camera.SetMove(false);
		}
#endif
	}

	void processWindowInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, 1);
		}

#if USE_PERSPECTIVE_CAMERA

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.Move(MoveDirection::FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.Move(MoveDirection::BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.Move(MoveDirection::LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.Move(MoveDirection::RIGHT, deltaTime);
#else
		
#endif
	}
}