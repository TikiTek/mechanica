#pragma once
#ifndef TIKI_MUTEX_HPP
#define TIKI_MUTEX_HPP

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
//#if TIKI_ENABLED( TIKI_PLATFORM_LINUX )
//	struct MutexPlatformData
//	{
//		pthread_mutex_t mutex;
//	};
//#endif


	class Mutex
	{
		TIKI_NONCOPYABLE_CLASS( Mutex );

	public:

		Mutex();
		~Mutex();

		void				create();
		void				dispose();

		void				lock();
		bool				tryLock( uint timeOut = TimeOutInfinity );

		void				unlock();

	private:

		MutexPlatformData	m_platformData;

	};
}

#endif // TIKI_MUTEX_HPP
