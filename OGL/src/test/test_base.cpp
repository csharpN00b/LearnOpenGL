#include "test_base.h"

#include <malloc.h>

#include <stb/stb_image.h>


GLFWwindow* CreateWindow(int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "GLFWwindow", nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		__debugbreak();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {glViewport(0, 0, w, h); });

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		__debugbreak();
		return nullptr;
	}

	return window;
}

unsigned int CreateShader(const char* vs, const char* fs)
{
	auto compile = [](unsigned int type, const char* sourceCode)
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
		}

		return shader;
	};

	auto vertexShader = compile(GL_VERTEX_SHADER, vs);
	auto fragmentShader = compile(GL_FRAGMENT_SHADER, fs);

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
	}

	return shaderProgram;
}

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


void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		PRINT("[OpenGL Error] ({0}): {1} {2}: {3}\n", function, file, line);
		return false;
	}
	return true;
}