
#include "tiki/debuggui/debugguicheckbox.hpp"

namespace tiki
{
	void DebugGuiCheckBox::create( const char* pText, bool value )
	{
		m_value = value;
		copyString( m_aText, TIKI_COUNT( m_aText ), pText );

		m_mouseOver = false;
		m_mouseDown = false;
	}

	void DebugGuiCheckBox::dispose()
	{
		m_mouseOver = false;
		m_mouseDown = false;
	}

	bool DebugGuiCheckBox::getValue() const
	{
		return m_value;
	}

	void DebugGuiCheckBox::setValue( bool value )
	{
		m_value = value;
	}

	Vector2 DebugGuiCheckBox::getMinimumSize()
	{
		Vector2 textSize;
		getDefaultFont()->calcuateTextSize( textSize, m_aText, getStringLength( m_aText ) );
		textSize.x += ( 3.0f * DebugGui_DefaultMargin ) + BoxSize;
		textSize.y = ( 2.0f * DebugGui_DefaultMargin ) + BoxSize;

		return textSize;
	}

	void DebugGuiCheckBox::update()
	{
	}

	void DebugGuiCheckBox::render( ImmediateRenderer& renderer )
	{
		renderer.drawText( m_textPosition, *getDefaultFont(), m_aText, TIKI_COLOR_WHITE );
		renderer.drawRectangle( m_box, TIKI_COLOR( 255, 255, 255, 128 ) );

		if ( m_value )
		{
			renderer.drawRectangle( m_checker, TIKI_COLOR( 128, 128, 255, 224 ) );
		}
	}

	void DebugGuiCheckBox::handleRectangleChanged( const Rectangle& boundingRectangle )
	{
		m_textPosition.x = boundingRectangle.x + ( DebugGui_DefaultMargin * 2.0f ) + BoxSize;
		m_textPosition.y = boundingRectangle.y + DebugGui_DefaultMargin;

		m_box.x			= boundingRectangle.x + DebugGui_DefaultMargin;
		m_box.y			= boundingRectangle.y + DebugGui_DefaultMargin;
		m_box.width		= BoxSize;
		m_box.height	= BoxSize;

		m_checker.x			= boundingRectangle.x + DebugGui_DefaultMargin + 3.0f;
		m_checker.y			= boundingRectangle.y + DebugGui_DefaultMargin + 3.0f;
		m_checker.width		= CheckerSize;
		m_checker.height	= CheckerSize;
	}

	bool DebugGuiCheckBox::processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state )
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
			m_value = !m_value;
			pushEvent( DebugGuiEvent( this, DebugGuiEventType_ValueChanged ) );
			m_mouseDown = false;
			return true;
		}

		return false;
	}
}