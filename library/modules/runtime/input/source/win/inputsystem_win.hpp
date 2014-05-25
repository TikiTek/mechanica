#pragma once
#ifndef __TIKI_INPUTSYSTEM_WIN32_HPP_INCLUDED__
#define __TIKI_INPUTSYSTEM_WIN32_HPP_INCLUDED__

#include "tiki/base/types.hpp"

struct IDirectInputA;
struct IDirectInputDeviceA;

namespace tiki
{
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
}

#endif // __TIKI_INPUTSYSTEM_WIN32_HPP_INCLUDED__
