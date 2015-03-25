
#include "webinterface.hpp"

#include "tiki/base/reflection.hpp"
#include "tiki/base/types.hpp"
#include "tiki/threading/thread.hpp"

#include "webrquesthandler.hpp"

namespace tiki
{
	IWebInterface* createWebInterface()
	{
		reflection::initialize();
		return TIKI_NEW WebInterface();
	}

	void disposeWebInterface( IWebInterface* pObject )
	{
		TIKI_DEL pObject;
		reflection::shutdown();
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
		WebRquestHandler* pWebHandler = TIKI_NEW WebRquestHandler();
		pWebHandler->create( pUrlPath, pHandler );

		m_server.registerRequestHandler( *pWebHandler );
	}

	void WebInterface::unregisterRequestHandler( IWebHandler* pHandler )
	{
		//m_server.unregisterRequestHandler( handler );
	}
}