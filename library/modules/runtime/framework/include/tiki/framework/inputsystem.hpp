#ifndef TIKI_INPUTSYSTEM_HPP__
#define TIKI_INPUTSYSTEM_HPP__

#include "tiki/base/keys.hpp"
#include "tiki/base/mouse.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/graphicssystem.hpp"

struct IDirectInputA;
struct IDirectInputDeviceA;

namespace tiki
{
	class MainWindow;
	class WindowEventBuffer;

	struct InputSystemParameters
	{
		const MainWindow*	pWindow;
	};

	struct InputSystemState
	{
		uint8		mouseState[ 16u ];
		uint8		keyboardState[ 256u ];
	};

	class InputSystem
	{
		TIKI_NONCOPYABLE_CLASS( InputSystem );
		friend struct FrameworkData;

	public:

		bool					create( const InputSystemParameters& params );
		void					dispose();

		void					update( const WindowEventBuffer& windowEvents );

		bool					isKeyDown( Keys key ) const;
		bool					isKeyUp( Keys key ) const;

		bool					hasKeyPressed( Keys key ) const;
		bool					hasKeyReleased( Keys key ) const;

		bool					isButtonDown( MouseButtons button ) const;
		bool					isButtonUp( MouseButtons button ) const;

		bool					isButtonPressed( MouseButtons button ) const;
		bool					isButtonReleased( MouseButtons button ) const;

		const Vector2&			getMousePosition() const;
		const Vector2&			getMousePositionDisplay() const;
		const Vector2&			getDistance() const;

	protected:

								InputSystem();
								~InputSystem();
							
	private:

		IDirectInputA*			m_pInputDevice;
		IDirectInputDeviceA*	m_pMouse;
		IDirectInputDeviceA*	m_pKeyboard;

		InputSystemState		m_currentState;
		InputSystemState		m_previousState;
		
	};
}
#endif // TIKI_INPUTSYSTEM_HPP__
