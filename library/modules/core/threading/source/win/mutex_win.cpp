
#include "tiki/threading/mutex.hpp"

#include "tiki/base/assert.hpp"

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

	void Mutex::create()
	{
		TIKI_ASSERT( !m_platformData.isInitialized );

		m_platformData.isInitialized = true;
		InitializeCriticalSection( &m_platformData.mutexData );
	}

	void Mutex::dispose()
	{
		if ( m_platformData.isInitialized )
		{
			m_platformData.isInitialized = false;
			DeleteCriticalSection( &m_platformData.mutexData );
		}
	}

	void Mutex::lock()
	{
		TIKI_ASSERT( m_platformData.isInitialized );
		EnterCriticalSection( &m_platformData.mutexData );
	}

	bool Mutex::tryLock( uint timeOut /*= TimeOutInfinity*/ )
	{
		TIKI_ASSERT( m_platformData.isInitialized );
		return TryEnterCriticalSection( &m_platformData.mutexData ) != 0;
	}

	void Mutex::unlock()
	{
		TIKI_ASSERT( m_platformData.isInitialized );
		LeaveCriticalSection( &m_platformData.mutexData );
	}
}