#ifndef TIKI_INPUTSYSTEM_HPP__
#define TIKI_INPUTSYSTEM_HPP__

#include "tiki/base/types.hpp"
#include "tiki/graphics/graphicsystem.hpp"

#include "tiki/input/keyboardmanager.hpp"
#include "tiki/input/mousemanager.hpp"

struct IDirectInputA;

namespace tiki
{
	struct InputSystemParameters
	{
		void*	p_hWnd;
		uint	screenWidth;
		uint	screenHeight;
	};

	class InputSystem
	{
		TIKI_NONCOPYABLE_CLASS( InputSystem );

		friend class GameFramework;

	public:
							InputSystem();
							~InputSystem();

		bool				isKeyDown( const Keys key ) const;
		bool				isKeyUp( const Keys key ) const;

		bool				isKeyPressed( const Keys key ) const;
		bool				isKeyReleased( const Keys key ) const;

		bool				isButtonDown( const MouseButtons& button ) const;
		bool				isButtonUp( const MouseButtons& button ) const;

		bool				isButtonPressed( const MouseButtons& button ) const;
		bool				isButtonReleased( const MouseButtons& button ) const;

		const Vector2&		getMousePosition( void ) const;
		const Vector2&		getMousePositionDisplay( void ) const;
		const Vector2&		getDistance( void ) const;
							
	private:

		bool				create( const InputSystemParameters& params );

		void				dispose( void );

		void				frame( void );

		IDirectInputA*		m_pInputDevice;

		KeyboardManager		m_keyboardManager;
		MouseManager		m_mouseManager;	
		
	};
}
#endif // TIKI_INPUTSYSTEM_HPP__
