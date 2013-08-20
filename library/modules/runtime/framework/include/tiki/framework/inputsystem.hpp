#ifndef TIKI_INPUTSYSTEM_HPP__
#define TIKI_INPUTSYSTEM_HPP__

#include "tiki/base/types.hpp"
#include "tiki/graphics/graphicsystem.hpp"

#include "tiki/input/keyboardmanager.hpp"
#include "tiki/input/mousemanager.hpp"

struct IDirectInputA;
struct IDirectInputDeviceA;

namespace tiki
{
	class WindowEventBuffer;

	struct InputSystemParameters
	{
		void*	p_hWnd;
		size_t	screenWidth;
		size_t	screenHeight;
	};

	struct InputSystemState
	{
		uint8		mouseState[ 16u ];
		uint8		keyboardState[ 256u ];
	};

	class InputSystem
	{
		TIKI_NONCOPYABLE_CLASS( InputSystem );

		friend class GameFramework;

	public:

		bool				create( const InputSystemParameters& params );
		void				dispose( void );

		void				update( const WindowEventBuffer& windowEvents );

		bool				isKeyDown( Keys key ) const;
		bool				isKeyUp( Keys key ) const;

		bool				hasKeyPressed( Keys key ) const;
		bool				hasKeyReleased( Keys key ) const;

		bool				isButtonDown( MouseButtons button ) const;
		bool				isButtonUp( MouseButtons button ) const;

		bool				isButtonPressed( MouseButtons button ) const;
		bool				isButtonReleased( MouseButtons button ) const;

		const Vector2&		getMousePosition( void ) const;
		const Vector2&		getMousePositionDisplay( void ) const;
		const Vector2&		getDistance( void ) const;
							
	private:

		IDirectInputA*			m_pInputDevice;
		IDirectInputDeviceA*	m_pMouse;
		IDirectInputDeviceA*	m_pKeyboard;

		InputSystemState		m_currentState;
		InputSystemState		m_previousState;
		
	};
}
#endif // TIKI_INPUTSYSTEM_HPP__
