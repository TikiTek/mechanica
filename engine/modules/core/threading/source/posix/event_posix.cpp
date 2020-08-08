#include "tiki/threading/event.hpp"

#include "tiki/base/assert.hpp"

namespace tiki
{
	Event::Event()
	{
		m_platformData.isInitialized = false;
	}

	Event::~Event()
	{
		TIKI_ASSERT( m_platformData.isInitialized == false );
	}

	bool Event::create( bool initialState /*= false*/, bool manualReset /*= false */, const char* pName /*= nullptr*/ )
	{
		m_platformData.isInitialized			= true;
		m_platformData.isSignaled			= initialState;
		m_platformData.waitingThreadCount	= 0u;
		
		if ( pthread_mutex_init( &m_platformData.signalMutex, nullptr ) < 0 ||
			 pthread_cond_init( &m_platformData.condition, nullptr ) < 0 )
		{
			dispose();
			return false;
		}
		
		return true;
	}

	void Event::dispose()
	{
		if ( m_platformData.isInitialized == false )
		{
			return;
		}
		
		TIKI_ASSERT( m_platformData.waitingThreadCount == 0u );
		
		pthread_cond_destroy( &m_platformData.condition );
		pthread_mutex_destroy( &m_platformData.signalMutex );
		
		m_platformData.isInitialized = false;
	}

	void Event::signal()
	{
		TIKI_ASSERT( m_platformData.isInitialized );
		
		TIKI_VERIFY( pthread_mutex_lock( &m_platformData.signalMutex ) >= 0 );
		if ( !m_platformData.isSignaled )
		{
			m_platformData.isSignaled = true;
			if ( m_platformData.waitingThreadCount > 0u )
			{
				TIKI_VERIFY( pthread_cond_signal( &m_platformData.condition ) );			
			}
		}
		TIKI_VERIFY( pthread_mutex_unlock( &m_platformData.signalMutex ) >= 0 );
	}

	void Event::reset()
	{
	}

	bool Event::waitForSignal( timems timeOut /*= TIKI_TIME_OUT_INFINITY*/ )
	{
	}
}