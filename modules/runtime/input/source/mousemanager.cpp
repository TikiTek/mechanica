
#include "tiki/input/mousemanager.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/graphics/functions.hpp"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace tiki
{
	MouseManager::MouseManager()
	{

	}

	MouseManager::~MouseManager()
	{

	}

	bool MouseManager::create( const MouseManagerParameter& params )
	{
		HRESULT result;

		result = params.m_pInputDevice->CreateDevice( GUID_SysMouse, &m_pMouse, nullptr );
		if( FAILED( result ) )
			return false;

		result = m_pMouse->SetDataFormat( &c_dfDIMouse );
		if( FAILED( result ) )
			return false;

		result = m_pMouse->SetCooperativeLevel( (HWND)params.m_phWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE );
		if( FAILED( result ) )
			return false;

		m_phwnd = (HWND)params.m_phWnd;

		m_viewport = Vector2( (float)params.m_screenwidth, (float)params.m_screenheight );

		memory::zero( &m_currState, 3 );
		memory::zero( &m_prevState, 3 );

		m_position			= Vector2::zero;
		m_distance			= Vector2::zero;
		m_positionDisplay	= Vector2::zero;
		m_wheel				= 0.0f;

		TIKI_ASSERT( sizeof( m_inputStateBuffer ) == sizeof( DIMOUSESTATE ) );
		m_pInputState		= (DIMOUSESTATE*)m_inputStateBuffer;

		return true;
	}

	void MouseManager::dispose( void )
	{
		m_pMouse->Unacquire();
		safeRelease( &m_pMouse );
	}

	void MouseManager::frame( void )
	{
		HRESULT result = m_pMouse->GetDeviceState( sizeof( DIMOUSESTATE ), m_pInputState );
		if( FAILED( result ) )
		{
			if( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED )
			{
				m_pMouse->Acquire();
			}
		}

		POINT p;
		if( GetCursorPos( &p ) && ScreenToClient( (HWND)m_phwnd, &p ) )
		{
			m_positionDisplay = Vector2( (float)p.x, (float)p.y );
			m_positionDisplay.clamp( Vector2::zero, m_viewport );

			m_position = m_positionDisplay;
			m_position.div( m_viewport );
		}

		m_distance = Vector2((float)m_pInputState->lX, (float)m_pInputState->lY );
		m_distance.div( m_viewport );

		m_wheel = (float)m_pInputState->lZ;
		memcpy( &m_prevState, &m_currState, 3 );
		memcpy( &m_currState, m_pInputState->rgbButtons, 3 );
	}

	bool MouseManager::isButtonDown( const MouseButtons button ) const
	{
		return m_currState[ button ] != 0;
	}

	bool MouseManager::isButtonUp( const MouseButtons button ) const
	{
		return m_currState[ button ] == 0;
	}

	bool MouseManager::isButtonPressed( const MouseButtons button ) const
	{
		return ( m_prevState[ button ] == 0 ) && ( m_currState[ button ] != 0 );
	}

	bool MouseManager::isButtonReleased( const MouseButtons button ) const
	{
		return ( m_prevState[ button ] != 0 ) && ( m_currState[ button ] == 0 );
	}

	const Vector2& MouseManager::getMousePosition( void ) const
	{
		return m_position;
	}

	const Vector2& MouseManager::getMousePositionDisplay( void ) const
	{
		return m_positionDisplay;
	}

	const Vector2& MouseManager::getDistance( void ) const
	{
		return m_distance;
	}

}