#pragma once

#include "Event.h"

namespace Logl
{
	class KeyEvent : public Event
	{
	public:
		KeyEvent(int key)
			: m_key(key)
		{}

		int GetKey() const { return m_key; }

	private:
		int m_key;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int key)
			: KeyEvent(key)
		{}

		EVENT_CLASS_TYPE(KeyPressed)
	};


	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int key)
			: KeyEvent(key)
		{}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}