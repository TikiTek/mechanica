
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

	void DebugGuiVerticalLayout::handleRectangleChanged( const Rectangle& boundingRectangle )
	{
		const float x			= boundingRectangle.x + getPadding().left;
		const float paddingY	= getPadding().bottom + ( getPadding().top * TIKI_MAX( 1.0f, (float)getChildCount() ) );
		const float maxWidth	= boundingRectangle.width - getPadding().getWidth();
		const float maxHeight	= boundingRectangle.height - paddingY;

		const Vector2 minSize = getMinimumSize();
		const float minSizeYWithoutPadding = minSize.y - paddingY;
		const float expandingScale = ( minSizeYWithoutPadding == 0.0f ? 1.0f : maxHeight / minSizeYWithoutPadding );
		const float scale = ( m_expandChildren ? expandingScale : 1.0f );
		TIKI_ASSERT( scale >= 1.0f );

		float currentY = boundingRectangle.y + getPadding().top;
		for ( LinkedIterator< DebugGuiControl > it = getChildrenBegin(); it != getChildrenEnd(); ++it )
		{
			const Vector2 childMinSize = it->getMinimumSize();

			Rectangle childRect;
			childRect.x			= x;
			childRect.y			= currentY;
			childRect.width		= maxWidth;
			childRect.height	= childMinSize.y * scale;
			TIKI_ASSERT( childRect.getRight() <= boundingRectangle.getRight() );
			TIKI_ASSERT( childRect.getBottom() <= boundingRectangle.getBottom() );
			it->setRectangle( childRect );

			currentY += childRect.height + getPadding().top;
		}
	}
}