
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

	//TIKI_FORCE_INLINE static void checkControllerTrigger( InputSystem::InputEventArray& events, uint controllerIndex, uint32 triggerIndex, uint8 currentState, uint8 previousState )
	//{
	//	if ( currentState != previousState )
	//	{
	//		if ( currentState < XINPUT_GAMEPAD_TRIGGER_THRESHOLD )
	//		{
	//			currentState = 0u;
	//		}

	//		InputEvent& inputEvent = events.push();
	//		inputEvent.eventType	= InputEventType_Controller_TriggerChanged;
	//		inputEvent.deviceType	= InputDeviceType_Controller;
	//		inputEvent.deviceId		= controllerIndex;
	//		inputEvent.data.controllerTrigger.triggerIndex	= triggerIndex;
	//		inputEvent.data.controllerTrigger.state			= (float)currentState / 255.0f;
	//	}
	//}

	//TIKI_FORCE_INLINE static void checkControllerStick( InputSystem::InputEventArray& events, uint controllerIndex, uint32 stickIndex, sint16 currentStateX, sint16 previousStateX, sint16 currentStateY, sint16 previousStateY )
	//{
	//	if ( currentStateX != previousStateX || currentStateY != previousStateY )
	//	{
	//		if ( abs( currentStateX ) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
	//		{
	//			currentStateX = 0u;
	//		}

	//		if ( abs( currentStateY ) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
	//		{
	//			currentStateY = 0u;
	//		}

	//		InputEvent& inputEvent = events.push();
	//		inputEvent.eventType	= InputEventType_Controller_StickChanged;
	//		inputEvent.deviceType	= InputDeviceType_Controller;
	//		inputEvent.deviceId		= controllerIndex;
	//		inputEvent.data.controllerStick.stickIndex		= stickIndex;
	//		inputEvent.data.controllerStick.xState			= (float)currentStateX / 32767.0f;
	//		inputEvent.data.controllerStick.yState			= (float)currentStateY / 32767.0f;
	//	}
	//}

	//TIKI_FORCE_INLINE static void checkControllerButton( InputSystem::InputEventArray& events, uint controllerIndex, ControllerButton button, uint16 nativeButton, sint16 currentState, sint16 previousState )
	//{
	//	const bool isPressed	= isBitSet( currentState, nativeButton );
	//	const bool wasPressed	= isBitSet( previousState, nativeButton );

	//	if ( isPressed && !wasPressed )
	//	{
	//		InputEvent& inputEvent = events.push();
	//		inputEvent.eventType	= InputEventType_Controller_ButtonDown;
	//		inputEvent.deviceType	= InputDeviceType_Controller;
	//		inputEvent.deviceId		= controllerIndex;
	//		inputEvent.data.controllerButton.button = button;
	//	}
	//	else if ( !isPressed && wasPressed )
	//	{
	//		InputEvent& inputEvent = events.push();
	//		inputEvent.eventType	= InputEventType_Controller_ButtonUp;
	//		inputEvent.deviceType	= InputDeviceType_Controller;
	//		inputEvent.deviceId		= controllerIndex;
	//		inputEvent.data.controllerButton.button = button;
	//	}
	//}

	//TIKI_FORCE_INLINE static TouchInputState getTouchInputMapping( uint32 touchId, uint32* pTouchMapping, uint touchMappingCapacity )
	//{
	//	uint firstFreeIndex = TIKI_SIZE_T_MAX;
	//	for (uint i = 0u; i < touchMappingCapacity; ++i)
	//	{
	//		if ( pTouchMapping[ i ] == touchId )
	//		{
	//			TouchInputState state;
	//			state.isValid		= true;
	//			state.isNewPoint	= false;
	//			state.index			= i;

	//			return state;
	//		}
	//		else if ( pTouchMapping[ i ] == InvalidTouchInputMapping )
	//		{
	//			firstFreeIndex = i;
	//		}
	//	}

	//	if ( firstFreeIndex != TIKI_SIZE_T_MAX )
	//	{
	//		TouchInputState state;
	//		state.isValid		= true;
	//		state.isNewPoint	= true;
	//		state.index			= firstFreeIndex;
	//		
	//		return state;
	//	}

	//	TouchInputState state;
	//	state.isValid		= false;
	//	state.isNewPoint	= false;
	//	state.index			= TIKI_SIZE_T_MAX;

	//	return state;
	//}

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

//		const InputSystemState* pPreviousState = m_platformData.pStates[ m_platformData.currentStateIndex ];
//
//		m_platformData.currentStateIndex = 1u - m_platformData.currentStateIndex;
//		InputSystemState* pCurrentState = m_platformData.pStates[ m_platformData.currentStateIndex ];
//
//		HRESULT result = DIERR_UNSUPPORTED;		
//		if ( m_platformData.pKeyboard != nullptr )
//		{
//			result = m_platformData.pKeyboard->GetDeviceState( sizeof( pCurrentState->aKeyboard ), &pCurrentState->aKeyboard );
//			if( FAILED( result ) )
//			{
//				if( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED )
//				{
//					m_platformData.pKeyboard->Acquire();
//				}
//				else
//				{
//					TIKI_TRACE_ERROR( "[input] Get keyboard state has returned an error. Code: 0x%08x\n", result );
//				}
//			}
//			else
//			{
//				for (uint i = 0u; i < TIKI_COUNT( pCurrentState->aKeyboard ); ++i)
//				{
//					const KeyboardKey mappedKey = static_cast< KeyboardKey >( m_platformData.keyboardMapping[ i ] );
//					if ( mappedKey >= KeyboardKey_Count )
//					{
//#if TIKI_DISABLED( TIKI_BUILD_MASTER )
//						const bool isPressed = isBitSet( pCurrentState->aKeyboard[ i ], 0x80u );
//						const bool wasPressed = isBitSet( pPreviousState->aKeyboard[ i ], 0x80u );
//						if ( isPressed != wasPressed )
//						{
//							TIKI_TRACE_INFO( "[input] input received from unmapped key with code: %u\n", i );
//						}
//#endif
//
//						continue;
//					}
//
//					const bool isPressed = isBitSet( pCurrentState->aKeyboard[ i ], 0x80u );
//					const bool wasPressed = isBitSet( pPreviousState->aKeyboard[ i ], 0x80u );
//
//					if ( isPressed && !wasPressed )
//					{
//						InputEvent& inputEvent = m_events.push();
//						inputEvent.eventType	= InputEventType_Keyboard_Down;
//						inputEvent.deviceType	= InputDeviceType_Keyboard;
//						inputEvent.deviceId		= 0u;
//						inputEvent.data.keybaordKey.key = mappedKey;
//					}
//					else if ( !isPressed && wasPressed )
//					{
//						InputEvent& inputEvent = m_events.push();
//						inputEvent.eventType	= InputEventType_Keyboard_Up;
//						inputEvent.deviceType	= InputDeviceType_Keyboard;
//						inputEvent.deviceId		= 0u;
//						inputEvent.data.keybaordKey.key = mappedKey;
//					}
//				} 
//			}
//		}
//
//		if ( m_platformData.pMouse != nullptr )
//		{
//			result = m_platformData.pMouse->GetDeviceState( sizeof( pCurrentState->mouse ), &pCurrentState->mouse );
//			if( FAILED( result ) )
//			{
//				if( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED )
//				{
//					m_platformData.pMouse->Acquire();
//				}
//				else
//				{
//					TIKI_TRACE_ERROR( "[input] Get mouse state has returned an error. Code: 0x%08x\n", result );
//				}
//			}
//			else
//			{
//				for (uint i = 0u; i < MouseButton_Count; ++i)
//				{
//					const bool isPressed = ( pCurrentState->mouse.rgbButtons[ i ] != 0u );
//					const bool wasPressed = ( pPreviousState->mouse.rgbButtons[ i ] != 0u );
//
//					if ( isPressed && !wasPressed )
//					{
//						InputEvent& inputEvent = m_events.push();
//						inputEvent.eventType	= InputEventType_Mouse_ButtonDown;
//						inputEvent.deviceType	= InputDeviceType_Mouse;
//						inputEvent.deviceId		= 0u;
//						inputEvent.data.mouseButton.button = s_aMouseButtonMapping[ i ];
//					}
//					else if ( !isPressed && wasPressed )
//					{
//						InputEvent& inputEvent = m_events.push();
//						inputEvent.eventType	= InputEventType_Mouse_ButtonUp;
//						inputEvent.deviceType	= InputDeviceType_Mouse;
//						inputEvent.deviceId		= 0u;
//						inputEvent.data.mouseButton.button = s_aMouseButtonMapping[ i ];
//					}
//				} 
//				
//				if ( pCurrentState->mouse.lX != 0u || pCurrentState->mouse.lY != 0u )
//				{
//					InputEvent& inputEvent = m_events.push();
//					inputEvent.eventType	= InputEventType_Mouse_Moved;
//					inputEvent.deviceType	= InputDeviceType_Mouse;
//					inputEvent.deviceId		= 0u;
//					inputEvent.data.mouseMoved.xOffset = (sint16)pCurrentState->mouse.lX;
//					inputEvent.data.mouseMoved.yOffset = (sint16)pCurrentState->mouse.lY;
//
//					POINT mousePosition;
//					if ( GetCursorPos( &mousePosition) && ScreenToClient( (HWND)m_platformData.windowHandle, &mousePosition ) )
//					{
//						inputEvent.data.mouseMoved.xState = (sint16)mousePosition.x;
//						inputEvent.data.mouseMoved.yState = (sint16)mousePosition.y;
//					}
//					else
//					{
//						inputEvent.data.mouseMoved.xState = 0;
//						inputEvent.data.mouseMoved.yState = 0;
//					}
//				}
//
//				if ( pCurrentState->mouse.lZ != pPreviousState->mouse.lZ )
//				{
//					InputEvent& inputEvent = m_events.push();
//					inputEvent.eventType	= InputEventType_Mouse_Wheel;
//					inputEvent.deviceType	= InputDeviceType_Mouse;
//					inputEvent.deviceId		= 0u;
//					inputEvent.data.mouseWheel.offset = ( pPreviousState->mouse.lZ - pCurrentState->mouse.lZ );
//				}
//			}
//		}
//
//		// controller
//		for (uint controllerIndex = 0u; controllerIndex < XUSER_MAX_COUNT; ++controllerIndex)
//		{
//			if ( XInputGetState( DWORD( controllerIndex ), &pCurrentState->aController[ controllerIndex ] ) == ERROR_SUCCESS )
//			{
//				if ( !pCurrentState->aControllerConnected[ controllerIndex ] )
//				{
//					InputDevice device;
//					device.deviceType	= InputDeviceType_Controller;
//					device.deviceId		= controllerIndex;
//					connectDevice( device );
//
//					pCurrentState->aControllerConnected[ controllerIndex ] = true;
//					continue;
//				}
//
//				const XINPUT_GAMEPAD& currentState = pCurrentState->aController[ controllerIndex ].Gamepad;
//				const XINPUT_GAMEPAD& previousState = pPreviousState->aController[ controllerIndex ].Gamepad;
//
//				checkControllerTrigger( m_events, controllerIndex, 0u, currentState.bLeftTrigger, previousState.bLeftTrigger );
//				checkControllerTrigger( m_events, controllerIndex, 1u, currentState.bRightTrigger, previousState.bRightTrigger );
//
//				checkControllerStick( m_events, controllerIndex, 0u, currentState.sThumbLX, previousState.sThumbLX, currentState.sThumbLY, previousState.sThumbLY );
//				checkControllerStick( m_events, controllerIndex, 1u, currentState.sThumbRX, previousState.sThumbRX, currentState.sThumbRY, previousState.sThumbRY );
//
//				const uint16 currentButtons = currentState.wButtons;
//				const uint16 previousButtons = previousState.wButtons;
//				for (uint i = 0u; i < TIKI_COUNT( s_aControllerButtonMapping ); ++i)
//				{
//					checkControllerButton( m_events, controllerIndex, (ControllerButton)i, s_aControllerButtonMapping[ i ], currentButtons, previousButtons );
//				}
//			}
//			else if ( pCurrentState->aControllerConnected[ controllerIndex ] )
//			{
//				InputDevice device;
//				device.deviceType	= InputDeviceType_Controller;
//				device.deviceId		= controllerIndex;
//				disconnectDevice( device );
//
//				pCurrentState->aControllerConnected[ controllerIndex ] = false;
//			}
//		}
//
//		// touch
//		if ( m_platformData.isTouchInputReady )
//		{
//			const uint eventCount = windowEvents.getEventCount();
//			for (uint eventIndex = 0u; eventIndex < eventCount; ++eventIndex)
//			{
//				const WindowEvent& windowEvent = windowEvents.getEventByIndex(eventIndex );
//
//				if ( windowEvent.type != WindowEventType_Touch )
//				{
//					continue;
//				}
//
//				const HTOUCHINPUT touchHandle = (HTOUCHINPUT)windowEvent.data.touch.handle;
//				const uint touchInputCount = TIKI_MIN( TIKI_COUNT( pCurrentState->aTouchPoints ), windowEvent.data.touch.pointCount );
//				if ( GetTouchInputInfo( touchHandle, (UINT)touchInputCount, pCurrentState->aTouchPoints, sizeof( TOUCHINPUT ) ) )
//				{
//					for (uint touchIndex = 0u; touchIndex < touchInputCount; touchIndex++)
//					{
//						const TOUCHINPUT& touchInput = pCurrentState->aTouchPoints[touchIndex];
//						
//						const TouchInputState state = getTouchInputMapping( touchInput.dwID, m_platformData.touchInputMapping, TIKI_COUNT( m_platformData.touchInputMapping ) );
//						if ( !state.isValid )
//						{
//							continue;
//						}
//
//						if ( state.isNewPoint )
//						{
//							TIKI_ASSERT( isBitSet( touchInput.dwFlags, TOUCHEVENTF_DOWN ) );
//							m_platformData.touchInputMapping[ state.index ] = touchInput.dwID;
//						}
//
//						POINT touchPoint;
//						touchPoint.x = TOUCH_COORD_TO_PIXEL( touchInput.x );
//						touchPoint.y = TOUCH_COORD_TO_PIXEL( touchInput.y );
//						ScreenToClient( (HWND)m_platformData.windowHandle, &touchPoint );
//
//						InputEventType eventType = InputEventType_Touch_PointMove;
//						if ( isBitSet( touchInput.dwFlags, TOUCHEVENTF_DOWN ) )
//						{
//							eventType = InputEventType_Touch_PointDown;
//						}
//						else if ( isBitSet( touchInput.dwFlags, TOUCHEVENTF_UP ) )
//						{
//							eventType = InputEventType_Touch_PointUp;
//
//							TIKI_ASSERT( m_platformData.touchInputMapping[ state.index ] == touchInput.dwID );
//							m_platformData.touchInputMapping[ state.index ] = InvalidTouchInputMapping;
//						}
//
//						InputEvent& inputEvent = m_events.push();
//						inputEvent.eventType	= eventType;
//						inputEvent.deviceType	= InputDeviceType_Touch;
//						inputEvent.deviceId		= 0u;
//						inputEvent.data.touch.pointIndex	= (uint16)state.index;
//						inputEvent.data.touch.xState		= uint16( touchPoint.x );
//						inputEvent.data.touch.yState		= uint16( touchPoint.y );
//					}
//				}
//				else
//				{
//					TIKI_TRACE_ERROR( "[input] Could not get touch points from Handle. Touch Input Events will not generated.\n" );
//				}
//
//				CloseTouchInputHandle( touchHandle );
//			} 
//		}
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