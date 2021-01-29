#pragma once

#include <string>

namespace Logl
{
	enum class EventType
	{
		None = -1,
		WindowClose = 0, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, 
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	};


	class Event
	{
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }

	public:
		bool Handled = false;
	};


#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
	virtual EventType GetEventType() const override { return GetStaticType(); }\
	virtual const char* GetName() const override { return #type; }


	class EventDispatcher
	{	
	public:
		template<typename T>
		using EventFunc = std::function<bool(T&)>;

		EventDispatcher(Event& event)
			:m_Event(event)
		{}

		template<typename T>
		bool Dispatch(const EventFunc<T>& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)(&m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

}