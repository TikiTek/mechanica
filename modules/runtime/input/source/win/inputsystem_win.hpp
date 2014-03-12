#pragma once
#ifndef __TIKI_INPUTSYSTEM_WIN32_HPP_INCLUDED__
#define __TIKI_INPUTSYSTEM_WIN32_HPP_INCLUDED__

#include "tiki/base/types.hpp"

struct IDirectInputA;
struct IDirectInputDeviceA;

namespace tiki
{
	struct InputSystemState;

	struct InputSystemPlatformData
	{
		InputSystemPlatformData()
		{
			pInputDevice	= nullptr;
			pMouse			= nullptr;
			pKeyboard		= nullptr;

			currentStateIndex	= 0u;
			pStates[ 0u ]		= nullptr;
			pStates[ 1u ]		= nullptr;
		}

		IDirectInputA*			pInputDevice;
		IDirectInputDeviceA*	pMouse;
		IDirectInputDeviceA*	pKeyboard;

		uint8					keyboardMapping[ 256u ];

		uint					currentStateIndex;
		InputSystemState*		pStates[ 2u ];
	};
}

#endif // __TIKI_INPUTSYSTEM_WIN32_HPP_INCLUDED__
