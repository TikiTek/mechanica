#pragma once
#ifndef __TIKI_DEBUGGUILAYOUT_HPP_INCLUDED__
#define __TIKI_DEBUGGUILAYOUT_HPP_INCLUDED__

#include "tiki/debuggui/debugguicontrol.hpp"

#include "tiki/base/sizedarray.hpp"

namespace tiki
{
	class DebugGuiLayout : public DebugGuiControl
	{
		TIKI_NONCOPYABLE_CLASS( DebugGuiLayout );

	public:

		virtual void	addChildControl( DebugGuiControl* pChild );
		virtual bool	removeChildControl( DebugGuiControl* pChild );

		virtual void	update();
		virtual void	render( ImmediateRenderer& renderer );

		virtual bool	processInputEvent( const InputEvent& inputEvent, const DebugGuiInputState& state );

	protected:

		DebugGuiLayout();
		~DebugGuiLayout();
		
		uint				getChildCount() const;
		DebugGuiControl*	getChildByIndex( uint index ) const;

	private:

		SizedArray< DebugGuiControl* >	m_childControls;

	};
}

#endif // __TIKI_DEBUGGUILAYOUT_HPP_INCLUDED__
