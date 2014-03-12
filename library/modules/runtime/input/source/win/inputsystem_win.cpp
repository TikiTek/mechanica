
#include "tiki/input/inputsystem.hpp"

#include "tiki/base/platform.hpp"
#include "tiki/framework/mainwindow.hpp"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <xinput.h>
#include <Windows.h>

namespace tiki
{
	static const uint8 s_aVirtualKeyCodeMapping[ KeyboardKey_Count ] =
	{
		DIK_ESCAPE,		// KeyboardKey_Escape
		DIK_BACKSPACE,	// KeyboardKey_BackSpace
		DIK_TAB,		// KeyboardKey_Tab
		DIK_RETURN,		// KeyboardKey_Return
		DIK_LCONTROL,	// KeyboardKey_LeftControl
		DIK_RCONTROL,	// KeyboardKey_RightControl
		DIK_LSHIFT,		// KeyboardKey_LeftShift
		DIK_RSHIFT,		// KeyboardKey_RightShift
		DIK_LMENU,		// KeyboardKey_LeftAlt
		DIK_RMENU,		// KeyboardKey_RightAlt
		DIK_LWIN,		// KeyboardKey_LeftSystem
		DIK_RWIN,		// KeyboardKey_RightSystem

		DIK_INSERT,		// KeyboardKey_Insert
		DIK_DELETE,		// KeyboardKey_Delete
		DIK_HOME,		// KeyboardKey_Home
		DIK_END,		// KeyboardKey_End
		DIK_PRIOR,		// KeyboardKey_PageUp
		DIK_NEXT,		// KeyboardKey_PageDown

		DIK_LEFT,		// KeyboardKey_Left
		DIK_UP,			// KeyboardKey_Up
		DIK_DOWN,		// KeyboardKey_Down
		DIK_RIGHT,		// KeyboardKey_Right

		DIK_SPACE,		// KeyboardKey_Space
		DIK_MINUS,		// KeyboardKey_Minus
		DIK_ADD,		// KeyboardKey_Plus
		DIK_DIVIDE,		// KeyboardKey_Slash
		DIK_MULTIPLY,	// KeyboardKey_Star
		DIK_EQUALS,		// KeyboardKey_Equals
		DIK_SEMICOLON,	// KeyboardKey_Semicolon
		DIK_APOSTROPHE,	// KeyboardKey_Apostrophe
		DIK_BACKSLASH,	// KeyboardKey_BackSlash
		DIK_COMMA,		// KeyboardKey_Comma
		DIK_DECIMAL,	// KeyboardKey_Dot
		DIK_GRAVE,		// KeyboardKey_GraveAccent
		
		DIK_1,			// KeyboardKey_Alpha1
		DIK_2,			// KeyboardKey_Alpha2
		DIK_3,			// KeyboardKey_Alpha3
		DIK_4,			// KeyboardKey_Alpha4
		DIK_5,			// KeyboardKey_Alpha5
		DIK_6,			// KeyboardKey_Alpha6
		DIK_7,			// KeyboardKey_Alpha7
		DIK_8,			// KeyboardKey_Alpha8
		DIK_9,			// KeyboardKey_Alpha9
		DIK_0,			// KeyboardKey_Alpha0

		DIK_A,			// KeyboardKey_A
		DIK_B,			// KeyboardKey_B
		DIK_C,			// KeyboardKey_C
		DIK_D,			// KeyboardKey_D
		DIK_E,			// KeyboardKey_E
		DIK_F,			// KeyboardKey_F
		DIK_G,			// KeyboardKey_G
		DIK_H,			// KeyboardKey_H
		DIK_I,			// KeyboardKey_I
		DIK_J,			// KeyboardKey_J
		DIK_K,			// KeyboardKey_K
		DIK_L,			// KeyboardKey_L
		DIK_M,			// KeyboardKey_M
		DIK_N,			// KeyboardKey_N
		DIK_O,			// KeyboardKey_O
		DIK_P,			// KeyboardKey_P
		DIK_Q,			// KeyboardKey_Q
		DIK_R,			// KeyboardKey_R
		DIK_S,			// KeyboardKey_S
		DIK_T,			// KeyboardKey_T
		DIK_U,			// KeyboardKey_U
		DIK_V,			// KeyboardKey_V
		DIK_W,			// KeyboardKey_W
		DIK_X,			// KeyboardKey_X
		DIK_Y,			// KeyboardKey_Y
		DIK_Z,			// KeyboardKey_Z

		DIK_F1,				// KeyboardKey_F1
		DIK_F2,				// KeyboardKey_F2
		DIK_F3,				// KeyboardKey_F3
		DIK_F4,				// KeyboardKey_F4
		DIK_F5,				// KeyboardKey_F5
		DIK_F6,				// KeyboardKey_F6
		DIK_F7,				// KeyboardKey_F7
		DIK_F8,				// KeyboardKey_F8
		DIK_F9,				// KeyboardKey_F9
		DIK_F10,			// KeyboardKey_F10
		DIK_F11,			// KeyboardKey_F11
		DIK_F12,			// KeyboardKey_F12

		DIK_NUMPAD0,		// KeyboardKey_Numpad0
		DIK_NUMPAD1,		// KeyboardKey_Numpad1
		DIK_NUMPAD2,		// KeyboardKey_Numpad2
		DIK_NUMPAD3,		// KeyboardKey_Numpad3
		DIK_NUMPAD4,		// KeyboardKey_Numpad4
		DIK_NUMPAD5,		// KeyboardKey_Numpad5
		DIK_NUMPAD6,		// KeyboardKey_Numpad6
		DIK_NUMPAD7,		// KeyboardKey_Numpad7
		DIK_NUMPAD8,		// KeyboardKey_Numpad8
		DIK_NUMPAD9,		// KeyboardKey_Numpad9
		DIK_NUMPADEQUALS,	// KeyboardKey_NumpadEquals
		DIK_NUMPADCOMMA,	// KeyboardKey_NumpadComma
		DIK_NUMPADENTER,	// KeyboardKey_NumpadEnter

		DIK_CAPSLOCK,		// KeyboardKey_CapsLock
		DIK_NUMLOCK,		// KeyboardKey_Numlock
		DIK_SCROLL,			// KeyboardKey_Scroll
	};

	struct InputSystemState
	{
		DIMOUSESTATE	mouse;
		uint8			keyboard[ 256u ];

		bool			controllerConnected[ XUSER_MAX_COUNT ];
		XINPUT_STATE	controller[ XUSER_MAX_COUNT ];
	};

	InputSystem::InputSystem()
	{
	}

	InputSystem::~InputSystem()
	{
		TIKI_ASSERT( m_platformData.pInputDevice	== nullptr );
		TIKI_ASSERT( m_platformData.pMouse			== nullptr );
		TIKI_ASSERT( m_platformData.pKeyboard		== nullptr );
	}

	bool InputSystem::create( const InputSystemParameters& params )
	{
		const HINSTANCE hInstance = (HINSTANCE)getInstanceHandle();

		m_platformData.currentStateIndex = 0u;
		m_platformData.pStates[ 0u ] = static_cast< InputSystemState* >( TIKI_MEMORY_ALLOC( sizeof( InputSystemState ) * 2u ) );
		m_platformData.pStates[ 1u ] = m_platformData.pStates[ 0u ] + 1u;
		memory::zero( m_platformData.pStates[ 0u ], sizeof( InputSystemState ) * 2u );

		HRESULT result = DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_platformData.pInputDevice, nullptr );
		if( FAILED( result ) || m_platformData.pInputDevice == nullptr )
		{
			dispose();
			return false;
		}

		// keyboard
		for (uint i = 0u; i < TIKI_COUNT( m_platformData.keyboardMapping ); ++i)
		{
			m_platformData.keyboardMapping[ i ] = 0xffu;
		} 

		for (uint i = 0u; i < TIKI_COUNT( s_aVirtualKeyCodeMapping ); ++i)
		{
			const uint8 directInputCode = s_aVirtualKeyCodeMapping[ i ];
			m_platformData.keyboardMapping[ directInputCode ] = i;
		} 

		result = m_platformData.pInputDevice->CreateDevice( GUID_SysKeyboard, &m_platformData.pKeyboard, nullptr );
		if( SUCCEEDED( result ) && m_platformData.pKeyboard != nullptr )
		{
			if( SUCCEEDED( m_platformData.pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
			{
				result = m_platformData.pKeyboard->SetCooperativeLevel( (HWND)params.windowHandle, DISCL_FOREGROUND | DISCL_EXCLUSIVE );
				if( SUCCEEDED( result ) )
				{
					InputDevice device;
					device.deviceType	= InputDeviceType_Keyboard;
					device.deviceId		= 0u;
					connectDevice( device );
				}
			}
		}
		
		// mouse
		result = m_platformData.pInputDevice->CreateDevice( GUID_SysMouse, &m_platformData.pMouse, nullptr );
		if( SUCCEEDED( result ) && m_platformData.pMouse != nullptr )
		{
			result = m_platformData.pMouse->SetDataFormat( &c_dfDIMouse );
			if( SUCCEEDED( result ) )
			{
				result = m_platformData.pMouse->SetCooperativeLevel( (HWND)params.windowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
				if( SUCCEEDED( result ) )
				{
					InputDevice device;
					device.deviceType	= InputDeviceType_Mouse;
					device.deviceId		= 0u;
					connectDevice( device );
				}
			}
		}

		// controller
		for (uint controllerIndex = 0u; controllerIndex < XUSER_MAX_COUNT; ++controllerIndex)
		{
			if ( XInputGetState( controllerIndex, &m_platformData.pStates[ 0u ]->controller[ controllerIndex ] ) == ERROR_SUCCESS )
			{
				m_platformData.pStates[ 0u ]->controllerConnected[ controllerIndex ] = true;

				InputDevice device;
				device.deviceType	= InputDeviceType_Controller;
				device.deviceId		= controllerIndex;
				connectDevice( device );
			}
		} 

		return true;
	}

	void InputSystem::dispose()
	{
		if ( m_platformData.pStates[ 0u ] != nullptr )
		{
			TIKI_MEMORY_FREE( m_platformData.pStates[ 0u ] );
			m_platformData.pStates[ 0u ] = nullptr;
			m_platformData.pStates[ 1u ] = nullptr;
		}

		if ( m_platformData.pKeyboard != nullptr )
		{
			m_platformData.pKeyboard->Unacquire();

			m_platformData.pKeyboard->Release();
			m_platformData.pKeyboard = nullptr;
		}

		if ( m_platformData.pMouse != nullptr )
		{
			m_platformData.pMouse->Unacquire();

			m_platformData.pMouse->Release();
			m_platformData.pMouse = nullptr;
		}

		if ( m_platformData.pInputDevice != nullptr )
		{
			m_platformData.pInputDevice->Release();
			m_platformData.pInputDevice = nullptr;
		}
	}

	void InputSystem::update()
	{
		m_events.clear();

		const InputSystemState* pPreviousState = m_platformData.pStates[ m_platformData.currentStateIndex ];

		m_platformData.currentStateIndex = 1u - m_platformData.currentStateIndex;
		InputSystemState* pCurrentState = m_platformData.pStates[ m_platformData.currentStateIndex ];

		HRESULT result = DIERR_UNSUPPORTED;		
		if ( m_platformData.pKeyboard != nullptr )
		{
			result = m_platformData.pKeyboard->GetDeviceState( sizeof( pCurrentState->keyboard ), &pCurrentState->keyboard );
			if( FAILED( result ) )
			{
				if( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED )
				{
					m_platformData.pKeyboard->Acquire();
				}
				else
				{
					TIKI_TRACE_ERROR( "[input] Get keyboard state has returned an error. Code: 0x%08x\n", result );
				}
			}
			else
			{
				for (uint i = 0u; i < TIKI_COUNT( pCurrentState->keyboard ); ++i)
				{
					const KeyboardKey mappedKey = static_cast< KeyboardKey >( m_platformData.keyboardMapping[ i ] );
					if ( mappedKey >= KeyboardKey_Count )
					{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
						const bool isPressed = isBitSet( pCurrentState->keyboard[ i ], 0x80u );
						const bool wasPressed = isBitSet( pPreviousState->keyboard[ i ], 0x80u );
						if ( isPressed != wasPressed )
						{
							TIKI_TRACE_INFO( "[input] input received from unmapped key with code: %u\n", i );
						}
#endif

						continue;
					}

					const bool isPressed = isBitSet( pCurrentState->keyboard[ i ], 0x80u );
					const bool wasPressed = isBitSet( pPreviousState->keyboard[ i ], 0x80u );

					if ( isPressed && !wasPressed )
					{
						InputEvent& inputEvent = m_events.push();
						inputEvent.eventType	= InputEventType_Keyboard_Down;
						inputEvent.deviceType	= InputDeviceType_Keyboard;
						inputEvent.deviceId		= 0u;
						inputEvent.data.keybaordKey.key = mappedKey;
					}
					else if ( !isPressed && wasPressed )
					{
						InputEvent& inputEvent = m_events.push();
						inputEvent.eventType	= InputEventType_Keyboard_Up;
						inputEvent.deviceType	= InputDeviceType_Keyboard;
						inputEvent.deviceId		= 0u;
						inputEvent.data.keybaordKey.key = mappedKey;
					}
				} 
			}
		}

		if ( m_platformData.pMouse != nullptr )
		{
			result = m_platformData.pMouse->GetDeviceState( sizeof( pCurrentState->mouse ), &pCurrentState->mouse );
			if( FAILED( result ) )
			{
				if( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED )
				{
					m_platformData.pMouse->Acquire();
				}
				else
				{
					TIKI_TRACE_ERROR( "[input] Get mouse state has returned an error. Code: 0x%08x\n", result );
				}
			}
			else
			{
				// check for changes
			}
		}

		// controller
		for (uint controllerIndex = 0u; controllerIndex < XUSER_MAX_COUNT; ++controllerIndex)
		{
			if ( XInputGetState( controllerIndex, &m_platformData.pStates[ 0u ]->controller[ controllerIndex ] ) == ERROR_SUCCESS )
			{
				// check for changes				
			}
			else if ( pCurrentState->controllerConnected[ controllerIndex ] )
			{
				InputDevice device;
				device.deviceType	= InputDeviceType_Controller;
				device.deviceId		= controllerIndex;
				disconnectDevice( device );

				pCurrentState->controllerConnected[ controllerIndex ] = false;
			}
		}		
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

	//bool InputSystem::isButtonDown( const MouseButtons button ) const
	//{
	//	const DIMOUSESTATE* pState = reinterpret_cast< const DIMOUSESTATE* >( m_currentState.mouseState );
	//	return pState->rgbButtons[ button ] != 0;
	//}

	//bool InputSystem::isButtonUp( const MouseButtons button ) const
	//{
	//	const DIMOUSESTATE* pState = reinterpret_cast< const DIMOUSESTATE* >( m_currentState.mouseState );
	//	return pState->rgbButtons[ button ] == 0;
	//}

	//bool InputSystem::isButtonPressed( const MouseButtons button ) const
	//{
	//	const DIMOUSESTATE* pPreviousState = reinterpret_cast< const DIMOUSESTATE* >( m_previousState.mouseState );
	//	const DIMOUSESTATE* pCurrentState = reinterpret_cast< const DIMOUSESTATE* >( m_currentState.mouseState );

	//	return ( pPreviousState->rgbButtons[ button ] == 0 ) && ( pCurrentState->rgbButtons[ button ] != 0 );
	//}

	//bool InputSystem::isButtonReleased( const MouseButtons button ) const
	//{
	//	const DIMOUSESTATE* pPreviousState = reinterpret_cast< const DIMOUSESTATE* >( m_previousState.mouseState );
	//	const DIMOUSESTATE* pCurrentState = reinterpret_cast< const DIMOUSESTATE* >( m_currentState.mouseState );

	//	return ( pPreviousState->rgbButtons[ button ] != 0 ) && ( pCurrentState->rgbButtons[ button ] == 0 );
	//}

	//bool InputSystem::isKeyDown( Keys key ) const
	//{
	//	return ( m_currentState.keyboardState[ key ] & 0x80 ) != 0;
	//}

	//bool InputSystem::isKeyUp( Keys key ) const
	//{
	//	return ( m_currentState.keyboardState[ key ] & 0x80 ) == 0;
	//}

	//bool InputSystem::hasKeyPressed( Keys key ) const
	//{
	//	return ( !( m_previousState.keyboardState[ key ] & 0x80 ) && ( m_currentState.keyboardState[ key ] & 0x80 ) );
	//}

	//bool InputSystem::hasKeyReleased( Keys key ) const
	//{
	//	return (( m_previousState.keyboardState[ key ] & 0x80 ) && !( m_currentState.keyboardState[ key ] & 0x80 ) );
	//}
}