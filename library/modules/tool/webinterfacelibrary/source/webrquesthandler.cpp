
#include "webrquesthandler.hpp"

#include "tiki/toollibraries/iwebhandler.hpp"
#include "tiki/webserver/httprequest.hpp"
#include "tiki/webserver/httpresponse.hpp"

namespace tiki
{
	WebRquestHandler::WebRquestHandler()
	{
		m_pHandler = nullptr;
	}

	WebRquestHandler::~WebRquestHandler()
	{
		TIKI_ASSERT( m_pHandler == nullptr );
	}

	void WebRquestHandler::create( cstring pUrlPath, IWebHandler* pHandler )
	{
		TIKI_ASSERT( pHandler != nullptr );

		m_pHandler = pHandler;

		Url url;
		url.createFromString( "http://*/");
		url.setPath( pUrlPath );

		HttpRequestHandler::create( url );
	}

	void WebRquestHandler::dispose()
	{
		m_pHandler = nullptr;

		HttpRequestHandler::dispose();
	}

	void WebRquestHandler::handleRequest( HttpResponse& response, const HttpRequest& request )
	{
		const string& urlPath = request.url.getPath();

		if ( m_pHandler->handleRequest( response.content, urlPath.cStr() ) )
		{
			response.statusCode = HttpStatusCode_200_OK;
		}
	}
}