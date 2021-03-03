#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Base.h"

GLFWwindow* CreateWindow(int width = 800, int height = 600);

unsigned int CreateShader(const char* vs, const char* fs);

unsigned int LoadTexture(const char* filepath, int format);


#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);