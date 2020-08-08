
#include "tiki/threading/thread.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/functions.hpp"

#include <pthread.h>
#include <errno.h>
#include <sys/syscall.h>
#include <unistd.h>

namespace tiki
{
	Thread::ThreadList Thread::s_threadList;

	Thread::Thread()
	{
		m_platformData.threadHandle	= 0u;
		m_platformData.threadId		= 0u;
		m_platformData.name[ 0u ]	= '\0';

		m_pEntryFunction				= nullptr;
		m_pArgument					= nullptr;
		m_isExitRequested				= false;

		s_threadList.push( this );
	}

	Thread::~Thread()
	{
		TIKI_ASSERT( m_platformData.threadHandle == 0u );

		s_threadList.removeSortedByValue( *this );
	}

	bool Thread::create( ThreadEntryFunction pEntryFunc, void* pArgument, uint stackSize /*= 0u*/, const char* pName /*= nullptr*/ )
	{
		TIKI_ASSERT( pEntryFunc != nullptr );
		TIKI_ASSERT( m_platformData.threadHandle == 0u );

		m_pEntryFunction	= pEntryFunc;
		m_pArgument		= pArgument;
		m_isExitRequested	= false;
		copyString( m_platformData.name, TIKI_COUNT( m_platformData.name ), pName );
		
		pthread_attr_t threadAttributes;
		if( pthread_attr_init( &threadAttributes ) < 0 )
		{			
			return false;
		}
		
		if( pthread_attr_setdetachstate( &threadAttributes, PTHREAD_CREATE_JOINABLE ) < 0 )
		{
			pthread_attr_destroy( &threadAttributes );
			return false;
		}
		
		if( stackSize > 0u )
		{
			const uint minStackSize = (uint)sysconf( _SC_THREAD_STACK_MIN );
			stackSize = TIKI_MAX( minStackSize, stackSize );
			
			const uint pageSize = (uint)sysconf( _SC_PAGESIZE );
			stackSize = alignValue( stackSize, pageSize );
			
			if( pthread_attr_setstacksize( &threadAttributes, stackSize ) < 0 )
			{
				pthread_attr_destroy( &threadAttributes );
				return false;
			}
		}

		const int result = pthread_create( &m_platformData.threadHandle, &threadAttributes, &threadEntryPoint, this );
		pthread_attr_destroy( &threadAttributes );
		
		if ( result < 0 )
		{
			return false;
		}
		
		TIKI_VERIFY( pthread_setname_np( m_platformData.threadHandle, m_platformData.name ) >= 0 );

		return true;
	}

	void Thread::dispose()
	{
		if ( m_platformData.threadHandle != 0u )
		{
			void* pExitCode = 0u;
			TIKI_VERIFY0( pthread_join( m_platformData.threadHandle, &pExitCode ) );
		}
		
		m_platformData.threadHandle	= 0u;
		m_platformData.threadId		= 0u;

		m_pEntryFunction				= nullptr;
		m_pArgument					= nullptr;
	}

	void Thread::requestExit()
	{
		TIKI_ASSERT( m_platformData.threadHandle != 0u );
		m_isExitRequested = true;
	}

	bool Thread::waitForExit( timems timeOut /*= TIKI_TIME_OUT_INFINITY*/ )
	{
		TIKI_ASSERT( m_platformData.threadHandle != 0u );
		TIKI_ASSERT( m_platformData.threadId != getCurrentThreadId() );

		timespec time;
		time.tv_sec	= timeOut / 1000000u;
		time.tv_nsec	= ( timeOut - ( time.tv_sec * 1000000u ) ) * 1000u;

		void* pExitCode = 0u;
		if ( pthread_timedjoin_np( m_platformData.threadHandle, &pExitCode, &time ) != 0 )
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
		return m_platformData.threadHandle != 0u;
	}

	/*static*/ uint64 Thread::getCurrentThreadId()
	{
		return syscall( SYS_gettid );
	}

	/*static*/ const Thread& Thread::getCurrentThread()
	{
		const Thread* pThread = getThreadById( getCurrentThreadId() );
		if ( pThread == nullptr )
		{
			Thread* pCurrentThread = TIKI_MEMORY_NEW_OBJECT( Thread );
			pCurrentThread->m_platformData.threadHandle	= pthread_self();
			pCurrentThread->m_platformData.threadId		= getCurrentThreadId();

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
		timespec sleepTime;
		sleepTime.tv_sec		= time / 1000000u;
		sleepTime.tv_nsec		= ( time - ( sleepTime.tv_sec * 1000000u ) ) * 1000u;

		int error = 0;
		timespec remainingTime;
		while( true )
		{
			const int result = nanosleep( &sleepTime, &remainingTime );
			if( result == -1 )
			{
				error = errno;
			}

			if( result == 0 || error != EINTR )
			{
				break;
			}

			sleepTime = remainingTime;
		}
	}

	void Thread::shutdownSystem()
	{
		while ( !s_threadList.isEmpty() )
		{
			LinkedIterator< Thread > it = s_threadList.getBegin();
			Thread& thread = *it;

			thread.dispose();
			TIKI_MEMORY_DELETE_OBJECT( &thread  );
		}
	}
	
	void* Thread::threadEntryPoint( void* pArgument )
	{
		TIKI_ASSERT( pArgument != nullptr );

		Thread* pThread = static_cast< Thread* >( pArgument );
		pThread->m_platformData.threadId = getCurrentThreadId();
		
		ThreadEntryFunction pEntryFunc = pThread->m_pEntryFunction;
		const uint result = pEntryFunc( *pThread );
		
		pthread_exit( (void*)result );
		
		return nullptr;
	}
}
