#pragma once
#ifndef __TIKI_THREADING_POSIX_HPP_INCLUDED__
#define __TIKI_THREADING_POSIX_HPP_INCLUDED__

namespace tiki
{
	enum
	{
		TimeOutInfinity = 0xffffffffu
	};

	struct ThreadPlatformData
	{
	};

	struct MutexPlatformData
	{
	};

	struct SemaphorePlatformData
	{
	};

	struct EventPlatformData
	{
	};
}

#endif // __TIKI_THREADING_POSIX_HPP_INCLUDED__
