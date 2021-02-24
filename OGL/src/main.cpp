#include "Core/Base.h"
#include "GLFuncs.h"
#include "test/test.h"

int main()
{
	// test math
	//TestLogl();
	//TestGLM();

	// simple test
	//GLFWwindow* window = initOpenGL();
	//ASSERT(window);
	//CALL_RENDER_FUNC(E3);

	// use renderer
	CALL_RENDERER_FUNC(E9);

	glfwTerminate();
	return 0;
}

