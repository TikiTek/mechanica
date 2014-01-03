#ifndef TIKI_INPUTSYSTEM_HPP__
#define TIKI_INPUTSYSTEM_HPP__

#include "tiki/base/keys.hpp"
#include "tiki/base/mouse.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/math/vector.hpp"

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

		const Vector2&			getMousePosition() const			{ return m_mousePosition; }
		const Vector2&			getMousePositionNormalized() const	{ return m_mousePositionNormalized; }
		const Vector2&			getMouseDeltaNormalized() const		{ return m_mouseDeltaNormalized; }

	protected:

								InputSystem();
								~InputSystem();
							
	private:

		uint					m_screenWidth;
		uint					m_screenHeight;

		IDirectInputA*			m_pInputDevice;
		IDirectInputDeviceA*	m_pMouse;
		IDirectInputDeviceA*	m_pKeyboard;

		Vector2					m_mousePosition;
		Vector2					m_mousePositionNormalized;
		Vector2					m_mouseDeltaNormalized;

		InputSystemState		m_currentState;
		InputSystemState		m_previousState;
		
	};
}
#endif // TIKI_INPUTSYSTEM_HPP__
