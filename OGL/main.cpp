#include <glad/glad.h>
#include <GLFW/glfw3.h>


GLFWwindow* initOpenGL(); // initialize OpenGL and create window
void RenderScene(GLFWwindow* window);

int main()
{
	GLFWwindow* window = initOpenGL();
	if (window == nullptr)
		return -1;

	RenderScene(window);

	glfwTerminate();
	
	return 0;
}

