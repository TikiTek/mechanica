
#include "tiki/threading/semaphore.hpp"

#include "tiki/base/assert.hpp"

#include <semaphore.h>

namespace tiki
{
	Semaphore::Semaphore()
	{
		m_platformData.isInitialized = false;
	}

	Semaphore::~Semaphore()
	{
		TIKI_ASSERT( m_platformData.isInitialized == false );
	}

	bool Semaphore::create( uint initialCount /*= 0*/, uint maxCount /*= 0x7fffffff*/, const char* pName /*= nullptr*/ )
	{
		TIKI_ASSERT( m_platformData.isInitialized == false );		
		return sem_init( &m_platformData.semaphoreData, 0, initialCount );
	}

	void Semaphore::dispose()
	{
		if ( m_platformData.isInitialized )
		{
			sem_destroy( &m_platformData.semaphoreData );
			m_platformData.isInitialized = false;;
		}
	}

	void Semaphore::incement()
	{
		TIKI_ASSERT( m_platformData.isInitialized );
		TIKI_VERIFY( sem_post( &m_platformData.semaphoreData ) >= 0 );
	}

	void Semaphore::decrement()
	{
		TIKI_ASSERT( m_platformData.isInitialized );
		TIKI_VERIFY( sem_wait( &m_platformData.semaphoreData ) >= 0 );
	}

	bool Semaphore::tryDecrement( timems timeOut /*= TIKI_TIME_OUT_INFINITY*/ )
	{
		TIKI_ASSERT( m_platformData.isInitialized );
		if ( timeOut == TIKI_TIME_OUT_INFINITY )
		{
			return sem_trywait( &m_platformData.semaphoreData ) >= 0;
		}
		else
		{
			timespec time;
			time.tv_sec	= timeOut / 1000;
			time.tv_nsec	= (timeOut - (time.tv_sec * 1000)) * 1000000;
			
			return sem_timedwait( &m_platformData.semaphoreData, &time ) >= 0;
		}
	}
}