#pragma once
#ifndef __TIKI_THREAD_HPP_INCLUDED__
#define __TIKI_THREAD_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/container/linkedlist.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include "../../../source/win/platformdata_win.hpp"
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )
#	include "../../../source/posix/platformdata_posix.hpp"
#else
#	error not supported
#endif

namespace tiki
{
	class Thread;

	typedef int(*ThreadEntryFunction)(const Thread&);

	class Thread : public LinkedItem< Thread >
	{
		TIKI_NONCOPYABLE_CLASS( Thread );

	public:

		Thread();
		~Thread();

		bool					create( ThreadEntryFunction pEntryFunc, void* pArgument, uint stackSize = 0u, const char* pName = nullptr );
		void					dispose();

		void					requestExit();

		bool					waitForExit( timems timeOut = TIKI_TIME_OUT_INFINITY );

		void*					getArgument() const			{ return m_pArgument; }
		bool					isExitRequested() const		{ return m_isExitRequested; }

		uint64					getThreadId() const;
		bool					isCreated() const;

		static uint64			getCurrentThreadId(); 
		static const Thread&	getCurrentThread();
		static const Thread*	getThreadById( uint64 threadId );

		static void			sleepCurrentThread( timems time );

		static void			shutdownSystem();

	private:

		typedef LinkedList< Thread > ThreadList;

		ThreadPlatformData	m_platformData;

		ThreadEntryFunction	m_pEntryFunction;
		void*					m_pArgument;

		volatile bool			m_isExitRequested;

		static ThreadList		s_threadList;
		
#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
		static DWORD WINAPI		threadEntryPoint( void* pArgument );
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )
		static void*			threadEntryPoint( void* pArgument );
#endif

	};
}

#endif // __TIKI_THREAD_HPP_INCLUDED__
