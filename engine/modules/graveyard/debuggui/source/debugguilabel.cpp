
#include "tiki/debuggui/debugguilabel.hpp"

#include "tiki/base/string.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/immediaterenderer.hpp"

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
		refreshRectangle();
	}

	Vector2 DebugGuiLabel::getMinimumSize()
	{
		Vector2 textSize;
		getDefaultFont()->calcuateTextSize( textSize, m_aText, getStringSize( m_aText ) );
		return textSize;
	}

	void DebugGuiLabel::handleRectangleChanged( const AxisAlignedRectangle& boundingRectangle )
	{
	}

	void DebugGuiLabel::update( double elapsedTime )
	{
	}

	void DebugGuiLabel::render( ImmediateRenderer& renderer )
	{
		renderer.drawText( getRectangle().min, *getDefaultFont(), m_aText, TIKI_COLOR_WHITE );
	}

	bool DebugGuiLabel::processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state )
	{
		return false;
	}
}
