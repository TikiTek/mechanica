
#include "tiki/debuggui/debugguiverticallayout.hpp"

namespace tiki
{
	void DebugGuiVerticalLayout::handleRectangleChanged( const Rectangle& boundingRectangle )
	{
		const float y			= boundingRectangle.y + DebugGui_DefaultMargin;
		const float maxWidth	= boundingRectangle.width - ( DebugGui_DefaultMargin * 2.0f );
		const float maxHeight	= boundingRectangle.height - ( DebugGui_DefaultMargin * 2.0f );

		const Vector2 minSize = getMinimumSize();
		const float scale = maxWidth / minSize.x;

		float currentX = boundingRectangle.x + DebugGui_DefaultMargin;
		for (uint childIndex = 0u; childIndex < getChildCount(); ++childIndex)
		{
			DebugGuiControl* pChild = getChildByIndex( childIndex );
			const Vector2 childMinSize = pChild->getMinimumSize();

			Rectangle childRect;
			childRect.x			= currentX;
			childRect.y			= y;
			childRect.width		= childMinSize.x * scale;
			childRect.height	= maxHeight;
			pChild->setRectangle( childRect );

			currentX += childRect.width + DebugGui_DefaultMargin;
		} 
	}

	Vector2 DebugGuiVerticalLayout::getMinimumSize()
	{
		Vector2 minSize = { DebugGui_DefaultMargin, 0.0f };
		for (uint childIndex = 0u; childIndex < getChildCount(); ++childIndex)
		{
			DebugGuiControl* pChild = getChildByIndex( childIndex );
			const Vector2 childMinSize = pChild->getMinimumSize();

			minSize.x += childMinSize.x + DebugGui_DefaultMargin;
			minSize.y = TIKI_MAX( minSize.y, childMinSize.y );
		}
		minSize.y += DebugGui_DefaultMargin * 2.0f;
		return minSize;
	}
}