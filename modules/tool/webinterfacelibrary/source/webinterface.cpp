
#include "webinterface.hpp"

#include "tiki/base/types.hpp"
#include "tiki/threading/thread.hpp"

#include "webrquesthandler.hpp"

namespace tiki
{
	IWebInterface* createWebInterface()
	{
		return TIKI_MEMORY_NEW_OBJECT( WebInterface );
	}

	void disposeWebInterface( IWebInterface* pObject )
	{
		TIKI_MEMORY_DELETE_OBJECT( pObject );
		Thread::shutdownSystem();
	}

	bool WebInterface::create()
	{
		if ( !m_server.create( 2156 ) )
		{
			return false;
		}

		return true;
	}

	void WebInterface::dispose()
	{
		m_server.dispose();
	}

	void WebInterface::update()
	{
		m_server.update();
	}

	void WebInterface::registerRequestHandler( cstring pUrlPath, IWebHandler* pHandler )
	{
		WebRquestHandler* pWebHandler = TIKI_MEMORY_NEW_OBJECT( WebRquestHandler );
		pWebHandler->create( pUrlPath, pHandler );

		m_server.registerRequestHandler( *pWebHandler );
	}

	void WebInterface::unregisterRequestHandler( IWebHandler* pHandler )
	{
		//m_server.unregisterRequestHandler( handler );
	}
}