#include <GLFW/glfw3.h>

#include "FMT.h"

namespace Logl
{

	GLFWwindow* initOpenGL();				// initialize OpenGL and create window
	void RenderScene0(GLFWwindow* window);	// not using shader class
	void RenderScene(GLFWwindow* window);	// using shader class

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

