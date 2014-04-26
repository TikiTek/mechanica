
#include "tiki/debuggui/debugguibutton.hpp"

namespace tiki
{
	void DebugGuiButton::create( const char* pText )
	{
		m_mouseOver = false;
		m_mouseDown = false;
		copyString( m_aText, TIKI_COUNT( m_aText ), pText );
	}

	void DebugGuiButton::dispose()
	{
		m_mouseOver = false;
		m_mouseDown = false;
	}

	void DebugGuiButton::handleRectangleChanged( const Rectangle& /*boundingRectangle*/ )
	{
	}

	Vector2 DebugGuiButton::getMinimumSize()
	{
		return vector::create( 25, 25 );
	}

	void DebugGuiButton::update()
	{
	}

	void DebugGuiButton::render( ImmediateRenderer& renderer )
	{
		renderer.drawTexture( nullptr, getRectangle(), m_mouseOver ? TIKI_COLOR( 164, 164, 255, 196 ) : TIKI_COLOR( 194, 194, 255, 164 ) );
		renderer.drawText( vector::add( vector::create( DebugGui_DefaultMargin, DebugGui_DefaultMargin ), getRectangle().xy() ), *getDefaultFont(), m_aText, TIKI_COLOR_WHITE );
	}

	bool DebugGuiButton::processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state )
	{
		if ( inputEvent.eventType == InputEventType_Mouse_Moved )
		{
			m_mouseOver = getRectangle().contains( state.mousePosition );
		}
		else if ( inputEvent.eventType == InputEventType_Controller_ButtonDown && m_mouseOver )
		{
			m_mouseDown = true;
			return true;
		}
		else if ( inputEvent.eventType == InputEventType_Controller_ButtonUp && m_mouseDown && m_mouseOver )
		{
			// todo click
			m_mouseDown = false;
			return true;
		}

		return false;
	}
}