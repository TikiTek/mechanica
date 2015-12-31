#pragma once
#ifndef __TIKI_THREADING_POSIX_HPP_INCLUDED__
#define __TIKI_THREADING_POSIX_HPP_INCLUDED__

#include <pthread.h>
#include <semaphore.h>

namespace tiki
{
	enum
	{
		TimeOutInfinity = 0xffffffffu
	};

	struct ThreadPlatformData
	{
		pthread_t		threadHandle;
	};

	struct MutexPlatformData
	{
		bool				isInitialized;
		pthread_mutex_t	mutexData;
	};

	struct SemaphorePlatformData
	{
		bool		isInitialized;
		sem_t		semaphoreData;
	};

	struct EventPlatformData
	{
	};
}

#endif // __TIKI_THREADING_POSIX_HPP_INCLUDED__
