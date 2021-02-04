#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "malloc.h"

namespace E1
{
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
			__debugbreak();
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
			__debugbreak();
			return -1;
		}

		return shaderProgram;
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

	void processWindowInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, 1);
		}
	}

	void RenderScene(GLFWwindow* window)
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
			glClear(GL_COLOR_BUFFER_BIT);

			//float timeValue = glfwGetTime();
			//float offset = sin(timeValue);

			float time = (float)glfwGetTime();
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
}
