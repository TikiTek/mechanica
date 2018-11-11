#pragma once

#include "tiki/base/path.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/queue.hpp"
#include "tiki/threading/mutex.hpp"
#include "tiki/threading/thread.hpp"

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include "../../../source/win/platform_data_win.hpp"
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )
#	include "../../../source/posix/platform_data_posix.hpp"
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
		Path					filePath;
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
		bool	waitForEvent( timems timeOut = TIKI_TIME_OUT_INFINITY );

	private:

		Queue< FileWatcherEvent >	m_events;

		FileWatcherPlatformData		m_platformData;
	};
}
