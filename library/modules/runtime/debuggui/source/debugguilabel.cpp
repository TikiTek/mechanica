
#include "tiki/debuggui/debugguilabel.hpp"

namespace tiki
{
	void DebugGuiLabel::create( const char* pText )
	{
		setText( pText );
	}

	void DebugGuiLabel::dispose()
	{
	}

	const char* DebugGuiLabel::getText() const
	{
		return m_aText;
	}

	void DebugGuiLabel::setText( const char* pText )
	{
		TIKI_ASSERT( pText != nullptr );
		copyString( m_aText, TIKI_COUNT( m_aText ), pText );
		setRectangle( getRectangle() );
	}

	tiki::Vector2 DebugGuiLabel::getMinimumSize()
	{
		Vector2 textSize;
		getDefaultFont()->calcuateTextSize( textSize, m_aText, getStringLength( m_aText ) );
		return textSize;
	}

	void DebugGuiLabel::handleRectangleChanged( const Rectangle& boundingRectangle )
	{
	}

	void DebugGuiLabel::update()
	{
	}

	void DebugGuiLabel::render( ImmediateRenderer& renderer )
	{
		renderer.drawText( getRectangle().xy(), *getDefaultFont(), m_aText, TIKI_COLOR_WHITE );
	}

	bool DebugGuiLabel::processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state )
	{
		return false;
	}
}