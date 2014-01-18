#pragma once
#ifndef __TIKI_FILESYSTEMWATCHER_HPP_INCLUDED__
#define __TIKI_FILESYSTEMWATCHER_HPP_INCLUDED__

#include "tiki/base/queue.hpp"
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
		const char*				pFileName;
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

	private:

		Queue< FileWatcherEvent >		m_events;
		Thread							m_watcherThread;
		Mutex							m_watcherMutex;

		FileSystemWatcherPlatformData	m_platformData;

		void							threadEntryPoint( const Thread& thread );
		static int						threadStaticEntryPoint( const Thread& thread );

	};
}

#endif // __TIKI_FILESYSTEMWATCHER_HPP_INCLUDED__
