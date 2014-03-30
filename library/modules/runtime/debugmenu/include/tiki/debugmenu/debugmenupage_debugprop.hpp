#pragma once
#ifndef __TIKI_DEBUGMENUPAGE_DEBUGPROP_HPP_INCLUDED__
#define __TIKI_DEBUGMENUPAGE_DEBUGPROP_HPP_INCLUDED__

#include "tiki/debugmenu/debugmenupage.hpp"

namespace tiki
{
	class DebugPropManager;

	class DebugMenuPageDebugProp : public DebugMenuPage
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugMenuPageDebugProp );

	public:

		void				create( DebugPropManager& debugPropManager );
		void				dispose();

		virtual const char*	getTitle() const { return "Debug Properties"; }

		virtual void		setRectangle( const Rectangle& boundingRectangle );

		virtual void		update();
		virtual void		render( const ImmediateRenderer& renderer );

	private:

	};
}

#endif // __TIKI_DEBUGMENUPAGE_DEBUGPROP_HPP_INCLUDED__
