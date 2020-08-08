#pragma once

#include "tiki/base/types.hpp"
#include "tiki/base/platform.hpp"

#include <windows.h>

struct IDirectInputA;
struct IDirectInputDeviceA;

namespace tiki
{
	class InputSystem;
	struct InputSystemState;

	enum
	{
		MaxInputTouchPoints			= 16u,
		InvalidTouchInputMapping	= 0xffffffffu
	};

	struct InputSystemPlatformData
	{
		InputSystemPlatformData()
		{
			windowHandle	= nullptr;

			pInputDevice	= nullptr;
			pMouse			= nullptr;
			pKeyboard		= nullptr;

			currentStateIndex	= 0u;
			pStates[ 0u ]		= nullptr;
			pStates[ 1u ]		= nullptr;
		}

		WindowHandle			windowHandle;

		IDirectInputA*			pInputDevice;
		IDirectInputDeviceA*	pMouse;
		IDirectInputDeviceA*	pKeyboard;
		bool					isTouchInputReady;

		uint8					keyboardMapping[ 256u ];
		uint32					touchInputMapping[ MaxInputTouchPoints ];

		uint					currentStateIndex;
		InputSystemState*		pStates[ 2u ];
	};

	class InputSystemPlatform
	{
	public:

		static bool handleWindowMessage( InputSystem& inputSystem, UINT message, WPARAM wParam, LPARAM lParam );

	};
}
