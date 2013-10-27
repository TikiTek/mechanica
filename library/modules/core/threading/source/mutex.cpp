
#include "tiki/threading/mutex.hpp"

namespace tiki
{
#if TIKI_ENABLED( TIKI_PLATFORM_WIN )

	void Mutex::create()
	{
		InitializeCriticalSection( &m_platformData.mutex );
	}

	void Mutex::dispose()
	{
		DeleteCriticalSection( &m_platformData.mutex );
	}

	void Mutex::lock()
	{
		EnterCriticalSection( &m_platformData.mutex );
	}

	bool Mutex::tryLock( size_t timeOut )
	{
		return TryEnterCriticalSection( &m_platformData.mutex ) != 0;
	}

	void Mutex::unlock()
	{
		LeaveCriticalSection( &m_platformData.mutex );
	}

#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )

	void Mutex::create()
	{
		pthread_mutex_init( &m_mutex, nullptr );
	}

	void Mutex::dispose()
	{
		pthread_mutex_destroy( &m_mutex );
	}

	void Mutex::lock()
	{
		pthread_mutex_lock( &m_mutex );
	}

	bool Mutex::tryLock( size_t timeOut )
	{
		return pthread_mutex_trylock( &m_mutex ) != 0;
	}

	void Mutex::unlock()
	{
		pthread_mutex_unlock( &m_mutex );
	}

#else
#	error Platform not supported
#endif
}