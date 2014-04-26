
#include "tiki/debuggui/debugguicontrol.hpp"

namespace tiki
{
	const Font* DebugGuiControl::s_pDefaultFont	= nullptr;
	DebugGui* DebugGuiControl::s_pDebugGui		= nullptr;

	const Font* DebugGuiControl::getDefaultFont()
	{
		TIKI_ASSERT( s_pDefaultFont != nullptr );
		return s_pDefaultFont;
	}

	void DebugGuiControl::pushEvent( const DebugGuiEvent& guiEvent )
	{
		TIKI_ASSERT( s_pDebugGui != nullptr );
		s_pDebugGui->pushEvent( guiEvent );
	}

	void DebugGuiControl::initialize( const Font* pDefaultFont, DebugGui* pDebugGui )
	{
		TIKI_ASSERT( s_pDefaultFont == nullptr );
		TIKI_ASSERT( s_pDebugGui == nullptr );
		s_pDefaultFont	= pDefaultFont;
		s_pDebugGui		= pDebugGui;
	}

	void DebugGuiControl::shutdown()
	{
		TIKI_ASSERT( s_pDefaultFont != nullptr );
		TIKI_ASSERT( s_pDebugGui != nullptr );
		s_pDefaultFont	= nullptr;
		s_pDebugGui		= nullptr;
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