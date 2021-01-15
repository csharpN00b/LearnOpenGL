#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

#include "Base.h"
#include "Shader.h"


int inputVertexData(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO);
void processWindowInput(GLFWwindow*);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow* initOpenGL()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "GLFWwindow", nullptr, nullptr);
	if (window == nullptr)
	{
		PRINT("failed to create GLFW Window!\n");
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		PRINT("failed to initialize GLAD!\n");
		return nullptr;
	}

	//int nrAttributes;
	//glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//PRINT("Maximum nr of vertex attributes supported: {}\n", nrAttributes);

	return window;
}

void RenderScene(GLFWwindow* window)
{
	Shader shaderProgram("Assert/vs.glsl", "Assert/fs.glsl");
	if (!shaderProgram.IsValid())
	{
		PRINT("Invalid Shader Program!\n");
		return;
	}

	unsigned int VAO{}, VBO{}, EBO{};
	int count = inputVertexData(VAO, VBO, EBO);

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
		//float offset =  sin(timeValue);

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

		shaderProgram.Use();
		shaderProgram.SetUniform("offset", offset);

		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, count);
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

int inputVertexData(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO)
{
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.8f, 0.2f,
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//return sizeof(vertices) / sizeof(vertices[0]) / 3; // call glDrawArrays
	return sizeof(indices) / sizeof(indices[0]);

}

void processWindowInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, 1);
	}
}


#pragma region without shader class

bool checkShaderCompileError(unsigned int shaderId, GLenum type)
{
	static std::unordered_map<GLenum, std::string> shaderName =
	{
		{GL_VERTEX_SHADER, "VertexShader"},
		{GL_FRAGMENT_SHADER, "FragmentShader"},
		{GL_GEOMETRY_SHADER, "GeometryShader"},
	};

	int success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		PRINT("ERROR::SHADER::{0}::COMPILATION_FAILED\n{1}\n", shaderName[type], infoLog);
		return false;
	}
	return true;
}

unsigned int createShader()
{
	unsigned int vertexShader{}, fragmentShader{}, shaderProgram{};

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
	}
)";
	const char* fs = R"(
	#version 330 core

	in vec3 color;
	out vec4 FragColor;
	uniform float offset;

	void main()
	{
		FragColor = vec4(color, 1.0f);
	}
)";
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vs, nullptr);
	glCompileShader(vertexShader);
	if (!checkShaderCompileError(vertexShader, GL_VERTEX_SHADER))
		return -1;

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fs, nullptr);
	glCompileShader(fragmentShader);
	if (!checkShaderCompileError(fragmentShader, GL_FRAGMENT_SHADER))
		return -1;

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	int success{};
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		PRINT("ERROR::SHADER::{0}::LINK_FAILED\n{1}\n", infoLog);
		return -1;
	}

	return shaderProgram;
}

void RenderScene0(GLFWwindow* window)
{
	unsigned int shaderProgram = createShader();
	if (shaderProgram <= 0)
		return;

	int offsetLocation = glGetUniformLocation(shaderProgram, "offset");

	unsigned int VAO{}, VBO{}, EBO{};
	int count = inputVertexData(VAO, VBO, EBO);

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
		//glDrawArrays(GL_TRIANGLES, 0, count);
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
