#pragma once
#ifndef TIKI_IWEBINTERRFACE_HPP_INCLUDED__
#define TIKI_IWEBINTERRFACE_HPP_INCLUDED__

#include "tiki/toollibraries/librarybase.hpp"

namespace tiki
{
	class IWebInterface
	{
	public:

		virtual bool		create() = 0;
		virtual void		dispose() = 0;

		virtual void		update() = 0;

	};

	TIKI_LIBRARY_METHOD IWebInterface*	createWebInterface();
	TIKI_LIBRARY_METHOD void			disposeWebInterface( IWebInterface* pObject );
}

#endif // TIKI_IWEBINTERRFACE_HPP_INCLUDED__
