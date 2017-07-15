#pragma once
#ifndef TIKI_WEBRQUESTHANDLER_HPP_INCLUDED__
#define TIKI_WEBRQUESTHANDLER_HPP_INCLUDED__

#include "tiki/webserver/httprequesthandler.hpp"

namespace tiki
{
	class IWebHandler;

	class WebRquestHandler : public HttpRequestHandler
	{
		TIKI_NONCOPYABLE_CLASS( WebRquestHandler );

	public:

							WebRquestHandler();
		virtual				~WebRquestHandler();

		void				create( const char* pUrlPath, IWebHandler* pHandler );
		void				dispose();

		virtual	void		handleRequest( HttpResponse& response, const HttpRequest& request ) TIKI_OVERRIDE;

	private:

		IWebHandler*		m_pHandler;

	};
}

#endif // TIKI_WEBRQUESTHANDLER_HPP_INCLUDED__
