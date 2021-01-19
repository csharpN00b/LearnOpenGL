#include <GLFW/glfw3.h>

namespace Logl
{

	GLFWwindow* initOpenGL(); // initialize OpenGL and create window
	void RenderScene0(GLFWwindow* window); // not using shader class
	void RenderScene(GLFWwindow* window); // using shader class

	void TestVector3();
}

#define TEST 0


int main()
{
#if TEST
	Logl::TestVector3();
#else

	GLFWwindow* window = Logl::initOpenGL();
	if (window == nullptr)
		return -1;

	Logl::RenderScene(window);

	glfwTerminate();
#endif
	return 0;
}

