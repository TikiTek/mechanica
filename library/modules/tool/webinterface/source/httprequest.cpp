
#include "tiki/webinterface/httprequest.hpp"

namespace tiki
{
	bool HttpRequest::create( cstring pUrlString, cstring pBodyString )
	{
		if ( !m_url.createFromString( pUrlString ) )
		{
			return false;
		}

		m_body = pBodyString;

		return true;
	}

	void HttpRequest::dispose()
	{
		m_url.clear();
		m_body = nullptr;
	}
}
