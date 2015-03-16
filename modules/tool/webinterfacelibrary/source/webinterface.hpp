#pragma once
#ifndef TIKI_WEBINTERFACE_HPP_INCLUDED__
#define TIKI_WEBINTERFACE_HPP_INCLUDED__

#include "tiki/toollibraries/iwebinterrface.hpp"

#include "tiki/webinterface/httpserver.hpp"

namespace tiki
{
	class WebInterface : public IWebInterface
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( WebInterface );

	public:

		virtual bool		create() TIKI_OVERRIDE;
		virtual void		dispose() TIKI_OVERRIDE;

		virtual void		update() TIKI_OVERRIDE;

	private:

		HttpServer			m_server;

	};
}

#endif // TIKI_WEBINTERFACE_HPP_INCLUDED__
