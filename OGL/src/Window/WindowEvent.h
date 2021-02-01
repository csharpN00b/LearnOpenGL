#pragma once

#include "Event.h"

namespace Logl
{

	class WindowCloseEvent : public Event
	{
	public:

		EVENT_CLASS_TYPE(WindowClose)
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height)
			: m_width(width), m_height(height)
		{}

		EVENT_CLASS_TYPE(WindowResize)

	private:
		int m_width;
		int m_height;
	};
}