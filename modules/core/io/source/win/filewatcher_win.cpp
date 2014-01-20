
#include "tiki/io/filewatcher.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/file.hpp"
#include "tiki/base/iopath.hpp"

namespace tiki
{
	static void createEvent( FileWatcherEvent& fileEvent, const string& basePath, const FILE_NOTIFY_INFORMATION* pNotifyInfo )
	{
		static FileWatcherEventType eventTypeMapping[] =
		{
			FileWatcherEventType_Invalid,
			FileWatcherEventType_Created,
			FileWatcherEventType_Deleted,
			FileWatcherEventType_Modified,
			FileWatcherEventType_Deleted,
			FileWatcherEventType_Created
		};
		TIKI_COMPILETIME_ASSERT( FILE_ACTION_ADDED				== 1 );
		TIKI_COMPILETIME_ASSERT( FILE_ACTION_REMOVED			== 2 );
		TIKI_COMPILETIME_ASSERT( FILE_ACTION_MODIFIED			== 3 );
		TIKI_COMPILETIME_ASSERT( FILE_ACTION_RENAMED_OLD_NAME	== 4 );
		TIKI_COMPILETIME_ASSERT( FILE_ACTION_RENAMED_NEW_NAME	== 5 );

		const string fileName = path::combine( basePath, convertString( wstring( pNotifyInfo->FileName, pNotifyInfo->FileNameLength / sizeof( wchar_t ) ) ) );
		if ( file::exists( fileName ) )
		{
			uint32 counter = 0;

			HANDLE handle = INVALID_HANDLE_VALUE;
			while( handle == INVALID_HANDLE_VALUE && counter < 1000u )
			{
				Sleep( 1 );
				handle = CreateFile( fileName.cStr(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
				counter++;
			}

			if ( handle != INVALID_HANDLE_VALUE )
			{
				CloseHandle( handle );
			}

			fileEvent.eventType	= eventTypeMapping[ pNotifyInfo->Action ];
			fileEvent.fileName	= fileName;
		}
	}

	static bool readEvents( FileWatcherPlatformData& platformData, Mutex* pMutex, Queue< FileWatcherEvent >& events )
	{
		if ( ReadDirectoryChangesW( platformData.watchHandle, &platformData.dataBuffer, sizeof( platformData.dataBuffer ), TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE, &platformData.requiredSize, 0, 0 ) )
		{
			const uint8* pBinaryData = platformData.dataBuffer;
			const FILE_NOTIFY_INFORMATION* pNotifyInfo = reinterpret_cast< const FILE_NOTIFY_INFORMATION* >( pBinaryData );

			do 
			{
				if ( pMutex != nullptr )
				{
					MutexStackLock lock( *pMutex );
					createEvent( events.push(), platformData.basePath, pNotifyInfo );
				}
				else
				{
					createEvent( events.push(), platformData.basePath, pNotifyInfo );
				}

				pBinaryData += pNotifyInfo->NextEntryOffset;
				pNotifyInfo = reinterpret_cast< const FILE_NOTIFY_INFORMATION* >( pBinaryData );
			} 
			while ( pNotifyInfo->NextEntryOffset );

			return true;
		}

		return false;
	}

	FileWatcher::FileWatcher()
	{
		m_platformData.watchHandle = INVALID_HANDLE_VALUE;
	}

	FileWatcher::~FileWatcher()
	{
		TIKI_ASSERT( m_platformData.watchHandle == INVALID_HANDLE_VALUE );
	}

	bool FileWatcher::create( const char* pPath, uint maxEventCount, bool blockingMode )
	{
		m_blockingMode = blockingMode;

		m_platformData.basePath = pPath;

		m_platformData.watchHandle = CreateFile(
			pPath,
			FILE_LIST_DIRECTORY,                
			FILE_SHARE_READ | FILE_SHARE_DELETE,
			NULL,                               
			OPEN_EXISTING,                      
			FILE_FLAG_BACKUP_SEMANTICS,         
			NULL                                
		);

		if ( m_platformData.watchHandle == INVALID_HANDLE_VALUE )
		{
			return false;
		}

		m_events.create( maxEventCount );

		if ( m_blockingMode == false )
		{
			if ( m_watcherThread.create( threadStaticEntryPoint, 8192u, "FileWatcher" ) == false )
			{
				return false;
			}

			m_watcherMutex.create();
			m_watcherThread.start( this );
		}

		return true;
	}

	void FileWatcher::dispose()
	{
		if ( m_platformData.watchHandle != INVALID_HANDLE_VALUE )
		{
			CloseHandle( m_platformData.watchHandle );
			m_platformData.watchHandle = INVALID_HANDLE_VALUE;
		}

		if ( m_blockingMode == false )
		{
			m_watcherThread.requestExit();
			m_watcherThread.waitForExit();
			m_watcherThread.dispose();

			m_watcherMutex.dispose();
		}

		m_events.dispose();
	}

	bool FileWatcher::popEvent( FileWatcherEvent& fileEvent )
	{
		if ( m_blockingMode == false )
		{
			MutexStackLock lock( m_watcherMutex );
			return m_events.pop( fileEvent );
		}
		else if ( m_events.isEmpty() == false || readEvents( m_platformData, nullptr, m_events ) )
		{
			return m_events.pop( fileEvent );
		}

		return false;
	}

	void FileWatcher::threadEntryPoint( const Thread& thread )
	{
		TIKI_ASSERT( m_platformData.watchHandle != INVALID_HANDLE_VALUE );
		
		while ( thread.isExitRequested() == false && readEvents( m_platformData, &m_watcherMutex, m_events ) )
		{
		}
	}

	int FileWatcher::threadStaticEntryPoint( const Thread& thread )
	{
		FileWatcher* pWatcher = static_cast< FileWatcher* >( thread.getArgument() );
		pWatcher->threadEntryPoint( thread );
		return 0;
	}
}