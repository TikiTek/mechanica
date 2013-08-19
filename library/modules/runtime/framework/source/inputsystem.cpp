
#include "tiki/input/inputsystem.hpp"

#include "tiki/graphics/functions.hpp"
#include "tiki/framework/framework.hpp"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Windows.h>

namespace tiki
{
	InputSystem::InputSystem()
		: m_pInputDevice( nullptr )
	{

	}

	InputSystem::~InputSystem()
	{

	}

	bool InputSystem::create( const InputSystemParameters& params )
	{
		HINSTANCE hinst = GetModuleHandle( nullptr );

		HRESULT result = DirectInput8Create( hinst, 
											DIRECTINPUT_VERSION, 
											IID_IDirectInput8, 
											(void**)&m_pInputDevice, 
											nullptr);
		if( FAILED( result ) )
			return false;

		KeyboardManagerParameter keyboardParams;
		keyboardParams.m_pInputDevice	= m_pInputDevice;
		keyboardParams.m_phWnd			= params.p_hWnd;
		
		if( !m_keyboardManager.create( keyboardParams ) )
			return false;

		MouseManagerParameter mouseParams;
		mouseParams.m_phWnd			= params.p_hWnd;
		mouseParams.m_pInputDevice	= m_pInputDevice;
		mouseParams.m_screenwidth	= params.screenWidth;
		mouseParams.m_screenheight	= params.screenHeight;

		if( !m_mouseManager.create( mouseParams ) )
			return false;

		return true;
	}

	void InputSystem::dispose( void )
	{
		if ( m_pInputDevice == nullptr )
		{
			return;
		}

		m_mouseManager.dispose();
		m_keyboardManager.dispose();
		safeRelease( &m_pInputDevice );
	}

	void InputSystem::frame( void )
	{
		m_keyboardManager.frame();
		m_mouseManager.frame();
	}

	bool InputSystem::isKeyDown( const Keys key ) const
	{
		return m_keyboardManager.isKeyDown( key );
	}

	bool InputSystem::isKeyUp( const Keys key ) const
	{
		return m_keyboardManager.isKeyUp( key );
	}

	bool InputSystem::isKeyPressed( const Keys key ) const
	{
		return m_keyboardManager.isKeyPressed( key );
	}

	bool InputSystem::isKeyReleased( const Keys key ) const
	{
		return m_keyboardManager.isKeyReleased( key );
	}

	bool InputSystem::isButtonDown( const MouseButtons& button ) const
	{
		return m_mouseManager.isButtonDown( button );
	}

	bool InputSystem::isButtonUp( const MouseButtons& button ) const
	{
		return m_mouseManager.isButtonUp( button );
	}

	bool InputSystem::isButtonPressed( const MouseButtons& button ) const
	{
		return m_mouseManager.isButtonPressed( button );
	}

	bool InputSystem::isButtonReleased( const MouseButtons& button ) const
	{
		return m_mouseManager.isButtonReleased( button );
	}

	const Vector2& InputSystem::getMousePosition( void ) const
	{
		return m_mouseManager.getMousePosition();
	}

	const Vector2& InputSystem::getMousePositionDisplay( void ) const
	{
		return m_mouseManager.getMousePositionDisplay();
	}

	const Vector2& InputSystem::getDistance( void ) const
	{
		return m_mouseManager.getDistance();
	}
}