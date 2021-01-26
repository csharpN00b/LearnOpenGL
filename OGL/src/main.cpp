#include <GLFW/glfw3.h>

#include "FMT.h"

namespace Logl
{

	GLFWwindow* initOpenGL();				// initialize OpenGL and create window
	void RenderScene0(GLFWwindow* window);	// without shader class
	void RenderScene1(GLFWwindow* window);	// 2D Geometry
	void RenderScene2(GLFWwindow* window);	// without camera class
	void RenderScene(GLFWwindow* window);	// dev

	void TestVector3();
	void TestMatrix4f();
}

#define TEST 0
void TestGLM();


int main()
{
#if TEST
	//Logl::TestVector3();
	Logl::TestMatrix4f();
	TestGLM();
#else

	GLFWwindow* window = Logl::initOpenGL();
	if (window == nullptr)
		return -1;

	Logl::RenderScene(window);

	glfwTerminate();
#endif
	return 0;
}

