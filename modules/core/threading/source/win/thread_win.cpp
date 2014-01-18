
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
	}

	Thread::~Thread()
	{
		TIKI_ASSERT( m_platformData.threadHandle == INVALID_HANDLE_VALUE );
	}

	bool Thread::create( ThreadEntryFunction pEntryFunc, uint stackSize, const char* pName /*= nullptr*/ )
	{
		TIKI_ASSERT( pEntryFunc != nullptr );
		TIKI_ASSERT( m_platformData.threadHandle == INVALID_HANDLE_VALUE );

		m_pEntryFunction = pEntryFunc;

		m_platformData.threadHandle = CreateThread(
			nullptr,
			stackSize,
			threadEntryPoint,
			this,
			CREATE_SUSPENDED,
			&m_platformData.threadId
		);

		return false;
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

	void Thread::start( const void* pArgument )
	{
		TIKI_ASSERT( m_platformData.threadHandle != INVALID_HANDLE_VALUE );

		m_pArgument = pArgument;
		ResumeThread( m_platformData.threadHandle );
	}
}