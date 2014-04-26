
#include "tiki/debuggui/debugguicontrol.hpp"

namespace tiki
{
	const Font* DebugGuiControl::s_pDefaultFont = nullptr;

	const Font* DebugGuiControl::getDefaultFont()
	{
		return s_pDefaultFont;
	}

	void DebugGuiControl::setDefaultFont( const Font* pDefaultFont )
	{
		s_pDefaultFont = pDefaultFont;
	}

	const Rectangle& DebugGuiControl::getRectangle() const
	{
		return m_rectangle;
	}

	void DebugGuiControl::setRectangle( const Rectangle& boundingRectangle )
	{
		m_rectangle = boundingRectangle;

		const Vector2 minSize = getMinimumSize();
		m_rectangle.width = TIKI_MAX( minSize.x, m_rectangle.width );
		m_rectangle.height = TIKI_MAX( minSize.y, m_rectangle.height );

		handleRectangleChanged( m_rectangle );
	}
}