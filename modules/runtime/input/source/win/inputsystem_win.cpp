
#include "tiki/input/inputsystem.hpp"

#include "tiki/base/platform.hpp"
#include "tiki/framework/mainwindow.hpp"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <xinput.h>
#include <Windows.h>

namespace tiki
{
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
		if ( m_platformData.pInputDevice == nullptr )
		{
			return;
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
		m_platformData.currentStateIndex = 1u - m_platformData.currentStateIndex;
		InputSystemState* pCurrentState = m_platformData.pStates[ m_platformData.currentStateIndex ];
		//InputSystemState* pCurrentState = m_platformData.pStates[ m_platformData.currentStateIndex ];

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
			}
			else
			{
				// check for changes
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
				pCurrentState->controllerConnected[ controllerIndex ] = true;

				
			}
			else if ( false /*is connected*/)
			{
				InputDevice device;
				device.deviceType	= InputDeviceType_Controller;
				device.deviceId		= controllerIndex;
				disconnectDevice( device );
			}
		}		
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