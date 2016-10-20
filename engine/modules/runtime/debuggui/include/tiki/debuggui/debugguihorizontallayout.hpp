#pragma once
#ifndef __TIKI_DEBUGGUIHORIZONTALLAYOUT_HPP_INCLUDED__
#define __TIKI_DEBUGGUIHORIZONTALLAYOUT_HPP_INCLUDED__

#include "tiki/debuggui/debugguilayout.hpp"

namespace tiki
{
	class DebugGuiHorizontalLayout : public DebugGuiLayout
	{
		TIKI_NONCOPYABLE_CLASS( DebugGuiHorizontalLayout );

	public:

		DebugGuiHorizontalLayout();

		virtual Vector2	getMinimumSize();

		bool			getExpandChildren() const;
		void			setExpandChildren( bool value );

	protected:

		virtual void	handleRectangleChanged( const AxisAlignedRectangle& boundingRectangle );

	private:

		bool			m_expandChildren;

	};
}

#endif // __TIKI_DEBUGGUIHORIZONTALLAYOUT_HPP_INCLUDED__
