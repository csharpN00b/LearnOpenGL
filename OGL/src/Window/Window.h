#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <functional>

#include "Event.h"

namespace Logl
{
	class Window
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		Window(unsigned int width, unsigned int height, const char* title);
		
		virtual ~Window();

		unsigned int GetWidth() const { return m_Data.Width; }
		unsigned int GetHeight() const { return m_Data.Height; }
		std::string GetTitle() const { return m_Data.Title; }

		void OnUpdate();
		void Shutdown();

		void SetCursorPos(double x, double y);

		void SetEventCallback(const EventCallbackFunc& callback) { m_Data.EventCallback = callback; }

		GLFWwindow* GetNativeWindow() const { return m_Window; }

	private:
		void Init();

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			unsigned int Width;
			unsigned int Height;
			std::string Title;
			EventCallbackFunc EventCallback;
		};

		WindowData m_Data;
	};
}