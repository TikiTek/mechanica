
#include "tiki/debuggui/debugguibutton.hpp"

#include "tiki/base/string.hpp"
#include "tiki/debuggui/debuggui.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/input/inputevent.hpp"

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
		const Color color = ( m_mouseOver ? ( m_mouseDown ? TIKI_COLOR( 164, 164, 255, 224 ) : TIKI_COLOR( 164, 164, 255, 196 ) ) : TIKI_COLOR( 194, 194, 255, 164 ) );

		renderer.drawRectangle( getRectangle(), color );
		Vector2 position = vector::create( DebugGui_DefaultMargin, DebugGui_DefaultMargin );
		renderer.drawText( vector::add( position, getRectangle().xy() ), *getDefaultFont(), m_aText, TIKI_COLOR_WHITE );
	}

	bool DebugGuiButton::processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state )
	{
		if ( inputEvent.eventType == InputEventType_Mouse_Moved )
		{
			m_mouseOver = getRectangle().contains( state.mousePosition );
		}
		else if ( inputEvent.eventType == InputEventType_Mouse_ButtonDown && m_mouseOver )
		{
			m_mouseDown = true;
			return true;
		}
		else if ( inputEvent.eventType == InputEventType_Mouse_ButtonUp && m_mouseDown && m_mouseOver )
		{
			pushEvent( DebugGuiEvent( this, DebugGuiEventType_Click ) );
			m_mouseDown = false;
			return true;
		}

		return false;
	}
}
