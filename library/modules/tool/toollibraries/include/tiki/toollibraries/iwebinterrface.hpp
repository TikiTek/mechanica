#pragma once
#ifndef TIKI_IWEBINTERRFACE_HPP_INCLUDED__
#define TIKI_IWEBINTERRFACE_HPP_INCLUDED__

#include "tiki/toollibraries/librarybase.hpp"

namespace tiki
{
	class IWebHandler;

	class IWebInterface
	{
	public:

		virtual bool		create() = 0;
		virtual void		dispose() = 0;

		virtual void		update() = 0;

		virtual void		registerRequestHandler( const char* url, IWebHandler* pHandler ) = 0;
		virtual void		unregisterRequestHandler( IWebHandler* pHandler ) = 0;

	};

	TIKI_LIBRARY_METHOD IWebInterface*	createWebInterface();
	TIKI_LIBRARY_METHOD void			disposeWebInterface( IWebInterface* pObject );
}

#endif // TIKI_IWEBINTERRFACE_HPP_INCLUDED__
