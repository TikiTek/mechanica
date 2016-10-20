
#include "tiki/debuggui/debugguihorizontallayout.hpp"

namespace tiki
{
	DebugGuiHorizontalLayout::DebugGuiHorizontalLayout()
		: m_expandChildren( false )
	{
	}

	Vector2 DebugGuiHorizontalLayout::getMinimumSize()
	{
		Vector2 minSize = { 0.0f, 0.0f };
		for ( LinkedIterator< DebugGuiControl > it = getChildrenBegin(); it != getChildrenEnd(); ++it )
		{
			const Vector2 childMinSize = it->getMinimumSize();

			minSize.x += childMinSize.x + getPadding().left;
			minSize.y = TIKI_MAX( minSize.y, childMinSize.y );
		}

		minSize.x = TIKI_MAX( getPadding().left, minSize.x );
		minSize.x += getPadding().right;
		minSize.y += getPadding().getHeight();

		return minSize;
	}

	bool DebugGuiHorizontalLayout::getExpandChildren() const
	{
		return m_expandChildren;
	}

	void DebugGuiHorizontalLayout::setExpandChildren( bool value )
	{
		m_expandChildren = value;
		refreshRectangle();
	}

	void DebugGuiHorizontalLayout::handleRectangleChanged( const AxisAlignedRectangle& boundingRectangle )
	{
		const float y			= boundingRectangle.getTop() + getPadding().top;
		const float paddingX	= getPadding().right + ( getPadding().left * TIKI_MAX( 1.0f, (float)getChildCount() ) );
		const float maxWidth	= boundingRectangle.getWidth() - paddingX;
		const float maxHeight	= boundingRectangle.getHeight() - getPadding().getHeight();

		const Vector2 minSize = getMinimumSize();
		const float minSizeXWithoutPadding = minSize.x - paddingX;
		const float expandingScale = ( minSizeXWithoutPadding == 0.0f ? 1.0f : maxWidth / minSizeXWithoutPadding );
		const float scale = (m_expandChildren ? expandingScale : 1.0f );
		TIKI_ASSERT( scale >= 1.0f );

		float currentX = boundingRectangle.getLeft() + getPadding().left;
		for ( LinkedIterator< DebugGuiControl > it = getChildrenBegin(); it != getChildrenEnd(); ++it )
		{
			const Vector2 childMinSize = it->getMinimumSize();

			const AxisAlignedRectangle childRect = createAxisAlignedRectangle( currentX, y, childMinSize.x * scale, maxHeight );
			TIKI_ASSERT( childRect.getRight() <= boundingRectangle.getRight() );
			TIKI_ASSERT( childRect.getBottom() <= boundingRectangle.getBottom() );
			it->setRectangle( childRect );

			currentX += childRect.getWidth() + getPadding().left;
		}
	}
}