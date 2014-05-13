
#include "tiki/debuggui/debugguiminimumlayout.hpp"

namespace tiki
{
	void DebugGuiMinimumLayout::addChildControl( DebugGuiControl* pChild )
	{
		if ( getChildCount() == 0u )
		{
			DebugGuiLayout::addChildControl( pChild );
		}
		else
		{
			TIKI_TRACE_ERROR( "[DebugGuiMinimumLayout] Only one child allowed.\n");
		}
	}

	void DebugGuiMinimumLayout::handleRectangleChanged( const Rectangle& boundingRectangle )
	{
		const Vector2 minSize = getMinimumSize();
		for (uint childIndex = 0u; childIndex < getChildCount(); ++childIndex)
		{
			DebugGuiControl* pChild = getChildByIndex( childIndex );

			Rectangle rectangle;
			rectangle.x = boundingRectangle.x;
			rectangle.y = boundingRectangle.y;
			rectangle.width = minSize.x;
			rectangle.height = minSize.y;
			pChild->setRectangle( rectangle );
		}
	}

	Vector2 DebugGuiMinimumLayout::getMinimumSize()
	{
		Vector2 minSize = { 0.0f, 0.0f };
		for (uint childIndex = 0u; childIndex < getChildCount(); ++childIndex)
		{
			DebugGuiControl* pChild = getChildByIndex( childIndex );
			const Vector2 childMinSize = pChild->getMinimumSize();

			minSize.x = TIKI_MAX( minSize.x, childMinSize.x );
			minSize.y = TIKI_MAX( minSize.y, childMinSize.y );
		}
		return minSize;
	}

}