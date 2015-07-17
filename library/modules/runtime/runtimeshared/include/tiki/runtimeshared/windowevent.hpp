#pragma once
#ifndef TIKI_WINDOWEVENT_HPP
#define TIKI_WINDOWEVENT_HPP

#include "tiki/base/structs.hpp"
#include "tiki/base/types.hpp"
#include "tiki/input/keyboardkey.hpp"
#include "tiki/input/mousebutton.hpp"

namespace tiki
{
	enum WindowEventType
	{
		WindowEventType_Create,
		WindowEventType_Destroy,
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

#endif // TIKI_WINDOWEVENT_HPP
