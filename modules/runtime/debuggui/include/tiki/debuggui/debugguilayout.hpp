#pragma once
#ifndef __TIKI_DEBUGGUILAYOUT_HPP_INCLUDED__
#define __TIKI_DEBUGGUILAYOUT_HPP_INCLUDED__

#include "tiki/debuggui/debugguicontrol.hpp"

#include "tiki/container/linkedlist.hpp"

namespace tiki
{
	class DebugGuiLayout : public DebugGuiControl
	{
		TIKI_NONCOPYABLE_CLASS( DebugGuiLayout );

	public:

		void			create();
		void			dispose();

		virtual void	addChildControl( DebugGuiControl* pChild );
		virtual void	removeChildControl( DebugGuiControl* pChild );

		virtual void	update( double elapsedTime ) TIKI_OVERRIDE;
		virtual void	render( ImmediateRenderer& renderer ) TIKI_OVERRIDE;

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state ) TIKI_OVERRIDE;

		uint			getChildCount() const;

	protected:

											DebugGuiLayout();
		virtual								~DebugGuiLayout();
		
		LinkedIterator< DebugGuiControl >	getChildrenBegin();
		LinkedIterator< DebugGuiControl >	getChildrenEnd();

	private:

		LinkedList< DebugGuiControl >		m_childControls;

	};
}

#endif // __TIKI_DEBUGGUILAYOUT_HPP_INCLUDED__
