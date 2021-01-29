#include "../FMT.h"

#include <glad/glad.h>

#include "Window.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace Logl
{
	static bool s_GLFWInitialized = false;

	Window::Window(unsigned int width, unsigned int height, const char* title)
	{
		m_Data.Width = width;
		m_Data.Height = height;
		m_Data.Title = title;

		Init();
	}

	Window::~Window()
	{
		Shutdown();
	}

	void Window::Init()
	{
		if (!s_GLFWInitialized)
		{
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			s_GLFWInitialized = true;
		}
		
		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		if (m_Window == nullptr)
		{
			PRINT("Failed to create GLFW window!\n");
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(m_Window);


		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			PRINT("Failed to initialize GLAD!\n");
			return;
		}

		glfwSetWindowUserPointer(m_Window, &m_Data);

		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				glViewport(0, 0, width, height);
			});



		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}
					//case GLFW_REPEAT:
					//{
					//	KeyPressedEvent event(key);
					//	data.EventCallback(event);
					//	break;
					//}
				}
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)xpos, (float)ypos);
				data.EventCallback(event);
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event(xoffset, yoffset);
				data.EventCallback(event);
			});


	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}