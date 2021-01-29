#pragma once

#include "Event.h"

namespace Logl
{

	class WindowCloseEvent : public Event
	{
	public:

		EVENT_CLASS_TYPE(WindowClose)
	};
}