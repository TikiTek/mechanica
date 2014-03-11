#pragma once
#ifndef __TIKI_INPUTEVENT_HPP_INCLUDED__
#define __TIKI_INPUTEVENT_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/input/controllerbutton.hpp"
#include "tiki/input/inputdevice.hpp"
#include "tiki/input/keyboardkey.hpp"
#include "tiki/input/mousebutton.hpp"

namespace tiki
{
	enum InputEventType
	{
		InputEventType_Device_Connected,
		InputEventType_Device_Disconnected,

		InputEventType_Keyboard_Down,
		InputEventType_Keyboard_Up,

		InputEventType_Mouse_Moved,
		InputEventType_Mouse_ButtonDown,
		InputEventType_Mouse_ButtonUp,
		InputEventType_Mouse_Wheel,

		InputEventType_Controller_ButtonDown,
		InputEventType_Controller_ButtonUp,
		InputEventType_Controller_StickChanged,
		InputEventType_Controller_TriggerChanged,

		InputEventType_Count
	};

	struct InputEventDeviceData
	{
		InputDevice device;
	};

	struct InputEventKeyboardKeyData
	{
		KeyboardKey key;
	};

	struct InputEventMouseMovedData
	{
		uint xOffset;
		uint yOffset;
	};

	struct InputEventMouseButtonData
	{
		MouseButton button;
	};

	struct InputEventMouseWheelData
	{
		uint offset;
	};

	struct InputEventControllerButtonData
	{
		ControllerButton button;
	};

	struct InputEventControllerStickData
	{
		float xOffset;
		float yOffset;
	};

	struct InputEventControllerTriggerData
	{
		float offset;
	};

	union InputEventData
	{
		InputEventDeviceData				device;

		InputEventKeyboardKeyData			keybaordKey;

		InputEventMouseMovedData			mouseMoved;
		InputEventMouseButtonData			mouseButton;
		InputEventMouseWheelData			mouseWheel;

		InputEventControllerButtonData		controllerButton;;
		InputEventControllerStickData		controllerStick;
		InputEventControllerTriggerData		controllerTrigger;
	};

	struct InputEvent
	{
		InputEventType	eventType;
		InputDeviceType	deviceType;
		uint			deviceId;

		InputEventData	data;
	};
}

#endif // __TIKI_INPUTEVENT_HPP_INCLUDED__
