#include "test_base.h"

namespace E2
{
	void processWindowInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, 1);
		}
	}

	void RenderScene()
	{
		GLFWwindow* window = CreateWindow();

		unsigned int vao, vbo, ibo;
		float vertices[] =
		{
			// positions         // colors        
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f
		};

		unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0,
		};

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ibo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

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
		out vec4 FragColor;

		in vec3 color;

		void main()
		{
			FragColor = vec4(color, 1.0f);
		})";

		unsigned int shader = CreateShader(vs, fs);
		int location = glGetUniformLocation(shader, "offset");

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		int indicesCount = sizeof(indices) / sizeof(indices[0]);
		float speed = 0.5f, direction = 1.0f;
		float offset = 0.0f;
		float currentTime{}, lastTime{};
		while (!glfwWindowShouldClose(window))
		{
			processWindowInput(window);

			glClearColor(0.0, 1.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT);

			currentTime = (float)glfwGetTime();
			if (lastTime > 0)
			{
				float timespan = currentTime - lastTime;
				offset += speed * timespan * direction;
				if (offset > 0.5f)
				{
					offset = 0.5f;
					direction = -direction;
				}
				else if (offset < -0.5f)
				{
					offset = -0.5f;
					direction = -direction;
				}
			}
			lastTime = currentTime;

			glUseProgram(shader);
			glUniform1f(location, offset);

			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, nullptr);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ibo);
		glDeleteProgram(shader);
	}
}
