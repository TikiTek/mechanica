#ifndef TIKI_MOUSEMANAGER_HPP__
#define TIKI_MOUSEMANAGER_HPP__

#include "tiki/base/mouse.hpp"
#include "tiki/base/types.hpp"
#include "tiki/math/vector2.hpp"

struct IDirectInputA;
struct IDirectInputDeviceA;

struct _DIMOUSESTATE;

namespace tiki
{
	struct MouseManagerParameter
	{
		IDirectInputA*			m_pInputDevice;
		void*					m_phWnd;
		uint					m_screenwidth;
		uint					m_screenheight;
	};

	class MouseManager
	{
	public:
								MouseManager();
								~MouseManager();

		bool					create( const MouseManagerParameter& params );
		void					dispose( void );

		void					frame( void );

		bool					isButtonDown( const MouseButtons button ) const;
		bool					isButtonUp( const MouseButtons button ) const;

		bool					isButtonPressed( const MouseButtons button ) const;
		bool					isButtonReleased( const MouseButtons button ) const;

		const Vector2&			getMousePosition( void ) const;
		const Vector2&			getMousePositionDisplay( void ) const;
		const Vector2&			getDistance( void ) const;

	private:

		IDirectInputDeviceA*	m_pMouse;

		void*					m_phwnd;

		Vector2					m_viewport;
		
		Vector2					m_position;
		Vector2					m_positionDisplay;
		Vector2					m_distance;

		float					m_wheel;

		byte					m_prevState[ 3 ];
		byte					m_currState[ 3 ];
		
		_DIMOUSESTATE*			m_pInputState;
		uint8					m_inputStateBuffer[ 16u ];

	};
}

#endif // TIKI_MOUSEMANAGER_HPP__
