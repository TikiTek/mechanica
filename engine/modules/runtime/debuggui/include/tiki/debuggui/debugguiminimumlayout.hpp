#pragma once
#ifndef __TIKI_DEBUGGUIMINIMUMLAYOUT_HPP_INCLUDED__
#define __TIKI_DEBUGGUIMINIMUMLAYOUT_HPP_INCLUDED__

#include "tiki/debuggui/debugguilayout.hpp"

namespace tiki
{
	class DebugGuiMinimumLayout : public DebugGuiLayout
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiMinimumLayout );

	public:

		virtual void	addChildControl( DebugGuiControl* pChild );

		virtual Vector2	getMinimumSize();

	protected:

		virtual void	handleRectangleChanged( const AxisAlignedRectangle& boundingRectangle );

	};
}

#endif // __TIKI_DEBUGGUIMINIMUMLAYOUT_HPP_INCLUDED__
