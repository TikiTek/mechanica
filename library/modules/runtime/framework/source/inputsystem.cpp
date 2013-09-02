
#include "tiki/framework/inputsystem.hpp"

#include "tiki/base/platform.hpp"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>

namespace tiki
{
	InputSystem::InputSystem()
	{
		m_pInputDevice	= nullptr;
		m_pMouse		= nullptr;
		m_pKeyboard		= nullptr;

		TIKI_COMPILETIME_ASSERT( sizeof( m_currentState.mouseState ) == sizeof( DIMOUSESTATE ) );
	}

	InputSystem::~InputSystem()
	{
		TIKI_ASSERT( m_pInputDevice == nullptr );
		TIKI_ASSERT( m_pMouse == nullptr );
		TIKI_ASSERT( m_pKeyboard == nullptr );
	}

	bool InputSystem::create( const InputSystemParameters& params )
	{
		HINSTANCE hinst = GetModuleHandle( nullptr );

		HRESULT result = DirectInput8Create( hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputDevice, nullptr );
		if( FAILED( result ) || m_pInputDevice == nullptr )
		{
			return false;
		}

		// keyboard
		result = m_pInputDevice->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, nullptr );
		if( FAILED( result ) || m_pKeyboard == nullptr )
		{
			return false;
		}

		result = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard );
		if( FAILED( result ) )
		{
			return false;
		}

		result = m_pKeyboard->SetCooperativeLevel( (HWND)params.pWindow->getHandle(), DISCL_FOREGROUND | DISCL_EXCLUSIVE );
		if( FAILED( result ) )
			return false;

		// mouse
		result = m_pInputDevice->CreateDevice( GUID_SysMouse, &m_pMouse, nullptr );
		if( FAILED( result ) )
		{
			return false;
		}

		result = m_pMouse->SetDataFormat( &c_dfDIMouse );
		if( FAILED( result ) )
		{
			return false;
		}

		result = m_pMouse->SetCooperativeLevel( (HWND)params.pWindow->getHandle(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
		if( FAILED( result ) )
		{
			return false;		
		}

		return true;
	}

	void InputSystem::dispose( void )
	{
		if ( m_pInputDevice == nullptr )
		{
			return;
		}

		m_pKeyboard->Unacquire();
		if ( m_pKeyboard != nullptr )
		{
			m_pKeyboard->Release();
			m_pKeyboard = nullptr;
		}

		m_pMouse->Unacquire();
		if ( m_pMouse != nullptr )
		{
			m_pMouse->Release();
			m_pMouse = nullptr;
		}

		if ( m_pInputDevice != nullptr )
		{
			m_pInputDevice->Release();
			m_pInputDevice = nullptr;
		}
	}

	void InputSystem::update( const WindowEventBuffer& windowEvents )
	{
		m_previousState = m_currentState;

		HRESULT result = m_pKeyboard->GetDeviceState( 256, (void*)&m_currentState.keyboardState );
		if( FAILED( result ) )
		{
			if( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED )
			{
				m_pKeyboard->Acquire();
			}
		}

		result = m_pMouse->GetDeviceState( sizeof( DIMOUSESTATE ), (void*)m_currentState.mouseState );
		if( FAILED( result ) )
		{
			if( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED )
			{
				m_pMouse->Acquire();
			}
		}
	}

	bool InputSystem::isButtonDown( const MouseButtons button ) const
	{
		const DIMOUSESTATE* pState = reinterpret_cast< const DIMOUSESTATE* >( m_currentState.mouseState );
		return pState->rgbButtons[ button ] != 0;
	}

	bool InputSystem::isButtonUp( const MouseButtons button ) const
	{
		const DIMOUSESTATE* pState = reinterpret_cast< const DIMOUSESTATE* >( m_currentState.mouseState );
		return pState->rgbButtons[ button ] == 0;
	}

	bool InputSystem::isButtonPressed( const MouseButtons button ) const
	{
		const DIMOUSESTATE* pPreviousState = reinterpret_cast< const DIMOUSESTATE* >( m_previousState.mouseState );
		const DIMOUSESTATE* pCurrentState = reinterpret_cast< const DIMOUSESTATE* >( m_currentState.mouseState );

		return ( pPreviousState->rgbButtons[ button ] == 0 ) && ( pCurrentState->rgbButtons[ button ] != 0 );
	}

	bool InputSystem::isButtonReleased( const MouseButtons button ) const
	{
		const DIMOUSESTATE* pPreviousState = reinterpret_cast< const DIMOUSESTATE* >( m_previousState.mouseState );
		const DIMOUSESTATE* pCurrentState = reinterpret_cast< const DIMOUSESTATE* >( m_currentState.mouseState );

		return ( pPreviousState->rgbButtons[ button ] != 0 ) && ( pCurrentState->rgbButtons[ button ] == 0 );
	}

	//const Vector2& InputSystem::getMousePosition( void ) const
	//{
	//	return m_position;
	//}

	//const Vector2& InputSystem::getMousePositionDisplay( void ) const
	//{
	//	return m_positionDisplay;
	//}

	//const Vector2& InputSystem::getDistance( void ) const
	//{
	//	return m_distance;
	//}

	bool InputSystem::isKeyDown( Keys key ) const
	{
		return ( m_currentState.keyboardState[ key ] & 0x80 ) != 0;
	}

	bool InputSystem::isKeyUp( Keys key ) const
	{
		return ( m_currentState.keyboardState[ key ] & 0x80 ) == 0;
	}

	bool InputSystem::hasKeyPressed( Keys key ) const
	{
		return ( !( m_previousState.keyboardState[ key ] & 0x80 ) && ( m_currentState.keyboardState[ key ] & 0x80 ) );
	}

	bool InputSystem::hasKeyReleased( Keys key ) const
	{
		return (( m_previousState.keyboardState[ key ] & 0x80 ) && !( m_currentState.keyboardState[ key ] & 0x80 ) );
	}
}