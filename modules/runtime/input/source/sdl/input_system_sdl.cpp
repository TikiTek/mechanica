
#include "tiki/input/inputsystem.hpp"

#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/runtimeshared/windoweventbuffer.hpp"

#include "SDL_events.h"
#include "SDL_gamecontroller.h"
#include "SDL_keycode.h"

namespace tiki
{
	//SDL_Keysym
	struct TouchInputState
	{
		bool			isValid;
		bool			isNewPoint;
		uint			index;
	};

	struct InputSystemState
	{
		//DIMOUSESTATE	mouse;
		//uint8			aKeyboard[ 256u ];

		//bool			aControllerConnected[ XUSER_MAX_COUNT ];
		//XINPUT_STATE	aController[ XUSER_MAX_COUNT ];

		//TOUCHINPUT		aTouchPoints[ MaxInputTouchPoints ];
	};

	static const int s_aVirtualKeyCodeMapping[ KeyboardKey_Count ] =
	{		
		SDLK_ESCAPE,		// KeyboardKey_Escape
		SDLK_BACKSPACE,		// KeyboardKey_BackSpace
		SDLK_TAB,			// KeyboardKey_Tab
		SDLK_RETURN,		// KeyboardKey_Return
		SDLK_LCTRL,			// KeyboardKey_LeftControl
		SDLK_RCTRL,			// KeyboardKey_RightControl
		SDLK_LSHIFT,		// KeyboardKey_LeftShift
		SDLK_RSHIFT,		// KeyboardKey_RightShift
		SDLK_LALT,			// KeyboardKey_LeftAlt
		SDLK_RALT,			// KeyboardKey_RightAlt
		SDLK_LGUI,			// KeyboardKey_LeftSystem
		SDLK_RGUI,			// KeyboardKey_RightSystem

		SDLK_INSERT,		// KeyboardKey_Insert
		SDLK_DELETE,		// KeyboardKey_Delete
		SDLK_HOME,			// KeyboardKey_Home
		SDLK_END,			// KeyboardKey_End
		SDLK_PAGEUP,		// KeyboardKey_PageUp
		SDLK_PAGEDOWN,		// KeyboardKey_PageDown

		SDLK_LEFT,			// KeyboardKey_Left
		SDLK_UP,			// KeyboardKey_Up
		SDLK_DOWN,			// KeyboardKey_Down
		SDLK_RIGHT,			// KeyboardKey_Right

		SDLK_SPACE,			// KeyboardKey_Space
		SDLK_MINUS,			// KeyboardKey_Minus
		SDLK_PLUS,			// KeyboardKey_Plus
		SDLK_SLASH,			// KeyboardKey_Slash
		SDLK_ASTERISK,		// KeyboardKey_Star
		SDLK_EQUALS,		// KeyboardKey_Equals
		SDLK_SEMICOLON,		// KeyboardKey_Semicolon
		SDLK_QUOTE,			// KeyboardKey_Apostrophe
		SDLK_BACKSLASH,		// KeyboardKey_BackSlash
		SDLK_COMMA,			// KeyboardKey_Comma
		SDLK_PERIOD,		// KeyboardKey_Dot
		SDLK_BACKQUOTE,		// KeyboardKey_GraveAccent
		
		SDLK_1,				// KeyboardKey_Alpha1
		SDLK_2,				// KeyboardKey_Alpha2
		SDLK_3,				// KeyboardKey_Alpha3
		SDLK_4,				// KeyboardKey_Alpha4
		SDLK_5,				// KeyboardKey_Alpha5
		SDLK_6,				// KeyboardKey_Alpha6
		SDLK_7,				// KeyboardKey_Alpha7
		SDLK_8,				// KeyboardKey_Alpha8
		SDLK_9,				// KeyboardKey_Alpha9
		SDLK_0,				// KeyboardKey_Alpha0

		SDLK_a,				// KeyboardKey_A
		SDLK_b,				// KeyboardKey_B
		SDLK_c,				// KeyboardKey_C
		SDLK_d,				// KeyboardKey_D
		SDLK_e,				// KeyboardKey_E
		SDLK_f,				// KeyboardKey_F
		SDLK_g,				// KeyboardKey_G
		SDLK_h,				// KeyboardKey_H
		SDLK_i,				// KeyboardKey_I
		SDLK_j,				// KeyboardKey_J
		SDLK_k,				// KeyboardKey_K
		SDLK_l,				// KeyboardKey_L
		SDLK_m,				// KeyboardKey_M
		SDLK_n,				// KeyboardKey_N
		SDLK_o,				// KeyboardKey_O
		SDLK_p,				// KeyboardKey_P
		SDLK_q,				// KeyboardKey_Q
		SDLK_r,				// KeyboardKey_R
		SDLK_s,				// KeyboardKey_S
		SDLK_t,				// KeyboardKey_T
		SDLK_u,				// KeyboardKey_U
		SDLK_v,				// KeyboardKey_V
		SDLK_w,				// KeyboardKey_W
		SDLK_x,				// KeyboardKey_X
		SDLK_y,				// KeyboardKey_Y
		SDLK_z,				// KeyboardKey_Z

		SDLK_F1,			// KeyboardKey_F1
		SDLK_F2,			// KeyboardKey_F2
		SDLK_F3,			// KeyboardKey_F3
		SDLK_F4,			// KeyboardKey_F4
		SDLK_F5,			// KeyboardKey_F5
		SDLK_F6,			// KeyboardKey_F6
		SDLK_F7,			// KeyboardKey_F7
		SDLK_F8,			// KeyboardKey_F8
		SDLK_F9,			// KeyboardKey_F9
		SDLK_F10,			// KeyboardKey_F10
		SDLK_F11,			// KeyboardKey_F11
		SDLK_F12,			// KeyboardKey_F12

		SDLK_KP_0,			// KeyboardKey_Numpad0
		SDLK_KP_1,			// KeyboardKey_Numpad1
		SDLK_KP_2,			// KeyboardKey_Numpad2
		SDLK_KP_3,			// KeyboardKey_Numpad3
		SDLK_KP_4,			// KeyboardKey_Numpad4
		SDLK_KP_5,			// KeyboardKey_Numpad5
		SDLK_KP_6,			// KeyboardKey_Numpad6
		SDLK_KP_7,			// KeyboardKey_Numpad7
		SDLK_KP_8,			// KeyboardKey_Numpad8
		SDLK_KP_9,			// KeyboardKey_Numpad9
		SDLK_KP_EQUALS,		// KeyboardKey_NumpadEquals
		SDLK_KP_PERIOD,		// KeyboardKey_NumpadComma
		SDLK_KP_ENTER,		// KeyboardKey_NumpadEnter

		SDLK_CAPSLOCK,		// KeyboardKey_CapsLock
		SDLK_NUMLOCKCLEAR,	// KeyboardKey_Numlock
		SDLK_SCROLLLOCK,	// KeyboardKey_Scroll
	};

	static const uint16 s_aControllerButtonMapping[ ControllerButton_Count ] = 
	{
		0,		// ControllerButton_A
		1,		// ControllerButton_B
		2,		// ControllerButton_X
		3,		// ControllerButton_Y

		4,		// ControllerButton_LeftShoulder
		5,		// ControllerButton_RightShoulder

		6,		// ControllerButton_LeftStick
		7,		// ControllerButton_RightStick

		8,		// ControllerButton_DPadLeft
		9,		// ControllerButton_DPadRight
		10,		// ControllerButton_DPadUp
		11,		// ControllerButton_DPadDown

		12,		// ControllerButton_Start
		13		// ControllerButton_Back
	};
	
	static const MouseButton s_aMouseButtonMapping[] =
	{
		MouseButton_Invalid,	// unused
		MouseButton_Left,		// SDL_BUTTON_LEFT
		MouseButton_Middle,		// SDL_BUTTON_MIDDLE
		MouseButton_Right,		// SDL_BUTTON_RIGHT
		MouseButton_Invalid,	// SDL_BUTTON_X1
		MouseButton_Invalid		// SDL_BUTTON_X2
	};
	TIKI_COMPILETIME_ASSERT( SDL_BUTTON_LEFT	== 1 );
	TIKI_COMPILETIME_ASSERT( SDL_BUTTON_MIDDLE	== 2 );
	TIKI_COMPILETIME_ASSERT( SDL_BUTTON_RIGHT	== 3 );

	InputSystem::InputSystem()
	{
	}

	InputSystem::~InputSystem()
	{
	}

	bool InputSystem::create( const InputSystemParameters& params )
	{
		// keyboard mapping
		m_platformData.keyboardMapping.create( TIKI_COUNT( s_aVirtualKeyCodeMapping ) );

		for (uint i = 0u; i < TIKI_COUNT( s_aVirtualKeyCodeMapping ); ++i)
		{
			const int sdlCode = s_aVirtualKeyCodeMapping[ i ];
			m_platformData.keyboardMapping.set( sdlCode, (KeyboardKey)i );
		} 

		return true;
	}

	void InputSystem::dispose()
	{
		m_platformData.keyboardMapping.dispose();
	}

	void InputSystem::update()
	{
		m_events.clear();
	}

	void InputSystem::endFrame()
	{
	}

	uint InputSystem::getEventCount() const
	{
		return m_events.getCount();
	}

	bool InputSystem::popEvent( InputEvent& inputEvent )
	{
		return m_events.pop( inputEvent );
	}
	
	void InputSystem::connectDevice( const InputDevice& device )
	{
		m_devices.push( device );

		InputEvent& inputEvent = m_events.push();
		inputEvent.eventType	= InputEventType_Device_Connected;
		inputEvent.deviceType	= device.deviceType;
		inputEvent.deviceId		= device.deviceId;
		inputEvent.data.device.device = device;
	}

	void InputSystem::disconnectDevice( const InputDevice& device )
	{
		if ( m_devices.removeUnsortedByValue( device ) )
		{
			InputEvent& inputEvent = m_events.push();
			inputEvent.eventType	= InputEventType_Device_Disconnected;
			inputEvent.deviceType	= device.deviceType;
			inputEvent.deviceId		= device.deviceId;
			inputEvent.data.device.device = device;
		}
	}

	void InputSystemPlatform::processEvent( InputSystem& inputSystem, const SDL_Event& sdlEvent )
	{
		switch ( sdlEvent.type )
		{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			processKeyEvent( inputSystem, sdlEvent.key );
			break;

		case SDL_MOUSEMOTION:
			processMouseMotionEvent( inputSystem, sdlEvent.motion );
			break;

		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			processMouseButtonEvent( inputSystem, sdlEvent.button );
			break;

		default:
			break;
		}	
	}

	void InputSystemPlatform::processKeyEvent( InputSystem& inputSystem, const SDL_KeyboardEvent& keyboardEvent )
	{
		const InputEventType eventType = (keyboardEvent.type == SDL_KEYDOWN ? InputEventType_Keyboard_Down : InputEventType_Keyboard_Up);

		KeyboardKey key = KeyboardKey_Invalid;
		if ( !inputSystem.m_platformData.keyboardMapping.findValue( &key, keyboardEvent.keysym.sym ) )
		{
			return;
		}

		InputEvent& inputEvent = inputSystem.m_events.push();
		inputEvent.eventType			= eventType;
		inputEvent.deviceType			= InputDeviceType_Keyboard;
		inputEvent.deviceId				= 0u;
		inputEvent.data.keybaordKey.key	= key;
	}

	void InputSystemPlatform::processMouseMotionEvent( InputSystem& inputSystem, const SDL_MouseMotionEvent& mouseMotionEvent )
	{
		InputEvent& inputEvent = inputSystem.m_events.push();
		inputEvent.eventType	= InputEventType_Mouse_Moved;
		inputEvent.deviceType	= InputDeviceType_Mouse;
		inputEvent.deviceId		= 0u;
		inputEvent.data.mouseMoved.xOffset = (sint16)mouseMotionEvent.xrel;
		inputEvent.data.mouseMoved.yOffset = (sint16)mouseMotionEvent.yrel;
		inputEvent.data.mouseMoved.xState = (sint16)mouseMotionEvent.x;
		inputEvent.data.mouseMoved.yState = (sint16)mouseMotionEvent.y;
	}

	void InputSystemPlatform::processMouseButtonEvent( InputSystem& inputSystem, const SDL_MouseButtonEvent& mouseButtonEvent )
	{
		const InputEventType eventType = (mouseButtonEvent.type == SDL_MOUSEBUTTONDOWN ? InputEventType_Mouse_ButtonDown : InputEventType_Mouse_ButtonUp);
		
		TIKI_ASSERT( mouseButtonEvent.button < TIKI_COUNT( s_aMouseButtonMapping ) );
		TIKI_ASSERT( s_aMouseButtonMapping[ mouseButtonEvent.button ] != MouseButton_Invalid );

		InputEvent& inputEvent = inputSystem.m_events.push();
		inputEvent.eventType				= eventType;
		inputEvent.deviceType				= InputDeviceType_Mouse;
		inputEvent.deviceId					= 0u;
		inputEvent.data.mouseButton.button	= s_aMouseButtonMapping[ mouseButtonEvent.button ];
	}
}