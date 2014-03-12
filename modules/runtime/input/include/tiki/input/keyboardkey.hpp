#pragma once
#ifndef TIKI_KEYBOARDKEY_HPP
#define TIKI_KEYBOARDKEY_HPP

namespace tiki
{
	enum KeyboardKey
	{
		KeyboardKey_Escape,
		KeyboardKey_BackSpace,
		KeyboardKey_Tab,
		KeyboardKey_Return,
		KeyboardKey_LeftControl,
		KeyboardKey_RightControl,
		KeyboardKey_LeftShift,
		KeyboardKey_RightShift,
		KeyboardKey_LeftAlt,
		KeyboardKey_RightAlt,
		KeyboardKey_LeftSystem,			// left windows key
		KeyboardKey_RightSystem,		// right windows key

		KeyboardKey_Insert,
		KeyboardKey_Delete,
		KeyboardKey_Home,
		KeyboardKey_End,
		KeyboardKey_PageUp,
		KeyboardKey_PageDown,

		KeyboardKey_Left,
		KeyboardKey_Up,
		KeyboardKey_Down,
		KeyboardKey_Right,

		KeyboardKey_Space,
		KeyboardKey_Minus,
		KeyboardKey_Plus,
		KeyboardKey_Slash,
		KeyboardKey_Star,
		KeyboardKey_Equals,
		KeyboardKey_Semicolon,
		KeyboardKey_Apostrophe,
		KeyboardKey_BackSlash,
		KeyboardKey_Comma,
		KeyboardKey_Dot,
		KeyboardKey_GraveAccent,

		KeyboardKey_Alpha1,
		KeyboardKey_Alpha2,
		KeyboardKey_Alpha3,
		KeyboardKey_Alpha4,
		KeyboardKey_Alpha5,
		KeyboardKey_Alpha6,
		KeyboardKey_Alpha7,
		KeyboardKey_Alpha8,
		KeyboardKey_Alpha9,
		KeyboardKey_Alpha0,

		KeyboardKey_A,
		KeyboardKey_B,
		KeyboardKey_C,
		KeyboardKey_D,
		KeyboardKey_E,
		KeyboardKey_F,
		KeyboardKey_G,
		KeyboardKey_H,
		KeyboardKey_I,
		KeyboardKey_J,
		KeyboardKey_K,
		KeyboardKey_L,
		KeyboardKey_M,
		KeyboardKey_N,
		KeyboardKey_O,
		KeyboardKey_P,
		KeyboardKey_Q,
		KeyboardKey_R,
		KeyboardKey_S,
		KeyboardKey_T,
		KeyboardKey_U,
		KeyboardKey_V,
		KeyboardKey_W,
		KeyboardKey_X,
		KeyboardKey_Y,
		KeyboardKey_Z,

		KeyboardKey_F1,
		KeyboardKey_F2,
		KeyboardKey_F3,
		KeyboardKey_F4,
		KeyboardKey_F5,
		KeyboardKey_F6,
		KeyboardKey_F7,
		KeyboardKey_F8,
		KeyboardKey_F9,
		KeyboardKey_F10,
		KeyboardKey_F11,
		KeyboardKey_F12,

		KeyboardKey_Numpad0,
		KeyboardKey_Numpad1,
		KeyboardKey_Numpad2,
		KeyboardKey_Numpad3,
		KeyboardKey_Numpad4,
		KeyboardKey_Numpad5,
		KeyboardKey_Numpad6,
		KeyboardKey_Numpad7,
		KeyboardKey_Numpad8,
		KeyboardKey_Numpad9,
		KeyboardKey_NumpadEquals,
		KeyboardKey_NumpadComma,
		KeyboardKey_NumpadEnter,

		KeyboardKey_CapsLock,
		KeyboardKey_Numlock,
		KeyboardKey_Scroll,

		KeyboardKey_Count
	};

	namespace input
	{
		const char*	getKeyboardKeyName( KeyboardKey key );
	}
}

#endif // TIKI_KEYBOARDKEY_HPP
