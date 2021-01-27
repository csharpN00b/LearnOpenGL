#include "GLFuncs.h"

void TestLogl();
void TestGLM();

#define DECLARE_TEST_FUNC(ns) namespace ns { void RenderScene(GLFWwindow*); }
#define CALL_TEST_FUNC(ns) ns::RenderScene(window)

DECLARE_TEST_FUNC(E1)
DECLARE_TEST_FUNC(E2)
DECLARE_TEST_FUNC(E3)

int main()
{
	//TestLogl();
	//TestGLM();
	//return 0;

	GLFWwindow* window = initOpenGL();
	if (window == nullptr)
		return -1;

	//CALL_TEST_FUNC(E1);
	//CALL_TEST_FUNC(E2);
	CALL_TEST_FUNC(E3);

	glfwTerminate();

	return 0;
}

