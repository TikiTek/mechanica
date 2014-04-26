#pragma once
#ifndef __TIKI_DEBUGGUIVERTICALLAYOUT_HPP_INCLUDED__
#define __TIKI_DEBUGGUIVERTICALLAYOUT_HPP_INCLUDED__

#include "tiki/debuggui/debugguilayout.hpp"

namespace tiki
{
	class DebugGuiVerticalLayout : public DebugGuiLayout
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugGuiVerticalLayout );

	public:

		virtual Vector2	getMinimumSize();

	protected:

		virtual void	handleRectangleChanged( const Rectangle& boundingRectangle );

	};
}

#endif // __TIKI_DEBUGGUIVERTICALLAYOUT_HPP_INCLUDED__
