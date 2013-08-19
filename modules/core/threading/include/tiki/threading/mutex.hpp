#pragma once
#ifndef TIKI_MUTEX_HPP
#define TIKI_MUTEX_HPP

#include "tiki/base/types.hpp"

#if TIKI_PLATFORM_WIN
#endif

#if TIKI_PLATFORM_WIN
#	include <windows.h>
#elif TIKI_PLATFORM_LINUX
#	include <pthread.h>
#endif

namespace tiki
{
	class Mutex
	{
	public:

		void				create();
		void				dispose();

		void				lock();
		bool				tryLock( size_t timeOut );
		void				unlock();

	private:

#if TIKI_PLATFORM_WIN
		CRITICAL_SECTION	m_mutex;
#elif TIKI_PLATFORM_LINUX
		pthread_mutex_t		m_mutex;
#else
#	error Platform not suported
#endif


	};
}

#endif // TIKI_MUTEX_HPP
