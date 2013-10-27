#pragma once
#ifndef TIKI_MUTEX_HPP
#define TIKI_MUTEX_HPP

#include "tiki/base/types.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include <windows.h>
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )
#	include <pthread.h>
#else
#	error Platform not suported
#endif

namespace tiki
{
#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
	struct MutexPlatformData
	{
		CRITICAL_SECTION mutex;
	};
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )
	struct MutexPlatformData
	{
		pthread_mutex_t mutex;
	};
#else
#	error Platform not suported
#endif

	class Mutex
	{
	public:

		void				create();
		void				dispose();

		void				lock();
		bool				tryLock( size_t timeOut );
		void				unlock();

	private:

		MutexPlatformData	m_platformData;

	};
}

#endif // TIKI_MUTEX_HPP
