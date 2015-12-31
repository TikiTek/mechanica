
#include "tiki/threading/semaphore.hpp"

#include "tiki/base/assert.hpp"

#include <windows.h>

namespace tiki
{
	Semaphore::Semaphore()
	{
		m_platformData.semaphoreHandle = INVALID_HANDLE_VALUE;
	}

	Semaphore::~Semaphore()
	{
		TIKI_ASSERT( m_platformData.semaphoreHandle == INVALID_HANDLE_VALUE );
	}

	bool Semaphore::create( uint initialCount /*= 0*/, uint maxCount /*= 0x7fffffff*/, const char* pName /*= nullptr*/ )
	{
		TIKI_ASSERT( m_platformData.semaphoreHandle == INVALID_HANDLE_VALUE );
		m_platformData.semaphoreHandle = CreateSemaphoreA(
			nullptr,
			DWORD( initialCount ),
			DWORD( maxCount ),
			pName
		);

		if (m_platformData.semaphoreHandle == nullptr)
		{
			m_platformData.semaphoreHandle = INVALID_HANDLE_VALUE;
			return false;
		}

		return true;
	}

	void Semaphore::dispose()
	{
		if ( m_platformData.semaphoreHandle != INVALID_HANDLE_VALUE )
		{
			CloseHandle( m_platformData.semaphoreHandle );
			m_platformData.semaphoreHandle = INVALID_HANDLE_VALUE;
		}
	}

	void Semaphore::incement( uint count /*= 1u*/ )
	{
		TIKI_ASSERT( m_platformData.semaphoreHandle != INVALID_HANDLE_VALUE );

		ReleaseSemaphore(
			m_platformData.semaphoreHandle,
			DWORD( count ),
			nullptr
		);
	}

	void Semaphore::decrement( uint count /*= 1u*/ )
	{
		TIKI_ASSERT( m_platformData.semaphoreHandle != INVALID_HANDLE_VALUE );

		for (uint i = 0u; i < count; ++i)
		{
			WaitForSingleObject( m_platformData.semaphoreHandle, INFINITE );
		} 
	}

	uint Semaphore::tryDecrement( uint count /*= 1u*/, uint timeOut /*= TimeOutInfinity */ )
	{
		TIKI_ASSERT( m_platformData.semaphoreHandle != INVALID_HANDLE_VALUE );

		for (uint i = 0u; i < count; ++i)
		{
			if ( WaitForSingleObject( m_platformData.semaphoreHandle, DWORD( timeOut ) ) == WAIT_TIMEOUT )
			{
				return i;
			}
		} 

		return count;
	}
}