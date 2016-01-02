#pragma once
#ifndef __TIKI_SEMAPHORE_HPP_INCLUDED__
#define __TIKI_SEMAPHORE_HPP_INCLUDED__

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
	class Semaphore
	{
		TIKI_NONCOPYABLE_CLASS( Semaphore );

	public:

		Semaphore();
		~Semaphore();

		bool	create( uint initialCount = 0, uint maxCount = 0x7fffffff, const char* pName = nullptr );
		void	dispose();

		void	incement();

		void	decrement();
		bool	tryDecrement( timems timeOut = TIKI_TIME_OUT_INFINITY );

	private:

		SemaphorePlatformData	m_platformData;

	};
}

#endif // __TIKI_SEMAPHORE_HPP_INCLUDED__
