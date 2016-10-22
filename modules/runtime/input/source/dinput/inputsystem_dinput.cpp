#include "tiki/input/inputsystem.hpp"

#include "tiki/base/functions.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/framework/mainwindow.hpp"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <xinput.h>
#include <Windows.h>

namespace tiki
{
	struct TouchInputState;

	namespace input
	{
		static bool									initializeKeyboard( InputSystemPlatformData& platformData, const InputSystemParameters& parameters );
		static bool									initializeMouse( InputSystemPlatformData& platformData, const InputSystemParameters& parameters );
		static bool									initializeTouch( InputSystemPlatformData& platformData, const InputSystemParameters& parameters );
		static bool									initializeWindow( InputSystemPlatformData& platformData, InputSystem* pInputSystem );

		static bool									handleWindowMessage( void* pUserData, UINT message, WPARAM wParam, LPARAM lParam );

		TIKI_FORCE_INLINE static void				checkControllerTrigger( InputSystem::InputEventArray& events, uint controllerIndex, uint32 triggerIndex, uint8 currentState, uint8 previousState );
		TIKI_FORCE_INLINE static void				checkControllerStick( InputSystem::InputEventArray& events, uint controllerIndex, uint32 stickIndex, sint16 currentStateX, sint16 previousStateX, sint16 currentStateY, sint16 previousStateY );
		TIKI_FORCE_INLINE static void				checkControllerButton( InputSystem::InputEventArray& events, uint controllerIndex, ControllerButton button, uint16 nativeButton, sint16 currentState, sint16 previousState );
		TIKI_FORCE_INLINE static TouchInputState	getTouchInputMapping( uint32 touchId, uint32* pTouchMapping, uint touchMappingCapacity );
	}

	struct TouchInputState
	{
		bool			isValid;
		bool			isNewPoint;
		uint			index;
	};

	struct InputSystemState
	{
		DIMOUSESTATE	mouse;
		uint8			aKeyboard[ 256u ];

		bool			aControllerConnected[ XUSER_MAX_COUNT ];
		XINPUT_STATE	aController[ XUSER_MAX_COUNT ];

		TOUCHINPUT		aTouchPoints[ MaxInputTouchPoints ];
	};

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

	static const uint16 s_aControllerButtonMapping[ ControllerButton_Count ] = 
	{
		XINPUT_GAMEPAD_A,				// ControllerButton_A
		XINPUT_GAMEPAD_B,				// ControllerButton_B
		XINPUT_GAMEPAD_X,				// ControllerButton_X
		XINPUT_GAMEPAD_Y,				// ControllerButton_Y

		XINPUT_GAMEPAD_LEFT_SHOULDER,	// ControllerButton_LeftShoulder
		XINPUT_GAMEPAD_RIGHT_SHOULDER,	// ControllerButton_RightShoulder

		XINPUT_GAMEPAD_LEFT_THUMB,		// ControllerButton_LeftStick
		XINPUT_GAMEPAD_RIGHT_THUMB,		// ControllerButton_RightStick

		XINPUT_GAMEPAD_DPAD_LEFT,		// ControllerButton_DPadLeft
		XINPUT_GAMEPAD_DPAD_RIGHT,		// ControllerButton_DPadRight
		XINPUT_GAMEPAD_DPAD_UP,			// ControllerButton_DPadUp
		XINPUT_GAMEPAD_DPAD_DOWN,		// ControllerButton_DPadDown

		XINPUT_GAMEPAD_START,			// ControllerButton_Start
		XINPUT_GAMEPAD_BACK				// ControllerButton_Back
	};

	static const MouseButton s_aMouseButtonMapping[ MouseButton_Count ] =
	{
		MouseButton_Left,
		MouseButton_Right,
		MouseButton_Middle
	};

	TIKI_FORCE_INLINE static void input::checkControllerTrigger( InputSystem::InputEventArray& events, uint controllerIndex, uint32 triggerIndex, uint8 currentState, uint8 previousState )
	{
		if ( currentState != previousState )
		{
			if ( currentState < XINPUT_GAMEPAD_TRIGGER_THRESHOLD )
			{
				currentState = 0u;
			}

			InputEvent& inputEvent = events.push();
			inputEvent.eventType	= InputEventType_Controller_TriggerChanged;
			inputEvent.deviceType	= InputDeviceType_Controller;
			inputEvent.deviceId		= controllerIndex;
			inputEvent.data.controllerTrigger.triggerIndex	= triggerIndex;
			inputEvent.data.controllerTrigger.state			= (float)currentState / 255.0f;
		}
	}

	TIKI_FORCE_INLINE static void input::checkControllerStick( InputSystem::InputEventArray& events, uint controllerIndex, uint32 stickIndex, sint16 currentStateX, sint16 previousStateX, sint16 currentStateY, sint16 previousStateY )
	{
		if ( currentStateX != previousStateX || currentStateY != previousStateY )
		{
			if ( abs( currentStateX ) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
			{
				currentStateX = 0u;
			}

			if ( abs( currentStateY ) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
			{
				currentStateY = 0u;
			}

			InputEvent& inputEvent = events.push();
			inputEvent.eventType	= InputEventType_Controller_StickChanged;
			inputEvent.deviceType	= InputDeviceType_Controller;
			inputEvent.deviceId		= controllerIndex;
			inputEvent.data.controllerStick.stickIndex		= stickIndex;
			inputEvent.data.controllerStick.xState			= (float)currentStateX / 32767.0f;
			inputEvent.data.controllerStick.yState			= (float)currentStateY / -32767.0f;
		}
	}

	TIKI_FORCE_INLINE static void input::checkControllerButton( InputSystem::InputEventArray& events, uint controllerIndex, ControllerButton button, uint16 nativeButton, sint16 currentState, sint16 previousState )
	{
		const bool isPressed	= isBitSet( currentState, nativeButton );
		const bool wasPressed	= isBitSet( previousState, nativeButton );

		if ( isPressed && !wasPressed )
		{
			InputEvent& inputEvent = events.push();
			inputEvent.eventType	= InputEventType_Controller_ButtonDown;
			inputEvent.deviceType	= InputDeviceType_Controller;
			inputEvent.deviceId		= controllerIndex;
			inputEvent.data.controllerButton.button = button;
		}
		else if ( !isPressed && wasPressed )
		{
			InputEvent& inputEvent = events.push();
			inputEvent.eventType	= InputEventType_Controller_ButtonUp;
			inputEvent.deviceType	= InputDeviceType_Controller;
			inputEvent.deviceId		= controllerIndex;
			inputEvent.data.controllerButton.button = button;
		}
	}

	TIKI_FORCE_INLINE static TouchInputState input::getTouchInputMapping( uint32 touchId, uint32* pTouchMapping, uint touchMappingCapacity )
	{
		uint firstFreeIndex = TIKI_SIZE_T_MAX;
		for (uint i = 0u; i < touchMappingCapacity; ++i)
		{
			if ( pTouchMapping[ i ] == touchId )
			{
				TouchInputState state;
				state.isValid		= true;
				state.isNewPoint	= false;
				state.index			= i;

				return state;
			}
			else if ( pTouchMapping[ i ] == InvalidTouchInputMapping )
			{
				firstFreeIndex = i;
			}
		}

		if ( firstFreeIndex != TIKI_SIZE_T_MAX )
		{
			TouchInputState state;
			state.isValid		= true;
			state.isNewPoint	= true;
			state.index			= firstFreeIndex;
			
			return state;
		}

		TouchInputState state;
		state.isValid		= false;
		state.isNewPoint	= false;
		state.index			= TIKI_SIZE_T_MAX;

		return state;
	}

	bool input::initializeKeyboard( InputSystemPlatformData& platformData, const InputSystemParameters& parameters )
	{
		for( uint i = 0u; i < TIKI_COUNT( platformData.keyboardMapping ); ++i )
		{
			platformData.keyboardMapping[ i ] = 0xffu;
		}

		for( uint i = 0u; i < TIKI_COUNT( s_aVirtualKeyCodeMapping ); ++i )
		{
			const uint8 directInputCode = s_aVirtualKeyCodeMapping[ i ];
			platformData.keyboardMapping[ directInputCode ] = uint8( i );
		}

		HRESULT result = platformData.pInputDevice->CreateDevice( GUID_SysKeyboard, &platformData.pKeyboard, nullptr );
		if( SUCCEEDED( result ) && platformData.pKeyboard != nullptr )
		{
			if( SUCCEEDED( platformData.pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
			{
				result = platformData.pKeyboard->SetCooperativeLevel( (HWND)parameters.windowHandle, DISCL_FOREGROUND | DISCL_EXCLUSIVE );
				if( SUCCEEDED( result ) )
				{
					return true;
				}
			}
		}

		return false;
	}

	bool input::initializeMouse( InputSystemPlatformData& platformData, const InputSystemParameters& parameters )
	{
		HRESULT result = platformData.pInputDevice->CreateDevice( GUID_SysMouse, &platformData.pMouse, nullptr );
		if( SUCCEEDED( result ) && platformData.pMouse != nullptr )
		{
			result = platformData.pMouse->SetDataFormat( &c_dfDIMouse );
			if( SUCCEEDED( result ) )
			{
				result = platformData.pMouse->SetCooperativeLevel( (HWND)parameters.windowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
				if( SUCCEEDED( result ) )
				{
					return true;
				}
			}
		}

		return false;
	}


	bool input::initializeTouch( InputSystemPlatformData& platformData, const InputSystemParameters& parameters )
	{
		const int touchMask = GetSystemMetrics( SM_DIGITIZER );
		platformData.isTouchInputReady = isBitSet( touchMask, NID_READY );

		for( uint i = 0u; i < TIKI_COUNT( platformData.touchInputMapping ); ++i )
		{
			platformData.touchInputMapping[ i ] = InvalidTouchInputMapping;
		}

		if( platformData.isTouchInputReady )
		{
			RegisterTouchWindow( (HWND)platformData.windowHandle, 0 );

			return true;
		}

		return false;		
	}

	bool input::initializeWindow( InputSystemPlatformData& platformData, InputSystem* pInputSystem )
	{
		MainWindowUserData* pUserData = (MainWindowUserData*)GetWindowLongPtr( (HWND)platformData.windowHandle, GWLP_USERDATA );
		if( pUserData != nullptr )
		{
			pUserData->apUserData[ MainWindowMesageHandler_InputSystem ]		= pInputSystem;
			pUserData->apHandlerFunction[ MainWindowMesageHandler_InputSystem ]	= input::handleWindowMessage;

			return true;
		}

		return false;
	}

	bool input::handleWindowMessage( void* pUserData, UINT message, WPARAM wParam, LPARAM lParam )
	{
		InputSystem& inputSystem = *(InputSystem*)pUserData;
		return InputSystemPlatform::handleWindowMessage( inputSystem, message, wParam, lParam );
	}

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
		const HINSTANCE hInstance = (HINSTANCE)params.instanceHandle;

		m_platformData.windowHandle			= params.windowHandle;
		m_platformData.currentStateIndex	= 0u;

		if( !input::initializeWindow( m_platformData, this ) )
		{
			dispose();
			return false;
		}

		m_platformData.pStates[ 0u ] = static_cast< InputSystemState* >( TIKI_ALLOC( sizeof( InputSystemState ) * 2u ) );
		m_platformData.pStates[ 1u ] = m_platformData.pStates[ 0u ] + 1u;
		memory::zero( m_platformData.pStates[ 0u ], sizeof( InputSystemState ) * 2u );

		HRESULT result = DirectInput8Create( hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_platformData.pInputDevice, nullptr );
		if( FAILED( result ) || m_platformData.pInputDevice == nullptr )
		{
			dispose();
			return false;
		}

		if( input::initializeKeyboard( m_platformData, params ) )
		{
			InputDevice device;
			device.deviceType	= InputDeviceType_Keyboard;
			device.deviceId		= 0u;
			connectDevice( device );
		}
		else
		{
			TIKI_TRACE_ERROR( "[input] Unable to initilaize Keyboard.\n" );
		}
		
		if( input::initializeMouse( m_platformData, params ) )
		{
			InputDevice device;
			device.deviceType	= InputDeviceType_Mouse;
			device.deviceId		= 0u;
			connectDevice( device );
		}
		else
		{
			TIKI_TRACE_ERROR( "[input] Unable to initilaize Mouse.\n" );
		}
		
		// controller
		for( uint controllerIndex = 0u; controllerIndex < XUSER_MAX_COUNT; ++controllerIndex )
		{
			if( XInputGetState( DWORD( controllerIndex ), &m_platformData.pStates[ 0u ]->aController[ controllerIndex ] ) == ERROR_SUCCESS )
			{
				m_platformData.pStates[ 0u ]->aControllerConnected[ controllerIndex ] = true;

				InputDevice device;
				device.deviceType	= InputDeviceType_Controller;
				device.deviceId		= controllerIndex;
				connectDevice( device );
			}
		}

		// touch
		if( input::initializeTouch( m_platformData, params ) )
		{
			InputDevice device;
			device.deviceType	= InputDeviceType_Touch;
			device.deviceId		= 0u;
			connectDevice( device );
		}
		else
		{
			TIKI_TRACE_DEBUG( "[input] No touch device present.\n" );
		}

		return true;
	}

	void InputSystem::dispose()
	{
		if ( m_platformData.pStates[ 0u ] != nullptr )
		{
			TIKI_FREE( m_platformData.pStates[ 0u ] );
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

		MainWindowUserData* pUserData = (MainWindowUserData*)GetWindowLongPtr( (HWND)m_platformData.windowHandle, GWLP_USERDATA );
		if( pUserData != nullptr )
		{
			pUserData->apUserData[ MainWindowMesageHandler_InputSystem ]		= nullptr;
			pUserData->apHandlerFunction[ MainWindowMesageHandler_InputSystem ]	= nullptr;
		}
	}

	void InputSystem::update()
	{
		const InputSystemState* pPreviousState = m_platformData.pStates[ m_platformData.currentStateIndex ];

		m_platformData.currentStateIndex = 1u - m_platformData.currentStateIndex;
		InputSystemState* pCurrentState = m_platformData.pStates[ m_platformData.currentStateIndex ];

		HRESULT result = DIERR_UNSUPPORTED;		
		if ( m_platformData.pKeyboard != nullptr )
		{
			result = m_platformData.pKeyboard->GetDeviceState( sizeof( pCurrentState->aKeyboard ), &pCurrentState->aKeyboard );
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
				for (uint i = 0u; i < TIKI_COUNT( pCurrentState->aKeyboard ); ++i)
				{
					const KeyboardKey mappedKey = static_cast< KeyboardKey >( m_platformData.keyboardMapping[ i ] );
					if ( mappedKey >= KeyboardKey_Count )
					{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
						const bool isPressed = isBitSet( pCurrentState->aKeyboard[ i ], 0x80u );
						const bool wasPressed = isBitSet( pPreviousState->aKeyboard[ i ], 0x80u );
						if ( isPressed != wasPressed )
						{
							TIKI_TRACE_INFO( "[input] input received from unmapped key with code: %u\n", i );
						}
#endif

						continue;
					}

					const bool isPressed = isBitSet( pCurrentState->aKeyboard[ i ], 0x80u );
					const bool wasPressed = isBitSet( pPreviousState->aKeyboard[ i ], 0x80u );

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
				for (uint i = 0u; i < MouseButton_Count; ++i)
				{
					const bool isPressed = ( pCurrentState->mouse.rgbButtons[ i ] != 0u );
					const bool wasPressed = ( pPreviousState->mouse.rgbButtons[ i ] != 0u );

					if ( isPressed && !wasPressed )
					{
						InputEvent& inputEvent = m_events.push();
						inputEvent.eventType	= InputEventType_Mouse_ButtonDown;
						inputEvent.deviceType	= InputDeviceType_Mouse;
						inputEvent.deviceId		= 0u;
						inputEvent.data.mouseButton.button = s_aMouseButtonMapping[ i ];
					}
					else if ( !isPressed && wasPressed )
					{
						InputEvent& inputEvent = m_events.push();
						inputEvent.eventType	= InputEventType_Mouse_ButtonUp;
						inputEvent.deviceType	= InputDeviceType_Mouse;
						inputEvent.deviceId		= 0u;
						inputEvent.data.mouseButton.button = s_aMouseButtonMapping[ i ];
					}
				} 
				
				if ( pCurrentState->mouse.lX != 0u || pCurrentState->mouse.lY != 0u )
				{
					InputEvent& inputEvent = m_events.push();
					inputEvent.eventType	= InputEventType_Mouse_Moved;
					inputEvent.deviceType	= InputDeviceType_Mouse;
					inputEvent.deviceId		= 0u;
					inputEvent.data.mouseMoved.xOffset = (sint16)pCurrentState->mouse.lX;
					inputEvent.data.mouseMoved.yOffset = (sint16)pCurrentState->mouse.lY;

					POINT mousePosition;
					if ( GetCursorPos( &mousePosition) && ScreenToClient( (HWND)m_platformData.windowHandle, &mousePosition ) )
					{
						inputEvent.data.mouseMoved.xState = (sint16)mousePosition.x;
						inputEvent.data.mouseMoved.yState = (sint16)mousePosition.y;
					}
					else
					{
						inputEvent.data.mouseMoved.xState = 0;
						inputEvent.data.mouseMoved.yState = 0;
					}
				}

				if ( pCurrentState->mouse.lZ != 0 )
				{
					InputEvent& inputEvent = m_events.push();
					inputEvent.eventType	= InputEventType_Mouse_Wheel;
					inputEvent.deviceType	= InputDeviceType_Mouse;
					inputEvent.deviceId		= 0u;
					inputEvent.data.mouseWheel.offset = (sint16)pCurrentState->mouse.lZ;
				}
			}
		}

		// controller
		for (uint controllerIndex = 0u; controllerIndex < XUSER_MAX_COUNT; ++controllerIndex)
		{
			if ( XInputGetState( DWORD( controllerIndex ), &pCurrentState->aController[ controllerIndex ] ) == ERROR_SUCCESS )
			{
				if ( !pCurrentState->aControllerConnected[ controllerIndex ] )
				{
					InputDevice device;
					device.deviceType	= InputDeviceType_Controller;
					device.deviceId		= controllerIndex;
					connectDevice( device );

					pCurrentState->aControllerConnected[ controllerIndex ] = true;
					continue;
				}

				const XINPUT_GAMEPAD& currentState = pCurrentState->aController[ controllerIndex ].Gamepad;
				const XINPUT_GAMEPAD& previousState = pPreviousState->aController[ controllerIndex ].Gamepad;

				input::checkControllerTrigger( m_events, controllerIndex, 0u, currentState.bLeftTrigger, previousState.bLeftTrigger );
				input::checkControllerTrigger( m_events, controllerIndex, 1u, currentState.bRightTrigger, previousState.bRightTrigger );

				input::checkControllerStick( m_events, controllerIndex, 0u, currentState.sThumbLX, previousState.sThumbLX, currentState.sThumbLY, previousState.sThumbLY );
				input::checkControllerStick( m_events, controllerIndex, 1u, currentState.sThumbRX, previousState.sThumbRX, currentState.sThumbRY, previousState.sThumbRY );

				const uint16 currentButtons = currentState.wButtons;
				const uint16 previousButtons = previousState.wButtons;
				for (uint i = 0u; i < TIKI_COUNT( s_aControllerButtonMapping ); ++i)
				{
					input::checkControllerButton( m_events, controllerIndex, (ControllerButton)i, s_aControllerButtonMapping[ i ], currentButtons, previousButtons );
				}
			}
			else if ( pCurrentState->aControllerConnected[ controllerIndex ] )
			{
				InputDevice device;
				device.deviceType	= InputDeviceType_Controller;
				device.deviceId		= controllerIndex;
				disconnectDevice( device );

				pCurrentState->aControllerConnected[ controllerIndex ] = false;
			}
		}

		// touch

	}

	void InputSystem::endFrame()
	{
		m_events.clear();
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

	/*static*/ bool InputSystemPlatform::handleWindowMessage( InputSystem& inputSystem, UINT message, WPARAM wParam, LPARAM lParam )
	{
		if( message == WM_TOUCH && inputSystem.m_platformData.isTouchInputReady )
		{
			InputSystemState* pCurrentState = inputSystem.m_platformData.pStates[ inputSystem.m_platformData.currentStateIndex ];

			const HTOUCHINPUT touchHandle = (HTOUCHINPUT)lParam;
			const uint touchInputCount = TIKI_MIN( TIKI_COUNT( pCurrentState->aTouchPoints ), LOWORD( wParam ) );
			if( GetTouchInputInfo( touchHandle, (UINT)touchInputCount, pCurrentState->aTouchPoints, sizeof( TOUCHINPUT ) ) )
			{
				for( uint touchIndex = 0u; touchIndex < touchInputCount; touchIndex++ )
				{
					const TOUCHINPUT& touchInput = pCurrentState->aTouchPoints[ touchIndex ];

					const TouchInputState state = input::getTouchInputMapping( touchInput.dwID, inputSystem.m_platformData.touchInputMapping, TIKI_COUNT( inputSystem.m_platformData.touchInputMapping ) );
					if( !state.isValid )
					{
						continue;
					}

					if( state.isNewPoint )
					{
						TIKI_ASSERT( isBitSet( touchInput.dwFlags, TOUCHEVENTF_DOWN ) );
						inputSystem.m_platformData.touchInputMapping[ state.index ] = touchInput.dwID;
					}

					POINT touchPoint;
					touchPoint.x = TOUCH_COORD_TO_PIXEL( touchInput.x );
					touchPoint.y = TOUCH_COORD_TO_PIXEL( touchInput.y );
					ScreenToClient( (HWND)inputSystem.m_platformData.windowHandle, &touchPoint );

					InputEventType eventType = InputEventType_Touch_PointMove;
					if( isBitSet( touchInput.dwFlags, TOUCHEVENTF_DOWN ) )
					{
						eventType = InputEventType_Touch_PointDown;
					}
					else if( isBitSet( touchInput.dwFlags, TOUCHEVENTF_UP ) )
					{
						eventType = InputEventType_Touch_PointUp;

						TIKI_ASSERT( inputSystem.m_platformData.touchInputMapping[ state.index ] == touchInput.dwID );
						inputSystem.m_platformData.touchInputMapping[ state.index ] = InvalidTouchInputMapping;
					}

					InputEvent& inputEvent = inputSystem.m_events.push();
					inputEvent.eventType	= eventType;
					inputEvent.deviceType	= InputDeviceType_Touch;
					inputEvent.deviceId		= 0u;
					inputEvent.data.touch.pointIndex	= (uint16)state.index;
					inputEvent.data.touch.xState		= uint16( touchPoint.x );
					inputEvent.data.touch.yState		= uint16( touchPoint.y );
				}
			}
			else
			{
				TIKI_TRACE_ERROR( "[input] Could not get touch points from Handle. Touch Input Events will not generated.\n" );
			}

			CloseTouchInputHandle( touchHandle );

			return true;
		}

		//case WM_KEYDOWN:
		//	{
		//		WindowEvent& event	= s_pEventBuffer->pushEvent( WindowEventType_KeyDown );
		//		event.data.key.key	= (KeyboardKey)MapVirtualKey( UINT( wParam ), 0 );
		//	}
		//	break;

		//case WM_KEYUP:
		//	{
		//		WindowEvent& event	= s_pEventBuffer->pushEvent( WindowEventType_KeyUp );
		//		event.data.key.key	= (KeyboardKey)MapVirtualKey( UINT( wParam ), 0 );
		//	}
		//	break;

		//case WM_MOUSEMOVE:
		//	{
		//		WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_MouseMove );
		//		event.data.mouseMove.position.x	= GET_X_LPARAM( lParam );
		//		event.data.mouseMove.position.y	= GET_Y_LPARAM( lParam );
		//	}
		//	break;

		//case WM_LBUTTONDOWN:
		//	{
		//		WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_MouseDown );
		//		event.data.mouseButton.button	= MouseButton_Left;
		//	}
		//	break;

		//case WM_MBUTTONDOWN:
		//	{
		//		WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_MouseDown );
		//		event.data.mouseButton.button	= MouseButton_Middle;
		//	}
		//	break;

		//case WM_RBUTTONDOWN:
		//	{
		//		WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_MouseDown );
		//		event.data.mouseButton.button	= MouseButton_Right;
		//	}
		//	break;

		//case WM_LBUTTONUP:
		//	{
		//		WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_MouseUp );
		//		event.data.mouseButton.button	= MouseButton_Left;
		//	}
		//	break;

		//case WM_MBUTTONUP:
		//	{
		//		WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_MouseUp );
		//		event.data.mouseButton.button	= MouseButton_Middle;
		//	}
		//	break;

		//case WM_RBUTTONUP:
		//	{
		//		WindowEvent& event				= s_pEventBuffer->pushEvent( WindowEventType_MouseUp );
		//		event.data.mouseButton.button	= MouseButton_Right;
		//	}
		//	break;

		return false;
	}
}