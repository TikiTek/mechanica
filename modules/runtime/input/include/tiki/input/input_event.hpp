#pragma once

#include "tiki/base/types.hpp"
#include "tiki/input/controller_button.hpp"
#include "tiki/input/input_device.hpp"
#include "tiki/input/keyboard_key.hpp"
#include "tiki/input/mouse_button.hpp"

namespace tiki
{
	enum
	{
		InputMouseWheelDelta = 120
	};

	enum InputEventType
	{
		InputEventType_Device_Connected,
		InputEventType_Device_Disconnected,

		InputEventType_Keyboard_Down,
		InputEventType_Keyboard_Up,
		InputEventType_Keyboard_Character,

		InputEventType_Mouse_Moved,
		InputEventType_Mouse_ButtonDown,
		InputEventType_Mouse_ButtonUp,
		InputEventType_Mouse_DoubleClick,		// not sent in game
		InputEventType_Mouse_Wheel,

		InputEventType_Controller_ButtonDown,
		InputEventType_Controller_ButtonUp,
		InputEventType_Controller_StickChanged,
		InputEventType_Controller_TriggerChanged,

		InputEventType_Touch_PointDown,
		InputEventType_Touch_PointUp,
		InputEventType_Touch_PointMove,

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

	struct InputEventKeyboardCharacterData
	{
		uint32		rune;
	};

	struct InputEventMouseMovedData
	{
		sint16 xState;
		sint16 yState;
		sint16 xOffset;
		sint16 yOffset;
	};

	struct InputEventMouseButtonData
	{
		MouseButton button;
	};

	struct InputEventMouseWheelData
	{
		sint16 offset;
	};

	struct InputEventControllerButtonData
	{
		ControllerButton button;
	};

	struct InputEventControllerStickData
	{
		uint32	stickIndex;
		float	xState;
		float	yState;
	};

	struct InputEventControllerTriggerData
	{
		uint32	triggerIndex;
		float	state;
	};

	struct InputEventTouchData
	{
		uint16 pointIndex;
		uint16 xState;
		uint16 yState;
	};

	union InputEventData
	{
		InputEventDeviceData				device;

		InputEventKeyboardKeyData			keybaordKey;
		InputEventKeyboardCharacterData		keyboardCharacter;

		InputEventMouseMovedData			mouseMoved;
		InputEventMouseButtonData			mouseButton;
		InputEventMouseWheelData			mouseWheel;

		InputEventControllerButtonData		controllerButton;;
		InputEventControllerStickData		controllerStick;
		InputEventControllerTriggerData		controllerTrigger;

		InputEventTouchData					touch;
	};

	struct InputEvent
	{
		InputEventType	eventType;
		InputDeviceType	deviceType;
		uint			deviceId;

		InputEventData	data;
	};
}
