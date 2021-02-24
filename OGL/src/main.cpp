#include "Core/Base.h"
#include "GLFuncs.h"
#include "test/test.h"

#define USE_RENEDERER 1

int main()
{
	// test math
	//TestLogl();
	//TestGLM();

#if USE_RENEDERER

	CALL_RENDERER_FUNC(E10);

#else

	GLFWwindow* window = initOpenGL();
	ASSERT(window);
	CALL_RENDER_FUNC(E3);

#endif

	glfwTerminate();
	return 0;
}

