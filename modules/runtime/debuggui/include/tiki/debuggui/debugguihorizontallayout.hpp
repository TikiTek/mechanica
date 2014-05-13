#pragma once
#ifndef __TIKI_DEBUGGUIHORIZONTALLAYOUT_HPP_INCLUDED__
#define __TIKI_DEBUGGUIHORIZONTALLAYOUT_HPP_INCLUDED__

#include "tiki/debuggui/debugguilayout.hpp"

namespace tiki
{
	class DebugGuiHorizontalLayout : public DebugGuiLayout
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiHorizontalLayout );

	public:

		virtual Vector2	getMinimumSize();

	protected:

		virtual void	handleRectangleChanged( const Rectangle& boundingRectangle );

	};
}

#endif // __TIKI_DEBUGGUIHORIZONTALLAYOUT_HPP_INCLUDED__
