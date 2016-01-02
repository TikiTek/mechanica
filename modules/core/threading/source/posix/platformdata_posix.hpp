#pragma once
#ifndef __TIKI_THREADING_POSIX_HPP_INCLUDED__
#define __TIKI_THREADING_POSIX_HPP_INCLUDED__

#include <pthread.h>
#include <semaphore.h>

namespace tiki
{
	struct ThreadPlatformData
	{
		pthread_t		threadHandle;
		uint64			threadId;
		
		char			name[ 16u ];
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
		bool				isInitialized;

		pthread_mutex_t	signalMutex;
		pthread_cond_t		condition;
		
		volatile bool		isSignaled;
		volatile uint32	waitingThreadCount;
	};
}

#endif // __TIKI_THREADING_POSIX_HPP_INCLUDED__
