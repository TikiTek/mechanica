#pragma once

#include "tiki/base/types.hpp"
#include "tiki/input/keyboard_key.hpp"
#include "tiki/input/mouse_button.hpp"

#include "base.hpp"

namespace tiki
{
	enum WindowEventType
	{
		WindowEventType_Create,
		WindowEventType_Close,
		WindowEventType_SizeChanged
	};

	struct WindowSizeChangedEventData
	{
		uint2			size;
	};

	union WindowEventData
	{
		WindowSizeChangedEventData	sizeChanged;
	};

	struct WindowEvent
	{
		WindowEventType		type;
		WindowEventData		data;
	};
}
