
#include "tiki/debuggui/debugguiverticallayout.hpp"

namespace tiki
{
	DebugGuiVerticalLayout::DebugGuiVerticalLayout()
		: m_expandChildren( true )
	{
	}

	Vector2 DebugGuiVerticalLayout::getMinimumSize()
	{
		Vector2 minSize = { 0.0f, 0.0f };
		for ( LinkedIterator< DebugGuiControl > it = getChildrenBegin(); it != getChildrenEnd(); ++it )
		{
			const Vector2 childMinSize = it->getMinimumSize();

			minSize.x = TIKI_MAX( minSize.x, childMinSize.x );
			minSize.y += childMinSize.y + getPadding().top;
		}

		minSize.x += getPadding().getWidth();
		minSize.y = TIKI_MAX( getPadding().top, minSize.y );
		minSize.y += getPadding().bottom;

		return minSize;
	}

	bool DebugGuiVerticalLayout::getExpandChildren() const
	{
		return m_expandChildren;
	}

	void DebugGuiVerticalLayout::setExpandChildren( bool value )
	{
		m_expandChildren = value;
		refreshRectangle();
	}

	void DebugGuiVerticalLayout::handleRectangleChanged( const AxisAlignedRectangle& boundingRectangle )
	{
		const float x			= boundingRectangle.getLeft() + getPadding().left;
		const float paddingY	= getPadding().bottom + ( getPadding().top * TIKI_MAX( 1.0f, (float)getChildCount() ) );
		const float maxWidth	= boundingRectangle.getWidth() - getPadding().getWidth();
		const float maxHeight	= boundingRectangle.getHeight() - paddingY;

		const Vector2 minSize = getMinimumSize();
		const float minSizeYWithoutPadding = minSize.y - paddingY;
		const float expandingScale = ( minSizeYWithoutPadding == 0.0f ? 1.0f : maxHeight / minSizeYWithoutPadding );
		const float scale = ( m_expandChildren ? expandingScale : 1.0f );
		TIKI_ASSERT( scale >= 1.0f );

		float currentY = boundingRectangle.getTop() + getPadding().top;
		for ( LinkedIterator< DebugGuiControl > it = getChildrenBegin(); it != getChildrenEnd(); ++it )
		{
			const Vector2 childMinSize = it->getMinimumSize();

			AxisAlignedRectangle childRect = createAxisAlignedRectangle( x, currentY, maxWidth, childMinSize.y * scale );
			TIKI_ASSERT( childRect.getRight() <= boundingRectangle.getRight() );
			TIKI_ASSERT( childRect.getBottom() <= boundingRectangle.getBottom() );
			it->setRectangle( childRect );

			currentY += childRect.getHeight() + getPadding().top;
		}
	}
}