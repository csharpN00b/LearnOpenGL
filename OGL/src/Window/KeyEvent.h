#pragma once

#include "Event.h"

namespace Logl
{
	class KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(int key)
			: m_key(key)
		{}

		EVENT_CLASS_TYPE(KeyPressed)

	public:
		int m_key;
	};


	class KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent(int key)
			: m_key(key)
		{}

		EVENT_CLASS_TYPE(KeyReleased)

	public:
		int m_key;
	};
}