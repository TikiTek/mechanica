#pragma once
#ifndef TIKI_WINDOWEVENT_HPP
#define TIKI_WINDOWEVENT_HPP

#include "tiki/base/types.hpp"
#include "tiki/base/keys.hpp"
#include "tiki/base/mouse.hpp"
#include "tiki/base/structs.hpp"

namespace tiki
{
	enum WindowEventType
	{
		WindowEventType_Create,
		WindowEventType_Destroy,
		WindowEventType_KeyDown,
		WindowEventType_KeyUp,
		WindowEventType_MouseDown,
		WindowEventType_MouseUp,
		WindowEventType_MouseMove,
		WindowEventType_SizeChanged
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
		WindowEventType			type;
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
