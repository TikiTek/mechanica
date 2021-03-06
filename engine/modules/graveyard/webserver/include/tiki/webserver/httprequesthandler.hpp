#pragma once
#ifndef TIKI_HTTPREQUESTHANDLER_HPP_INCLUDED__
#define TIKI_HTTPREQUESTHANDLER_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/container/linkedlist.hpp"
#include "tiki/net/url.hpp"

namespace tiki
{
	struct HttpRequest;
	struct HttpResponse;

	class HttpRequestHandler : public LinkedItem< HttpRequestHandler >
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( HttpRequestHandler );

	public:

		bool			matchUrl( const Url& requestUrl ) const;

		virtual	void	handleRequest( HttpResponse& response, const HttpRequest& request ) = 0;

	protected:

		bool			create( const Url& url );
		void			dispose();

	private:

		Url				m_url;

	};
}

#endif // TIKI_HTTPREQUESTHANDLER_HPP_INCLUDED__
