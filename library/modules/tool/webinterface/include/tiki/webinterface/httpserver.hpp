#pragma once
#ifndef TIKI_HTTPSERVER_HPP_INCLUDED__
#define TIKI_HTTPSERVER_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/base/linkedlist.hpp"

enum mg_event;
struct mg_connection;
struct mg_server;

namespace tiki
{
	class HttpRequestHandler;

	class HttpServer
	{
		TIKI_NONCOPYABLE_CLASS( HttpServer );
		friend int connectionHandler( mg_connection* pConnection, mg_event eventType );

	public:

				HttpServer();
				~HttpServer();

		bool	create( uint16 port );
		void	dispose();

		void	update();

		void	registerRequestHandler( HttpRequestHandler& handler );
		void	unregisterRequestHandler( HttpRequestHandler& handler );

	private:

		mg_server*						m_pServer;

		LinkedList<HttpRequestHandler>	m_handler;

		int								handleConnection( mg_connection* pConnection, mg_event eventType );

	};
}

#endif // TIKI_HTTPSERVER_HPP_INCLUDED__
