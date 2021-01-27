#pragma once

#include "GLFW/glfw3.h"

extern const unsigned int SCR_WIDTH;
extern const unsigned int SCR_HEIGHT;

GLFWwindow* initOpenGL();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

unsigned int LoadTexture(const char* filepath, int format);