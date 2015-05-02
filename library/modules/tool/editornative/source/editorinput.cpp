
#include "tiki/editornative/editorinput.hpp"

#include "tiki/input/inputevent.hpp"

namespace tiki
{
	static const System::Windows::Input::Key s_aKeyboardMaaping[ KeyboardKey_Count ] =
	{
		System::Windows::Input::Key::Escape,	// KeyboardKey_Escape,
		System::Windows::Input::Key::Back,	// KeyboardKey_BackSpace,
		System::Windows::Input::Key::Tab,	// KeyboardKey_Tab,
		System::Windows::Input::Key::Return,	// KeyboardKey_Return,
		System::Windows::Input::Key::LeftCtrl,	// KeyboardKey_LeftControl,
		System::Windows::Input::Key::RightCtrl,	// KeyboardKey_RightControl,
		System::Windows::Input::Key::LeftShift,	// KeyboardKey_LeftShift,
		System::Windows::Input::Key::RightShift,	// KeyboardKey_RightShift,
		System::Windows::Input::Key::LeftAlt,	// KeyboardKey_LeftAlt,
		System::Windows::Input::Key::RightAlt,	// KeyboardKey_RightAlt,
		System::Windows::Input::Key::LWin,	// KeyboardKey_LeftSystem
		System::Windows::Input::Key::RWin,	// KeyboardKey_RightSystem

		System::Windows::Input::Key::Insert,	// KeyboardKey_Insert,
		System::Windows::Input::Key::Delete,	// KeyboardKey_Delete,
		System::Windows::Input::Key::Home,	// KeyboardKey_Home,
		System::Windows::Input::Key::End,	// KeyboardKey_End,
		System::Windows::Input::Key::PageUp,	// KeyboardKey_PageUp,
		System::Windows::Input::Key::PageDown,	// KeyboardKey_PageDown,

		System::Windows::Input::Key::Left,	// KeyboardKey_Left,
		System::Windows::Input::Key::Up,	// KeyboardKey_Up,
		System::Windows::Input::Key::Down,	// KeyboardKey_Down,
		System::Windows::Input::Key::Right,	// KeyboardKey_Right,

		System::Windows::Input::Key::Space,	// KeyboardKey_Space,
		System::Windows::Input::Key::OemMinus,	// KeyboardKey_Minus,
		System::Windows::Input::Key::OemPlus,	// KeyboardKey_Plus,
		System::Windows::Input::Key::Divide,	// KeyboardKey_Slash,
		System::Windows::Input::Key::Multiply,	// KeyboardKey_Star,
		System::Windows::Input::Key::DbeNoCodeInput,	// KeyboardKey_Equals,
		System::Windows::Input::Key::OemSemicolon,	// KeyboardKey_Semicolon,
		System::Windows::Input::Key::DbeNoCodeInput,	// KeyboardKey_Apostrophe,
		System::Windows::Input::Key::OemBackslash,	// KeyboardKey_BackSlash,
		System::Windows::Input::Key::OemComma,	// KeyboardKey_Comma,
		System::Windows::Input::Key::Decimal,	// KeyboardKey_Dot,
		System::Windows::Input::Key::DbeNoCodeInput,	// KeyboardKey_GraveAccent,

		System::Windows::Input::Key::D1,	// KeyboardKey_Alpha1,
		System::Windows::Input::Key::D2,	// KeyboardKey_Alpha2,
		System::Windows::Input::Key::D3,	// KeyboardKey_Alpha3,
		System::Windows::Input::Key::D4,	// KeyboardKey_Alpha4,
		System::Windows::Input::Key::D5,	// KeyboardKey_Alpha5,
		System::Windows::Input::Key::D6,	// KeyboardKey_Alpha6,
		System::Windows::Input::Key::D7,	// KeyboardKey_Alpha7,
		System::Windows::Input::Key::D8,	// KeyboardKey_Alpha8,
		System::Windows::Input::Key::D9,	// KeyboardKey_Alpha9,
		System::Windows::Input::Key::D0,	// KeyboardKey_Alpha0,
		
		System::Windows::Input::Key::A,	// KeyboardKey_A,
		System::Windows::Input::Key::B,	// KeyboardKey_B,
		System::Windows::Input::Key::C,	// KeyboardKey_C,
		System::Windows::Input::Key::D,	// KeyboardKey_D,
		System::Windows::Input::Key::E,	// KeyboardKey_E,
		System::Windows::Input::Key::F,	// KeyboardKey_F,
		System::Windows::Input::Key::G,	// KeyboardKey_G,
		System::Windows::Input::Key::H,	// KeyboardKey_H,
		System::Windows::Input::Key::I,	// KeyboardKey_I,
		System::Windows::Input::Key::J,	// KeyboardKey_J,
		System::Windows::Input::Key::K,	// KeyboardKey_K,
		System::Windows::Input::Key::L,	// KeyboardKey_L,
		System::Windows::Input::Key::M,	// KeyboardKey_M,
		System::Windows::Input::Key::N,	// KeyboardKey_N,
		System::Windows::Input::Key::O,	// KeyboardKey_O,
		System::Windows::Input::Key::P,	// KeyboardKey_P,
		System::Windows::Input::Key::Q,	// KeyboardKey_Q,
		System::Windows::Input::Key::R,	// KeyboardKey_R,
		System::Windows::Input::Key::S,	// KeyboardKey_S,
		System::Windows::Input::Key::T,	// KeyboardKey_T,
		System::Windows::Input::Key::U,	// KeyboardKey_U,
		System::Windows::Input::Key::V,	// KeyboardKey_V,
		System::Windows::Input::Key::W,	// KeyboardKey_W,
		System::Windows::Input::Key::X,	// KeyboardKey_X,
		System::Windows::Input::Key::Y,	// KeyboardKey_Y,
		System::Windows::Input::Key::Z,	// KeyboardKey_Z,

		System::Windows::Input::Key::F1,	// KeyboardKey_F1,
		System::Windows::Input::Key::F2,	// KeyboardKey_F2,
		System::Windows::Input::Key::F3,	// KeyboardKey_F3,
		System::Windows::Input::Key::F4,	// KeyboardKey_F4,
		System::Windows::Input::Key::F5,	// KeyboardKey_F5,
		System::Windows::Input::Key::F6,	// KeyboardKey_F6,
		System::Windows::Input::Key::F7,	// KeyboardKey_F7,
		System::Windows::Input::Key::F8,	// KeyboardKey_F8,
		System::Windows::Input::Key::F9,	// KeyboardKey_F9,
		System::Windows::Input::Key::F10,	// KeyboardKey_F10,
		System::Windows::Input::Key::F11,	// KeyboardKey_F11,
		System::Windows::Input::Key::F12,	// KeyboardKey_F12,

		System::Windows::Input::Key::NumPad0,	// KeyboardKey_Numpad0,
		System::Windows::Input::Key::NumPad1,	// KeyboardKey_Numpad1,
		System::Windows::Input::Key::NumPad2,	// KeyboardKey_Numpad2,
		System::Windows::Input::Key::NumPad3,	// KeyboardKey_Numpad3,
		System::Windows::Input::Key::NumPad4,	// KeyboardKey_Numpad4,
		System::Windows::Input::Key::NumPad5,	// KeyboardKey_Numpad5,
		System::Windows::Input::Key::NumPad6,	// KeyboardKey_Numpad6,
		System::Windows::Input::Key::NumPad7,	// KeyboardKey_Numpad7,
		System::Windows::Input::Key::NumPad8,	// KeyboardKey_Numpad8,
		System::Windows::Input::Key::NumPad9,	// KeyboardKey_Numpad9,
		System::Windows::Input::Key::DbeNoCodeInput,	// KeyboardKey_NumpadEquals,
		System::Windows::Input::Key::DbeNoCodeInput,	// KeyboardKey_NumpadComma,
		System::Windows::Input::Key::DbeNoCodeInput,	// KeyboardKey_NumpadEnter,

		System::Windows::Input::Key::CapsLock,	// KeyboardKey_CapsLock,
		System::Windows::Input::Key::NumLock,	// KeyboardKey_Numlock,
		System::Windows::Input::Key::Scroll		// KeyboardKey_Scroll,
	};

	static MouseButton getMouseButton( System::Windows::Forms::MouseButtons formsButton )
	{
		switch ( formsButton )
		{
		case System::Windows::Forms::MouseButtons::Left:
			return MouseButton_Left;
			break;

		case System::Windows::Forms::MouseButtons::Right:
			return MouseButton_Right;
			break;

		case System::Windows::Forms::MouseButtons::Middle:
			return MouseButton_Middle;
			break;
		}

		return MouseButton_Count;
	}

	EditorInput::EditorInput()
	{
		m_pEvents = nullptr;

		m_LastPositionX	= 0;
		m_LastPositionY	= 0;
	}

	bool EditorInput::create()
	{
		m_pEvents = new Queue< InputEvent >();
		if ( !m_pEvents->create( 256u ) )
		{
			return false;
		}

		for (int i = 0u; i < KeyboardKey_Count; ++i)
		{
			m_KeyMapping[ s_aKeyboardMaaping[ i ] ] = i;
		}

		return true;
	}

	void EditorInput::dispose()
	{
		if ( m_pEvents )
		{
			m_pEvents->dispose();
			delete m_pEvents;
		}
	}

	bool EditorInput::popInputEvent( InputEvent& targetEvent )
	{
		return m_pEvents->pop( targetEvent );
	}

	void EditorInput::onKeyDown( KeyEventArgs^ e )
	{
		int keyEnum = 0;
		if ( !m_KeyMapping.TryGetValue( e->Key, keyEnum ) )
		{
			return;
		}

		InputEvent& inputEvent = m_pEvents->push();
		inputEvent.eventType			= InputEventType_Keyboard_Down;
		inputEvent.deviceType			= InputDeviceType_Keyboard;
		inputEvent.deviceId				= 0u;
		inputEvent.data.keybaordKey.key = (KeyboardKey)keyEnum;
	}

	void EditorInput::onKeyUp( KeyEventArgs^ e )
	{
		int keyEnum = 0;
		if ( !m_KeyMapping.TryGetValue( e->Key, keyEnum ) )
		{
			return;
		}

		InputEvent& inputEvent = m_pEvents->push();
		inputEvent.eventType			= InputEventType_Keyboard_Up;
		inputEvent.deviceType			= InputDeviceType_Keyboard;
		inputEvent.deviceId				= 0u;
		inputEvent.data.keybaordKey.key = (KeyboardKey)keyEnum;
	}

	void EditorInput::onMouseDown( System::Windows::Forms::MouseEventArgs^ e )
	{
		const MouseButton button = getMouseButton( e->Button );
		if ( button == MouseButton_Count )
		{
			return;
		}

		InputEvent& inputEvent = m_pEvents->push();
		inputEvent.eventType				= InputEventType_Mouse_ButtonDown;
		inputEvent.deviceType				= InputDeviceType_Keyboard;
		inputEvent.deviceId					= 0u;
		inputEvent.data.mouseButton.button	= button;
	}

	void EditorInput::onMouseUp( System::Windows::Forms::MouseEventArgs^ e )
	{
		const MouseButton button = getMouseButton( e->Button );
		if ( button == MouseButton_Count )
		{
			return;
		}

		InputEvent& inputEvent = m_pEvents->push();
		inputEvent.eventType				= InputEventType_Mouse_ButtonUp;
		inputEvent.deviceType				= InputDeviceType_Keyboard;
		inputEvent.deviceId					= 0u;
		inputEvent.data.mouseButton.button	= button;
	}

	void EditorInput::onMouseDoubleClick( System::Windows::Forms::MouseEventArgs^ e )
	{
		const MouseButton button = getMouseButton( e->Button );
		if ( button == MouseButton_Count )
		{
			return;
		}

		InputEvent& inputEvent = m_pEvents->push();
		inputEvent.eventType				= InputEventType_Mouse_DoubleClick;
		inputEvent.deviceType				= InputDeviceType_Keyboard;
		inputEvent.deviceId					= 0u;
		inputEvent.data.mouseButton.button	= button;

	}

	void EditorInput::onMouseMove( System::Windows::Forms::MouseEventArgs^ e )
	{
		int deltaX = e->X - m_LastPositionX;
		int deltaY = e->Y - m_LastPositionY;
		m_LastPositionX = e->X;
		m_LastPositionY = e->Y;

		InputEvent& inputEvent = m_pEvents->push();
		inputEvent.eventType			= InputEventType_Mouse_Moved;
		inputEvent.deviceType			= InputDeviceType_Keyboard;
		inputEvent.deviceId				= 0u;
		inputEvent.data.mouseMoved.xOffset	= (sint16)deltaX;
		inputEvent.data.mouseMoved.yOffset	= (sint16)deltaY;
		inputEvent.data.mouseMoved.xState	= (sint16)e->X;
		inputEvent.data.mouseMoved.xState	= (sint16)e->Y;
	}
}