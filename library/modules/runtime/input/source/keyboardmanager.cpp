
#include "tiki/input/keyboardmanager.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/graphics/functions.hpp"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace tiki
{
	KeyboardManager::KeyboardManager( void )
	{

	}

	KeyboardManager::~KeyboardManager( void )
	{

	}

	bool KeyboardManager::create( const KeyboardManagerParameter& params )
	{
		HRESULT result;

		result = params.m_pInputDevice->CreateDevice( GUID_SysKeyboard, &m_pKeyboard, nullptr );
		if( FAILED( result ) )
			return false;

		result = m_pKeyboard->SetDataFormat( &c_dfDIKeyboard );
		if( FAILED( result ) )
			return false;

		result = m_pKeyboard->SetCooperativeLevel( (HWND)params.m_phWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE );
		if( FAILED( result ) )
			return false;

		memory::zero( &m_currState, 256 );
		memory::zero( &m_prevState, 256 );

		return true;
	}

	void KeyboardManager::dispose( void )
	{
		m_pKeyboard->Unacquire();
		safeRelease( &m_pKeyboard );
	}

	void KeyboardManager::frame( void )
	{
		memcpy( &m_prevState, &m_currState, 256 );

		HRESULT result = m_pKeyboard->GetDeviceState( 256, (void*)&m_currState );
		if( FAILED( result ) )
		{
			if( result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED )
			{
				m_pKeyboard->Acquire();
			}
		}
	}

	bool KeyboardManager::isKeyDown( const Keys key ) const
	{
		return ( m_currState[ key ] & 0x80 ) != 0;
	}

	bool KeyboardManager::isKeyUp( const Keys key ) const
	{
		return ( m_currState[ key ] & 0x80 ) == 0;
	}

	bool KeyboardManager::isKeyPressed( const Keys key ) const
	{
		return (!( m_prevState[ key ] & 0x80 )  && 
			( m_currState[ key ] & 0x80 ) );
	}

	bool KeyboardManager::isKeyReleased( const Keys key ) const
	{
		return (( m_prevState[ key ] & 0x80 ) && 
			!( m_currState[ key ] & 0x80 ) );
	}

}