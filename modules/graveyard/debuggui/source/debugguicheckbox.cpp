
#include "tiki/debuggui/debugguicheckbox.hpp"

#include "tiki/base/string.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/debuggui/debuggui.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/input/inputevent.hpp"

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
		getDefaultFont()->calcuateTextSize( textSize, m_aText, getStringSize( m_aText ) );
		textSize.x += ( 5.0f * getPadding().left ) + getPadding().right;
		textSize.y = ( 4.0f * getPadding().top ) + getPadding().bottom;

		return textSize;
	}

	void DebugGuiCheckBox::update( double elapsedTime )
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

	void DebugGuiCheckBox::handleRectangleChanged( const AxisAlignedRectangle& boundingRectangle )
	{
		const float boxWidth = getPadding().left * 3.0f;
		const float boxHeight = getPadding().top * 3.0f;

		m_textPosition.x = boundingRectangle.getLeft() + ( 5.0f * getPadding().left );
		m_textPosition.y = boundingRectangle.getTop() + getPadding().top;

		m_box = createAxisAlignedRectangle(
			boundingRectangle.getLeft() + getPadding().left,
			boundingRectangle.getTop() + getPadding().top,
			boxWidth,
			boxHeight
		);

		m_checker = createAxisAlignedRectangle(
			boundingRectangle.getLeft() + getPadding().left + ((boxWidth - m_checker.getWidth()) / 2.0f),
			boundingRectangle.getTop() + getPadding().top + ((boxHeight - m_checker.getHeight()) / 2.0f),
			boxWidth * 0.6f,
			boxHeight * 0.6f
		);
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
