#pragma once
#ifndef __TIKI_THREAD_HPP_INCLUDED__
#define __TIKI_THREAD_HPP_INCLUDED__

#include "tiki/base/types.hpp"

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

	class Thread
	{
		TIKI_NONCOPYABLE_CLASS( Thread );

	public:

		Thread();
		~Thread();

		bool				create( ThreadEntryFunction pEntryFunc, uint stackSize, const char* pName = nullptr );
		void				dispose();

		void				start( void* pArgument );
		void				requestExit();

		bool				waitForExit( uint timeOut = TimeOutInfinity );

		void*				getArgument() const			{ return m_pArgument; }
		ThreadEntryFunction	getEntryFunction() const	{ return m_pEntryFunction; }
		bool				isExitRequested() const		{ return m_isExitRequested; }

		bool				isCreated() const;

	private:

		ThreadPlatformData	m_platformData;

		ThreadEntryFunction	m_pEntryFunction;
		void*				m_pArgument;

		volatile bool		m_isExitRequested;

	};
}

#endif // __TIKI_THREAD_HPP_INCLUDED__
