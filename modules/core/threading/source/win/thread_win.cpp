
#include "tiki/threading/thread.hpp"

#include "tiki/base/assert.hpp"

#include <windows.h>

namespace tiki
{
	DWORD WINAPI threadEntryPoint( void* pArgument )
	{
		TIKI_ASSERT( pArgument != nullptr );

		const Thread* pThread = static_cast< const Thread* >( pArgument );
		ThreadEntryFunction pEntryFunc = pThread->getEntryFunction();

		return pEntryFunc( *pThread );
	}

	Thread::Thread()
	{
		m_platformData.threadHandle	= INVALID_HANDLE_VALUE;
		m_platformData.threadId		= 0u;
		
		m_pEntryFunction	= nullptr;
		m_pArgument			= nullptr;
		m_isExitRequested	= false;
	}

	Thread::~Thread()
	{
		TIKI_ASSERT( m_platformData.threadHandle == INVALID_HANDLE_VALUE );
	}

	bool Thread::create( ThreadEntryFunction pEntryFunc, uint stackSize, const char* pName /*= nullptr*/ )
	{
		TIKI_ASSERT( pEntryFunc != nullptr );
		TIKI_ASSERT( m_platformData.threadHandle == INVALID_HANDLE_VALUE );

		m_pEntryFunction	= pEntryFunc;
		m_isExitRequested	= false;

		m_platformData.threadHandle = CreateThread(
			nullptr,
			stackSize,
			threadEntryPoint,
			this,
			CREATE_SUSPENDED,
			&m_platformData.threadId
		);
		
		return m_platformData.threadHandle != INVALID_HANDLE_VALUE;
	}

	void Thread::dispose()
	{
		if ( m_platformData.threadHandle != INVALID_HANDLE_VALUE )
		{
			CloseHandle( m_platformData.threadHandle );
		}
		m_platformData.threadHandle = INVALID_HANDLE_VALUE;
		m_platformData.threadId		= 0u;
		
		m_pEntryFunction	= nullptr;
		m_pArgument			= nullptr;
	}

	void Thread::start( void* pArgument )
	{
		TIKI_ASSERT( m_platformData.threadHandle != INVALID_HANDLE_VALUE );

		m_pArgument = pArgument;
		ResumeThread( m_platformData.threadHandle );
	}

	void Thread::requestExit()
	{
		TIKI_ASSERT( m_platformData.threadHandle != INVALID_HANDLE_VALUE );
		m_isExitRequested = true;
	}

	bool Thread::waitForExit( uint timeOut /*= TimeOutInfinity */ )
	{
		TIKI_ASSERT( m_platformData.threadHandle != INVALID_HANDLE_VALUE );
		if ( WaitForSingleObject( m_platformData.threadHandle, DWORD( timeOut ) ) == WAIT_TIMEOUT )
		{
			return false;
		}
		return true;
	}

	bool Thread::isCreated() const
	{
		return m_platformData.threadHandle != INVALID_HANDLE_VALUE;
	}
}