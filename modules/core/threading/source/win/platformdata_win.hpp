#pragma once
#ifndef __TIKI_THREADING_WIN_HPP_INCLUDED__
#define __TIKI_THREADING_WIN_HPP_INCLUDED__

#include <windows.h>

namespace tiki
{
	enum
	{
		TimeOutInfinity = 0xFFFFFFFF
	};

	struct ThreadPlatformData
	{
		HANDLE	threadHandle;
		DWORD	threadId;
	};

	struct MutexPlatformData
	{
		bool				isInitialized;
		CRITICAL_SECTION	mutexData;
	};

	struct SemaphorePlatformData
	{
		HANDLE	semaphoreHandle;
	};

	struct EventPlatformData
	{
		HANDLE	eventHandle;
	};
}

#endif // __TIKI_THREADING_WIN_HPP_INCLUDED__
