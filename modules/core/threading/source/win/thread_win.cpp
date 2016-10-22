
#include "tiki/threading/thread.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/string.hpp"

#include <windows.h>

const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD	dwType;			// Must be 0x1000.
	LPCSTR	szName;			// Pointer to name (in user addr space).
	DWORD	dwThreadID;		// Thread ID (-1=caller thread).
	DWORD	dwFlags;		// Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

namespace tiki
{
	Thread::ThreadList Thread::s_threadList;

	Thread::Thread()
	{
		m_platformData.threadHandle	= INVALID_HANDLE_VALUE;
		m_platformData.threadId		= 0u;
		m_platformData.name[ 0u ]	= '\0';

		m_pEntryFunction	= nullptr;
		m_pArgument			= nullptr;
		m_isExitRequested	= false;

		s_threadList.push( this );
	}

	Thread::~Thread()
	{
		TIKI_ASSERT( m_platformData.threadHandle == INVALID_HANDLE_VALUE );

		s_threadList.removeSortedByValue( *this );
	}

	bool Thread::create( ThreadEntryFunction pEntryFunc, void* pArgument, uint stackSize /* = 0u */, const char* pName /* = nullptr */ )
	{
		TIKI_ASSERT( pEntryFunc != nullptr );
		TIKI_ASSERT( m_platformData.threadHandle == INVALID_HANDLE_VALUE );

		m_pEntryFunction	= pEntryFunc;
		m_pArgument			= pArgument;
		m_isExitRequested	= false;

		m_platformData.threadHandle = CreateThread(
			nullptr,
			stackSize,
			threadEntryPoint,
			this,
			0,
			&m_platformData.threadId
		);

		if ( pName != nullptr )
		{
			copyString( m_platformData.name, TIKI_COUNT( m_platformData.name ), pName );
		}

		if( getStringSize( m_platformData.name ) > 0u )
		{
			THREADNAME_INFO info;
			info.dwType		= 0x1000;
			info.szName		= m_platformData.name;
			info.dwThreadID = m_platformData.threadId;
			info.dwFlags	= 0;

#if TIKI_ENABLED( TIKI_BUILD_MSVC )
			__try
			{
				RaiseException( MS_VC_EXCEPTION, 0, sizeof( info ) / sizeof( ULONG_PTR ), (ULONG_PTR*)&info );
			}
			__except( EXCEPTION_EXECUTE_HANDLER )
			{
			}
#endif
		}

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

	void Thread::requestExit()
	{
		TIKI_ASSERT( m_platformData.threadHandle != INVALID_HANDLE_VALUE );
		m_isExitRequested = true;
	}

	bool Thread::waitForExit( timems timeOut /* = TIKI_TIME_OUT_INFINITY */ )
	{
		TIKI_ASSERT( m_platformData.threadHandle != INVALID_HANDLE_VALUE );
		TIKI_ASSERT( m_platformData.threadId != getCurrentThreadId() );

		if ( WaitForSingleObject( m_platformData.threadHandle, DWORD( timeOut ) ) == WAIT_TIMEOUT )
		{
			return false;
		}
		return true;
	}

	uint64 Thread::getThreadId() const
	{
		return m_platformData.threadId;
	}

	bool Thread::isCreated() const
	{
		return m_platformData.threadHandle != INVALID_HANDLE_VALUE;
	}

	/*static*/ uint64 Thread::getCurrentThreadId()
	{
		return GetCurrentThreadId();
	}

	/*static*/ const Thread& Thread::getCurrentThread()
	{
		const Thread* pThread = getThreadById( getCurrentThreadId() );
		if ( pThread == nullptr )
		{
			Thread* pCurrentThread = TIKI_NEW( Thread );
			pCurrentThread->m_platformData.threadHandle	= GetCurrentThread();
			pCurrentThread->m_platformData.threadId		= GetCurrentThreadId();

			pThread = pCurrentThread;
		}

		return *pThread;
	}

	/*static*/ const Thread* Thread::getThreadById( uint64 threadId )
	{
		const uint64 currentThreadId = getCurrentThreadId();

		for (LinkedIterator< Thread > it = s_threadList.getBegin(); it != s_threadList.getEnd(); ++it)
		{
			if (it->getThreadId() == currentThreadId)
			{
				return &*it;
			}
		}

		return nullptr;
	}

	void Thread::sleepCurrentThread( timems time )
	{
		Sleep( DWORD( time / 1000 ) );
	}

	void Thread::shutdownSystem()
	{
		while ( !s_threadList.isEmpty() )
		{
			LinkedIterator< Thread > it = s_threadList.getBegin();
			Thread& thread = *it;

			thread.dispose();
			TIKI_DELETE( &thread  );
		}
	}

	DWORD WINAPI Thread::threadEntryPoint( void* pArgument )
	{
		TIKI_ASSERT( pArgument != nullptr );

		const Thread* pThread = static_cast<const Thread*>(pArgument);

		ThreadEntryFunction pEntryFunc = pThread->m_pEntryFunction;
		return pEntryFunc( *pThread );
	}
}
