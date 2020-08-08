#include "tiki/debuggui/debugguicontrol.hpp"

#include "tiki/debuggui/debuggui.hpp"
#include "tiki/math/vector.hpp"

namespace tiki
{
	const Font* DebugGuiControl::s_pDefaultFont	= nullptr;
	DebugGui* DebugGuiControl::s_pDebugGui		= nullptr;

	DebugGuiControl::DebugGuiControl()
		: m_padding( DebugGui_DefaultPadding )
		, m_pParent( nullptr )
	{
		m_rectangle.clear();
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

	const AxisAlignedRectangle& DebugGuiControl::getRectangle() const
	{
		return m_rectangle;
	}

	void DebugGuiControl::setRectangle( const AxisAlignedRectangle& boundingRectangle )
	{
		const Vector2 minSize = getMinimumSize();

		Vector2 extends = boundingRectangle.getSize();
		vector::maximum( extends, extends, minSize );

		m_rectangle = createAxisAlignedRectangle( boundingRectangle.getLeft(), boundingRectangle.getTop(), extends.x, extends.y );

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
