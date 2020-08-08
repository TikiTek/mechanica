
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

	void Semaphore::incement()
	{
		TIKI_ASSERT( m_platformData.semaphoreHandle != INVALID_HANDLE_VALUE );
		ReleaseSemaphore( m_platformData.semaphoreHandle, 1, nullptr );
	}

	void Semaphore::decrement()
	{
		TIKI_ASSERT( m_platformData.semaphoreHandle != INVALID_HANDLE_VALUE );
		WaitForSingleObject( m_platformData.semaphoreHandle, INFINITE );
	}

	bool Semaphore::tryDecrement( timems timeOut /* = TIKI_TIME_OUT_INFINITY */ )
	{
		TIKI_ASSERT( m_platformData.semaphoreHandle != INVALID_HANDLE_VALUE );
		return WaitForSingleObject( m_platformData.semaphoreHandle, DWORD( timeOut ) ) != WAIT_TIMEOUT;
	}
}