
#include "tiki/debuggui/debugguicontrol.hpp"

#include "tiki/debuggui/debuggui.hpp"

namespace tiki
{
	const Font* DebugGuiControl::s_pDefaultFont	= nullptr;
	DebugGui* DebugGuiControl::s_pDebugGui		= nullptr;

	DebugGuiControl::DebugGuiControl()
		: m_padding( DebugGui_DefaultPadding )
		, m_pParent( nullptr )
	{
	}

	const DebugGuiControl* DebugGuiControl::getParent() const
	{
		return m_pParent;
	}

	bool DebugGuiControl::isInHierarchy() const
	{
		if ( m_pParent == nullptr )
		{
			return false;
		}

		return m_pParent->isInHierarchy();
	}

	const Thickness& DebugGuiControl::getPadding() const
	{
		return m_padding;
	}

	void DebugGuiControl::setPadding( const Thickness& padding )
	{
		m_padding = padding;
		refreshRectangle();
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

	void DebugGuiControl::refreshRectangle()
	{
		setRectangle( m_rectangle );
	}

	/*static*/ const Font* DebugGuiControl::getDefaultFont()
	{
		TIKI_ASSERT( s_pDefaultFont != nullptr );
		return s_pDefaultFont;
	}

	/*static*/ void DebugGuiControl::pushEvent( const DebugGuiEvent& guiEvent )
	{
		TIKI_ASSERT( s_pDebugGui != nullptr );
		s_pDebugGui->pushEvent( guiEvent );
	}

	void DebugGuiControl::setParent( const DebugGuiControl* pParent )
	{
		m_pParent = pParent;
	}

	/*static*/ void DebugGuiControl::initialize( const Font* pDefaultFont, DebugGui* pDebugGui )
	{
		TIKI_ASSERT( s_pDefaultFont == nullptr );
		TIKI_ASSERT( s_pDebugGui == nullptr );
		s_pDefaultFont	= pDefaultFont;
		s_pDebugGui		= pDebugGui;
	}

	/*static*/ void DebugGuiControl::shutdown()
	{
		TIKI_ASSERT( s_pDefaultFont != nullptr );
		TIKI_ASSERT( s_pDebugGui != nullptr );
		s_pDefaultFont	= nullptr;
		s_pDebugGui		= nullptr;
	}
}
