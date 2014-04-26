
#include "tiki/debuggui/debugguihorizontallayout.hpp"

namespace tiki
{
	void DebugGuiHorizontalLayout::handleRectangleChanged( const Rectangle& boundingRectangle )
	{
		const float x			= boundingRectangle.x + DebugGui_DefaultMargin;
		const float maxWidth	= boundingRectangle.width - ( DebugGui_DefaultMargin * 2.0f );
		const float maxHeight	= boundingRectangle.height - ( DebugGui_DefaultMargin * 2.0f );

		const Vector2 minSize = getMinimumSize();
		const float scale = maxHeight / minSize.y;
		
		float currentY = boundingRectangle.y + DebugGui_DefaultMargin;
		for (uint childIndex = 0u; childIndex < getChildCount(); ++childIndex)
		{
			DebugGuiControl* pChild = getChildByIndex( childIndex );
			const Vector2 childMinSize = pChild->getMinimumSize();

			Rectangle childRect;
			childRect.x			= x;
			childRect.y			= currentY;
			childRect.width		= maxWidth;
			childRect.height	= childMinSize.y * scale;
			pChild->setRectangle( childRect );

			currentY += childRect.height + DebugGui_DefaultMargin;
		} 
	}

	Vector2 DebugGuiHorizontalLayout::getMinimumSize()
	{
		Vector2 minSize = { 0.0f, DebugGui_DefaultMargin };
		for (uint childIndex = 0u; childIndex < getChildCount(); ++childIndex)
		{
			DebugGuiControl* pChild = getChildByIndex( childIndex );
			const Vector2 childMinSize = pChild->getMinimumSize();

			minSize.y += childMinSize.y + DebugGui_DefaultMargin;
			minSize.x = TIKI_MAX( minSize.x, childMinSize.x );
		}
		minSize.x += DebugGui_DefaultMargin * 2.0f;
		return minSize;
	}
}