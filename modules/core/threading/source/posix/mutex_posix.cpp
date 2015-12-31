
#include "tiki/threading/mutex.hpp"

#include "tiki/base/assert.hpp"

#include <pthread.h>

namespace tiki
{
	Mutex::Mutex()
	{
		m_platformData.isInitialized = false;
	}

	Mutex::~Mutex()
	{
		TIKI_ASSERT( !m_platformData.isInitialized );
	}

	bool Mutex::create()
	{
		TIKI_ASSERT( !m_platformData.isInitialized );

		const int result = pthread_mutex_init( &m_platformData.mutexData, nullptr );
		if ( result < 0 )
		{
			TIKI_TRACE_ERROR( "[threading] Unable to initialize Mutex.\n" );
			return false;
		}

		m_platformData.isInitialized = true;
		
		return true;
	}

	void Mutex::dispose()
	{
		if ( m_platformData.isInitialized )
		{
			m_platformData.isInitialized = false;
			TIKI_VERIFY( pthread_mutex_destroy( &m_platformData.mutexData ) >= 0 );
		}
	}

	void Mutex::lock()
	{
		TIKI_ASSERT( m_platformData.isInitialized );
		TIKI_VERIFY( pthread_mutex_lock( &m_platformData.mutexData ) >= 0 );
	}

	bool Mutex::tryLock( uint timeOut /*= TimeOutInfinity*/ )
	{
		TIKI_ASSERT( m_platformData.isInitialized );

		timespec time;
		time.tv_sec	= timeOut / 1000;
		time.tv_nsec	= (timeOut - (time.tv_sec * 1000)) * 1000000;
		
		return pthread_mutex_timedlock( &m_platformData.mutexData, &time ) == 0;
	}

	void Mutex::unlock()
	{
		TIKI_ASSERT( m_platformData.isInitialized );
		TIKI_VERIFY( pthread_mutex_unlock( &m_platformData.mutexData ) >= 0 );
	}
}