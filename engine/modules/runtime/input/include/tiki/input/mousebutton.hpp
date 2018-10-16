#pragma once
#ifndef __TIKI_MOUSEBUTTON_HPP_INCLUDED__
#define __TIKI_MOUSEBUTTON_HPP_INCLUDED__

namespace tiki
{
	enum MouseButton
	{
		MouseButton_Invalid = -1,

		MouseButton_Left,
		MouseButton_Right,
		MouseButton_Middle,

		MouseButton_Count
	};

	struct MouseButtonState
	{
		MouseButtonState()
		{
			for( uint i = 0u; i < TIKI_COUNT( state ); ++i )
			{
				state[ i ] = false;
			}
		}

		bool state[ MouseButton_Count ];
	};
}

#endif // __TIKI_MOUSEBUTTON_HPP_INCLUDED__
