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

		float GetXPos() const { return m_xpos; }
		float GetYPos() const { return m_ypos; }

		EVENT_CLASS_TYPE(MouseMoved)

	private:
		float m_xpos;
		float m_ypos;
	};


	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xoffset, float yoffset)
			: m_xoffset(xoffset), m_yoffset(yoffset)
		{}

		float GetXOffset() const { return m_xoffset; }
		float GetYOffset() const { return m_yoffset; }

		EVENT_CLASS_TYPE(MouseScrolled)

	private:
		float m_xoffset;
		float m_yoffset;
	};
}