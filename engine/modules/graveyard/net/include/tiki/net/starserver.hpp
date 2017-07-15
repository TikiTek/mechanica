#pragma once
#ifndef __TIKI_FILESYSTEMWATCHER_HPP_INCLUDED__
#define __TIKI_FILESYSTEMWATCHER_HPP_INCLUDED__

#include "tiki/base/queue.hpp"
#include "tiki/base/basicstring.hpp"
#include "tiki/base/types.hpp"
#include "tiki/threading/mutex.hpp"
#include "tiki/threading/thread.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include "../../../source/win/platformdata_win.hpp"
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )
#	include "../../../source/posix/platformdata_posix.hpp"
#else
#	error not supported
#endif

namespace tiki
{
	enum FileWatcherEventType
	{
		FileWatcherEventType_Invalid = -1,

		FileWatcherEventType_Created,
		FileWatcherEventType_Deleted,
		FileWatcherEventType_Modified
		//FileWatcherEventType_Renamed -> will be converter into zwo events deleted followed from created
	};

	struct FileWatcherEvent
	{
		string					fileName;
		FileWatcherEventType	eventType;
	};

	class FileWatcher
	{
		TIKI_NONCOPYABLE_CLASS( FileWatcher );

	public:

		FileWatcher();
		~FileWatcher();

		bool	create( const char* pPath, uint maxEventCount );
		void	dispose();

		bool	popEvent( FileWatcherEvent& fileEvent );
		bool	waitForEvent( uint timeOut = TimeOutInfinity );
		
	private:

		Queue< FileWatcherEvent >	m_events;

		FileWatcherPlatformData		m_platformData;

	};
}

#endif // __TIKI_FILESYSTEMWATCHER_HPP_INCLUDED__
