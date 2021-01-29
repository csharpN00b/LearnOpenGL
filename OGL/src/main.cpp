#include "GLFuncs.h"

void TestLogl();
void TestGLM();

#define DECLARE_RENDER_FUNC(ns) namespace ns { void RenderScene(GLFWwindow*); }
#define CALL_RENDER_FUNC(ns) ns::RenderScene(window)

DECLARE_RENDER_FUNC(E1)
DECLARE_RENDER_FUNC(E2)
DECLARE_RENDER_FUNC(E3)
DECLARE_RENDER_FUNC(E4)

int main()
{
	//TestLogl();
	//TestGLM();
	//return 0;

#if 0
	GLFWwindow* window = initOpenGL();
	if (window == nullptr)
		return -1;

	//CALL_RENDER_FUNC(E1);
	//CALL_RENDER_FUNC(E2);
	CALL_RENDER_FUNC(E3);

#else 
	GLFWwindow* window = nullptr;
	CALL_RENDER_FUNC(E4);


#endif

	glfwTerminate();

	return 0;
}

