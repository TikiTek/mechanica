
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

	void DebugGuiMinimumLayout::handleRectangleChanged( const AxisAlignedRectangle& boundingRectangle )
	{
		const Vector2 minSize = getMinimumSize();

		for ( LinkedIterator< DebugGuiControl > it = getChildrenBegin(); it != getChildrenEnd(); ++it )
		{
			AxisAlignedRectangle rectangle = createAxisAlignedRectangle(
				boundingRectangle.getLeft(),
				boundingRectangle.getTop(),
				minSize.x,
				minSize.y
			);
			it->setRectangle( rectangle );
		}
	}

	Vector2 DebugGuiMinimumLayout::getMinimumSize()
	{
		Vector2 minSize = { 0.0f, 0.0f };

		for ( LinkedIterator< DebugGuiControl > it = getChildrenBegin(); it != getChildrenEnd(); ++it )
		{
			const Vector2 childMinSize = it->getMinimumSize();

			minSize.x = TIKI_MAX( minSize.x, childMinSize.x );
			minSize.y = TIKI_MAX( minSize.y, childMinSize.y );
		}

		return minSize;
	}

}