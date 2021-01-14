#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>

#include "Shader.h"

GLFWwindow* initOpenGL(); // initialize OpenGL and create window

void RenderScene(GLFWwindow* window);
unsigned int createShader();
int inputVertexData(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO);

void processWindowInput(GLFWwindow*);
void framebuffer_size_callback(GLFWwindow*, int, int);

#define USE_SHADER_CLASS 1

int main()
{
	GLFWwindow* window = initOpenGL();
	if (window == nullptr)
		return -1;

	/*int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;*/

	RenderScene(window);

	glfwTerminate();
	
	return 0;
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
		std::cout << "failed to create GLFW Window!" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "failed to initialize GLAD!" << std::endl;
		return nullptr;
	}

	return window;
}

#if USE_SHADER_CLASS
void RenderScene(GLFWwindow* window)
{
	Shader shaderProgram("Assert/vs.glsl", "Assert/fs.glsl");
	if (!shaderProgram.IsValid())
	{
		std::cout << "Invalid Shader Program!" << std::endl;
		return;
	}


	unsigned int VAO{}, VBO{}, EBO{};
	int count = inputVertexData(VAO, VBO, EBO);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		processWindowInput(window);

		glClearColor(0.0, 1.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Use();
		float timeValue = glfwGetTime();
		float offset = sin(timeValue);
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
#else
void RenderScene(GLFWwindow* window)
{
	unsigned int shaderProgram = createShader();
	int offsetLocation = glGetUniformLocation(shaderProgram, "offset");

	unsigned int VAO{}, VBO{}, EBO{};
	int count = inputVertexData(VAO, VBO, EBO);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		processWindowInput(window);

		glClearColor(0.0, 1.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		float timeValue = glfwGetTime();
		float offset = sin(timeValue);
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
#endif

void checkShaderCompileError(unsigned int shaderId, GLenum type)
{
	static std::map<GLenum, std::string> shaderName =
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
		std::cout << "ERROR::SHADER::" << shaderName[type] << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

unsigned int createShader()
{
	unsigned int vertexShader{}, fragmentShader{}, shaderProgram{};

	const char* vs = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform float offset;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);\n"
		"}\n\0";
	const char* fs = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform float offset;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(offset, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vs, nullptr);
	glCompileShader(vertexShader);
	checkShaderCompileError(vertexShader, GL_VERTEX_SHADER);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fs, nullptr);
	glCompileShader(fragmentShader);
	checkShaderCompileError(fragmentShader, GL_FRAGMENT_SHADER);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int inputVertexData(unsigned int& VAO, unsigned int& VBO, unsigned int& EBO)
{
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}