#pragma once
#ifndef __TIKI_INPUTDEVICE_HPP_INCLUDED__
#define __TIKI_INPUTDEVICE_HPP_INCLUDED__

#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	enum InputDeviceType
	{
		InputDeviceType_Keyboard,
		InputDeviceType_Mouse,
		InputDeviceType_Controller,

		InputDeviceType_Count
	};

	struct InputDevice
	{
		InputDeviceType		deviceType;
		uint				deviceId;		
	};

	TIKI_FORCE_INLINE bool operator==( const InputDevice& lhs, const InputDevice& rhs )
	{
		return lhs.deviceType == rhs.deviceType && lhs.deviceId == rhs.deviceId;
	}
}

#endif // __TIKI_INPUTDEVICE_HPP_INCLUDED__
