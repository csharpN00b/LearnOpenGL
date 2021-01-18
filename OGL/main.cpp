#include <GLFW/glfw3.h>

GLFWwindow* initOpenGL(); // initialize OpenGL and create window
void RenderScene0(GLFWwindow* window); // not using shader class
void RenderScene(GLFWwindow* window); // using shader class

#define TEST 0
void TestVector3();

int main()
{
#if TEST
	TestVector3();
#else

	GLFWwindow* window = initOpenGL();
	if (window == nullptr)
		return -1;

	RenderScene(window);

	glfwTerminate();
	
	return 0;

#endif
}

