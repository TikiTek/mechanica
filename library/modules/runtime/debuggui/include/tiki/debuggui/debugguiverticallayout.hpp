#pragma once
#ifndef __TIKI_DEBUGGUIVERTICALLAYOUT_HPP_INCLUDED__
#define __TIKI_DEBUGGUIVERTICALLAYOUT_HPP_INCLUDED__

#include "tiki/debuggui/debugguilayout.hpp"

namespace tiki
{
	class DebugGuiVerticalLayout : public DebugGuiLayout
	{
		TIKI_NONCOPYABLE_CLASS( DebugGuiVerticalLayout );

	public:

		DebugGuiVerticalLayout();

		virtual Vector2	getMinimumSize();

		bool			getExpandChildren() const;
		void			setExpandChildren( bool value );

	protected:

		virtual void	handleRectangleChanged( const Rectangle& boundingRectangle );

	private:

		bool			m_expandChildren;

	};
}

#endif // __TIKI_DEBUGGUIVERTICALLAYOUT_HPP_INCLUDED__
