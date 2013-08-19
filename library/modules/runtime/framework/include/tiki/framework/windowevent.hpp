#pragma once
#ifndef TIKI_WINDOWEVENT_HPP
#define TIKI_WINDOWEVENT_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/keys.hpp"
#include "tiki/base/mouse.hpp"
#include "tiki/base/structs.hpp"

namespace tiki
{
	enum WindowEventTypes
	{
		WET_Create,
		WET_Destroy,
		WET_KeyDown,
		WET_KeyUp,
		WET_MouseDown,
		WET_MouseUp,
		WET_MouseMove,
		WET_SizeChanged
	};

	struct WindowKeyEvent
	{
		Keys			key;		
	};

	struct WindowMouseButtonEvent
	{
		MouseButtons	button;
	};

	struct WindowMouseMoveEvent
	{
		uint2			position;
	};

	struct WindowSizeChangedEvent
	{
		uint2			size;
	};

	struct WindowEvent
	{
		WindowEventTypes			type;
		union
		{
			WindowKeyEvent			keyEvent;
			WindowMouseButtonEvent	mouseButtonEvent;
			WindowMouseMoveEvent	mouseMoveEvent;
			WindowSizeChangedEvent	sizeChangedEvent;
		}							data;
	};
}

#endif // TIKI_WINDOWEVENT_HPP
