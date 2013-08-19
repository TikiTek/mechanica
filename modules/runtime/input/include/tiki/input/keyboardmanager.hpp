#ifndef TIKI_KEYBOARDMANAGER_HPP__
#define TIKI_KEYBOARDMANAGER_HPP__

#include "tiki/base/keys.hpp"

struct IDirectInputA;
struct IDirectInputDeviceA;

namespace tiki
{
	struct KeyboardManagerParameter
	{
		IDirectInputA*	m_pInputDevice;
		void*			m_phWnd;
	};

	class KeyboardManager
	{
	public:
								KeyboardManager( void );
								~KeyboardManager( void );

		bool					create( const KeyboardManagerParameter& params );
		void					dispose( void );

		void					frame( void );

		bool					isKeyDown( const Keys key ) const;
		bool					isKeyUp( const Keys key ) const;

		bool					isKeyPressed( const Keys key ) const;
		bool					isKeyReleased( const Keys key ) const;

	private:

		IDirectInputDeviceA*	m_pKeyboard;

		byte					m_prevState[256];
		byte					m_currState[256];

	};
}

#endif // TIKI_KEYBOARDMANAGER_HPP__
