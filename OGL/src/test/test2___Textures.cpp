#include "test_base.h"

#include "Renderer/Shader.h"
#include "Math/Matrix4f.h"

using namespace Logl;

namespace E2_
{

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

	void RenderScene()
	{
		GLFWwindow* window = CreateWindow();

		// Shaders
		Shader shaderProgram("asserts/shaders/tex_vs.glsl", "asserts/shaders/tex_fs.glsl");
		if (!shaderProgram.IsValid())
		{
			__debugbreak();
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

		auto m1 = mat4::Translate(vec3(0.5f, -0.5f, 0.0f));
		auto m2 = mat4::Translate(vec3(-0.5f, 0.5f, 0.0f));

		// mvp transform
		auto model = mat4::Rotate(Radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
		auto view = mat4::Translate(vec3(0.0f, 0.0f, -3.0f));
		auto projection = mat4::Perspective(Radians(45.0f), 800.0f / 600.0f, 0.3f, 100.0f);
		//auto projection = mat4::Ortho(0.0f, 2.0f, 0.0f, 1.5f, 0.3f, 100.0f);
		//auto projection = mat4::Ortho(-1.0f, 1.0f, -0.75f, 0.75f, 0.3f, 100.0f);
		auto mvp = projection * view * model;

		//vec3 v(-0.5f, -0.5f, 0.0f);
		//auto v1 = mvpTrans * v;

		// Loop
		while (!glfwWindowShouldClose(window))
		{
			processWindowInput(window);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			auto time = glfwGetTime();
			auto transform1 = m1 * mat4::Rotate((float)time, vec3(0.0f, 0.0f, 1.0f));
			auto transform2 = m2 * mat4::Scale((float)sin(time));

			shaderProgram.Use();
			glBindVertexArray(VAO);

			shaderProgram.SetUniform("transform", transform1.ValuePtr());
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

			shaderProgram.SetUniform("transform", transform2.ValuePtr());
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

			shaderProgram.SetUniform("transform", mvp.ValuePtr());
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

}