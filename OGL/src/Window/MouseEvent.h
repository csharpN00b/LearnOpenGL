#pragma once

#include "Event.h"

namespace Logl
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float xpos, float ypos)
			: m_xpos(xpos), m_ypos(ypos)
		{}

		EVENT_CLASS_TYPE(MouseMoved)

	public:
		float m_xpos;
		float m_ypos;
	};


	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xoffset, float yoffset)
			: m_xoffset(xoffset), m_yoffset(yoffset)
		{}

		EVENT_CLASS_TYPE(MouseScrolled)

	public:
		float m_xoffset;
		float m_yoffset;
	};
}