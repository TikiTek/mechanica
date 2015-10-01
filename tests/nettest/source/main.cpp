
#include "tiki/base/platform.hpp"

#include "tiki/net/httpclient.hpp"
#include "tiki/net/httprequest.hpp"
#include "tiki/net/httpresponse.hpp"

namespace tiki
{
	int mainEntryPoint()
	{
		HttpClient client;
		if ( !client.create() )
		{
			return -1;
		}

		HttpRequest request;
		request.url.createFromString( "http://www.timboden.de/" );

		const HttpResponse* pResponse = client.startRequest( request );

		while ( !pResponse->isReady )
		{
			client.update();
		}

		const string body = string( (const char*)pResponse->bodyStream.getData(), pResponse->bodyStream.getLength() );

		debug::trace( "Response Code: %i ", pResponse->reponseCode );
		debug::trace( body.cStr() );

		client.releaseRequest( pResponse );
		client.dispose();

		return 0;
	}
}