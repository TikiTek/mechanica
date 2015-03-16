
#include "tiki/webinterface/httprequesthandler.hpp"

namespace tiki
{
	bool HttpRequestHandler::create( const Url& url )
	{
		if ( url.isValid() )
		{
			m_url = url;

			return true;
		}
		
		return false;
	}

	void HttpRequestHandler::dispose()
	{
		m_url.clear();
	}

	bool HttpRequestHandler::matchUrl( const Url& requestUrl ) const
	{
		return m_url.match( requestUrl );
	}
}