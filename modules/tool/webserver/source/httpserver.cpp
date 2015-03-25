
#include "tiki/webserver/httpserver.hpp"

#include "tiki/base/string.hpp"
#include "tiki/webserver/httprequest.hpp"
#include "tiki/webserver/httprequesthandler.hpp"
#include "tiki/webserver/httpresponse.hpp"
#include "tiki/webserver/url.hpp"

#include "mongoose.h"

namespace tiki
{
	int connectionHandler( mg_connection* pcOnnection, mg_event eventType );

	HttpServer::HttpServer()
	{
		m_pServer = nullptr;
	}

	HttpServer::~HttpServer()
	{
		TIKI_ASSERT( m_pServer == nullptr );
	}

	bool HttpServer::create( uint16 port )
	{
		m_pServer = mg_create_server( this, connectionHandler );
		if ( m_pServer == nullptr )
		{
			return false; 
		}

		char portString[ 8u ];
		formatStringBuffer( portString, TIKI_COUNT( portString ), "%i", port );

		mg_set_option( m_pServer, "listening_port", portString );

		return true;
	}

	void HttpServer::dispose()
	{
		mg_destroy_server( &m_pServer );
	}

	void HttpServer::update()
	{
		mg_poll_server( m_pServer, 0 );
	}

	void HttpServer::registerRequestHandler( HttpRequestHandler& handler )
	{
		m_handler.push( handler );
	}

	void HttpServer::unregisterRequestHandler( HttpRequestHandler& handler )
	{
		m_handler.removeSortedByValue( handler );
	}

	int HttpServer::handleConnection( mg_connection* pConnection, mg_event eventType )
	{
		if ( eventType == MG_AUTH )
		{
			return MG_TRUE;   // Authorize all requests
		}
		else if ( eventType == MG_REQUEST )
		{
			HttpRequest request;
			if ( !request.url.createFromString( pConnection->uri ) )
			{
				return MG_FALSE;
			}
			request.pBody = pConnection->content;

			for ( HttpRequestHandler& requestHandler : m_handler )
			{
				if ( requestHandler.matchUrl( request.url ) )
				{
					HttpResponse response;
					requestHandler.handleRequest( response, request );					

					if ( response.statusCode == HttpStatusCode_200_OK )
					{
						mg_send_data( pConnection, response.content.cStr(), (int)response.content.getLength() );
						return MG_TRUE;
					}

					mg_printf_data(pConnection, "%s", "error");
					return MG_TRUE;
				}
			}

		}

		return MG_FALSE;
	}

	int connectionHandler( mg_connection* pConnection, mg_event eventType )
	{
		HttpServer* pServer = (HttpServer*)pConnection->server_param;
		return pServer->handleConnection( pConnection, eventType );
	}
}