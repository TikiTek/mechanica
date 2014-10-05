
#include "tiki/debuggui/debugguicontrol.hpp"

#include "tiki/debuggui/debuggui.hpp"

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
		const Vector2 minSize = getMinimumSize();
		Rectangle rectangle = boundingRectangle;
		rectangle.width = TIKI_MAX( minSize.x, rectangle.width );
		rectangle.height = TIKI_MAX( minSize.y, rectangle.height );
		m_rectangle = rectangle;

		handleRectangleChanged( m_rectangle );
	}
}
