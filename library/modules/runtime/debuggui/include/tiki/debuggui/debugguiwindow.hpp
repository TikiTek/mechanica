#pragma once
#ifndef __TIKI_DEBUGMENUPAGE_HPP_INCLUDED__
#define __TIKI_DEBUGMENUPAGE_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	class DebugMenuPage
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( DebugMenuPage );

	public:

		virtual const char*		getTitle() const = 0;

		virtual void			setRectangle( const Rectangle& boundingRectangle ) = 0;

		virtual void			update() = 0;
		virtual void			render( const ImmediateRenderer& renderer ) = 0;

	};
}

#endif // __TIKI_DEBUGMENUPAGE_HPP_INCLUDED__
