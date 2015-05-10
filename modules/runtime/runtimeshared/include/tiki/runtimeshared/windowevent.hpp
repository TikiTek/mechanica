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
		//WindowEventType_KeyDown,
		//WindowEventType_KeyUp,
		//WindowEventType_MouseDown,
		//WindowEventType_MouseUp,
		//WindowEventType_MouseMove,
		WindowEventType_SizeChanged,
		WindowEventType_Touch
	};

	//struct WindowKeyEventData
	//{
	//	KeyboardKey		key;		
	//};

	//struct WindowMouseButtonEventData
	//{
	//	MouseButton		button;
	//};

	//struct WindowMouseMoveEventData
	//{
	//	uint2			position;
	//};

	struct WindowSizeChangedEventData
	{
		uint2			size;
	};

	TIKI_DEFINE_HANLE( TouchInputHandle );

	struct WindowTouchEventData
	{
		uint				pointCount;
		TouchInputHandle	handle;
	};

	union WindowEventData
	{
		//WindowKeyEventData			key;
		//WindowMouseButtonEventData	mouseButton;
		//WindowMouseMoveEventData	mouseMove;
		WindowSizeChangedEventData	sizeChanged;
		WindowTouchEventData		touch;
	};

	struct WindowEvent
	{
		WindowEventType		type;
		WindowEventData		data;
	};
}

#endif // TIKI_WINDOWEVENT_HPP
