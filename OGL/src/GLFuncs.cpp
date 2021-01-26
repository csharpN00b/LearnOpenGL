#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <unordered_map>

#include "FMT.h"

#include "Math/Matrix4f.h"

#include "Renderer/Shader.h"
#include "Renderer/PerspectiveCamera.h"


namespace Logl
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void processWindowInput(GLFWwindow* window);


	unsigned int LoadTexture(const char* filepath, int format);
	int SquareData(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO);
	int CubeData(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO);


	// settings
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	// camera
	float ratio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
	PerspectiveCamera camera(ViewParam(ratio), vec3(0.0f, 0.0f, 3.0f));

	GLFWwindow* initOpenGL()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GLFWwindow", nullptr, nullptr);
		if (window == nullptr)
		{
			PRINT("Failed to create GLFW window!\n");
			glfwTerminate();
			return nullptr;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			PRINT("Failed to initialize GLAD!\n");
			return nullptr;
		}

		//int nrAttributes;
		//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
		//PRINT("Maximum nr of vertex attributes supported: {}\n", nrAttributes);

		glEnable(GL_DEPTH_TEST);

		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		return window;
	}

	void RenderScene(GLFWwindow* window)
	{
		// Shaders
		Shader shaderProgram("asserts/shaders/mvp_vs.glsl", "asserts/shaders/mvp_fs.glsl");
		if (!shaderProgram.IsValid())
		{
			PRINT("Invalid shader program!\n");
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

#pragma region without camera class

	// camera
	vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
	vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
	vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);

	bool firstMouse = true;
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
	float fov = 45.0f;

#define RS2 0

	void RenderScene2(GLFWwindow* window)
	{
		// Shaders
		Shader shaderProgram("asserts/shaders/mvp_vs.glsl", "asserts/shaders/mvp_fs.glsl");
		if (!shaderProgram.IsValid())
		{
			PRINT("Invalid shader program!\n");
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

			// input
			processWindowInput(window);

			// render
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shaderProgram.Use();

			//  projection matrix
			auto projection = mat4::Perspective(Radians(fov), ratio, 0.1f, 100.0f);
			shaderProgram.SetUniform("projection", projection.ValuePtr());

			// camera/view transformation
			auto view = mat4::LookAt(cameraPos, cameraPos + cameraFront, cameraUp);
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

#pragma endregion

#pragma region render 2D objects

	void RenderScene1(GLFWwindow* window)
	{
		// Shaders
		Shader shaderProgram("asserts/shaders/tex_vs.glsl", "asserts/shaders/tex_fs.glsl");
		if (!shaderProgram.IsValid())
		{
			PRINT("Invalid shader program!\n");
			return;
		}

		// Vertex
		unsigned int VAO{}, VBO{}, EBO{};
		int count = SquareData(VAO, VBO, EBO);

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

		auto translate1 = mat4::Translate(vec3(0.5f, -0.5f, 0.0f));
		auto translate2 = mat4::Translate(vec3(-0.5f, 0.5f, 0.0f));
		vec3 rotateAxis(0.0f, 0.0f, 1.0f);

		// mvp transform
		auto modelTrans = mat4::Rotate(Radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
		auto viewTrans = mat4::Translate(vec3(0.0f, 0.0f, -3.0f));
		auto projectTrans = mat4::Perspective(Radians(45.0f), ratio, 0.3f, 100.0f);
		auto mvpTrans = projectTrans * viewTrans * modelTrans;

		// Loop
		while (!glfwWindowShouldClose(window))
		{
			processWindowInput(window);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			auto time = glfwGetTime();
			auto transform1 = translate1 * mat4::Rotate((float)time, rotateAxis);
			auto transform2 = translate2 * mat4::Scale((float)sin(time));

			//auto glmTransform = glm::translate(glm::mat4(1.f), glm::vec3(0.5f, -0.5f, 0.0f));
			//glmTransform = glm::rotate(glmTransform, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

			shaderProgram.Use();
			glBindVertexArray(VAO);

			shaderProgram.SetUniform("transform", transform1.ValuePtr());
			//shaderProgram.SetUniform("transform", glm::value_ptr(glmTransform));
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

			shaderProgram.SetUniform("transform", transform2.ValuePtr());
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

			shaderProgram.SetUniform("transform", mvpTrans.ValuePtr());
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

#pragma endregion

#pragma region without shader class

	static std::unordered_map<GLenum, std::string> ShaderName =
	{
		{GL_VERTEX_SHADER, "VERTEX"},
		{GL_FRAGMENT_SHADER, "FRAGMENT"},
		{GL_GEOMETRY_SHADER, "GEOMETRY"},
	};

	unsigned int CompileShader(unsigned int type, const char* sourceCode)
	{
		unsigned int shader = glCreateShader(type);
		glShaderSource(shader, 1, &sourceCode, nullptr);
		glCompileShader(shader);

		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			int length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			char* infoLog = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			PRINT("Faild to compile {0} shader!\n{1}\n", ShaderName[type], infoLog);
			glDeleteShader(shader);
			return -1;
		}

		return shader;
	}

	unsigned int CreateShaderProgram()
	{
		const char* vs = R"(
		#version 330 core

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aColor;
		out vec3 color;
		uniform float offset;

		void main()
		{
			gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);
			color = aColor;
		})";

		const char* fs = R"(
		#version 330 core

		in vec3 color;
		out vec4 FragColor;

		void main()
		{
			FragColor = vec4(color, 1.0f);
		})";

		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vs);
		//assert(vertexShader > 0);

		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fs);
		//assert(fragmentShader > 0);

		unsigned int shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		int success{};
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			int length;
			glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
			char* infoLog = (char*)alloca(length * sizeof(char));
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			PRINT("Failed to link shader program!\n{1}\n", infoLog);
			return -1;
		}

		return shaderProgram;
	}

	void RenderScene0(GLFWwindow* window)
	{
		unsigned int shaderProgram = CreateShaderProgram();
		if (shaderProgram <= 0)
			return;

		int offsetLocation = glGetUniformLocation(shaderProgram, "offset");

		unsigned int VAO{}, VBO{}, EBO{};
		int count = SquareData(VAO, VBO, EBO);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		float speed = 0.5f;
		float offset{};
		float lastTime{};

		while (!glfwWindowShouldClose(window))
		{
			processWindowInput(window);

			glClearColor(0.0, 1.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//float timeValue = glfwGetTime();
			//float offset = sin(timeValue);

			float time = glfwGetTime();
			if (lastTime > 0)
			{
				float timespan = time - lastTime;
				offset += speed * timespan;

				if (offset > 0.5f || offset < -0.5f)
				{
					speed = -speed;
					offset = offset > 0 ? 0.5f : -0.5f;
				}
			}
			lastTime = time;

			glUseProgram(shaderProgram);
			glUniform1f(offsetLocation, offset);

			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteProgram(shaderProgram);
	}

#pragma endregion

	unsigned int LoadTexture(const char* filepath, int format)
	{
		int width{}, height{}, nrChannels{};
		stbi_set_flip_vertically_on_load(1);
		unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);
		if (data)
		{
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
			return texture;
		}

		return 0;
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

	int SquareData(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO)
	{
		float vertices[] =
		{
			// positions         // colors          // texture coords
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f
		};

		unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0,
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return sizeof(indices) / sizeof(indices[0]);

	}


	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
		lastX = xpos;
		lastY = ypos;

#if RS2
		float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		vec3 front;
		front.x = cos(Radians(yaw)) * cos(Radians(pitch));
		front.y = sin(Radians(pitch));
		front.z = sin(Radians(yaw)) * cos(Radians(pitch));
		cameraFront = front.normalize();
#else
		camera.OnMouseMove(xoffset, yoffset);
#endif
	}

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
#if RS2
		if (fov >= 1.0f && fov <= 45.0f)
			fov -= yoffset;
		if (fov <= 1.0f)
			fov = 1.0f;
		if (fov >= 45.0f)
			fov = 45.0f;
#else
		camera.OnMouseScroll(yoffset);
#endif
	}
	
	void processWindowInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, 1);
		}

#if RS2
		float cameraSpeed = 2.5f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPos -= CrossProduct(cameraFront, cameraUp).normalize() * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPos += CrossProduct(cameraFront, cameraUp).normalize() * cameraSpeed;
#else
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.OnKeyPress(Camera_Movement::FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.OnKeyPress(Camera_Movement::BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.OnKeyPress(Camera_Movement::LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.OnKeyPress(Camera_Movement::RIGHT, deltaTime);
#endif
	}

}