
#include "tiki/io/filewatcher.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/file.hpp"

namespace tiki
{


	FileWatcher::FileWatcher()
	{
		m_platformData.watchHandle = INVALID_HANDLE_VALUE;
	}

	FileWatcher::~FileWatcher()
	{
		TIKI_ASSERT( m_platformData.watchHandle == INVALID_HANDLE_VALUE );
	}

	bool FileWatcher::create( const char* pPath, uint maxEventCount )
	{
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

		if ( m_watcherThread.create( threadStaticEntryPoint, 8192u, "FileWatcher" ) == false )
		{
			return false;
		}

		m_watcherMutex.create();
		m_events.create( maxEventCount );

		m_watcherThread.start( this );

		return true;
	}

	void FileWatcher::dispose()
	{
		if ( m_platformData.watchHandle != INVALID_HANDLE_VALUE )
		{
			CloseHandle( m_platformData.watchHandle );
			m_platformData.watchHandle = INVALID_HANDLE_VALUE;
		}
	}

	bool FileWatcher::popEvent( FileWatcherEvent& fileEvent )
	{
		MutexStackLock lock( m_watcherMutex );
		return m_events.pop( fileEvent );
	}

	void FileWatcher::threadEntryPoint( const Thread& thread )
	{
		TIKI_ASSERT( m_platformData.watchHandle != INVALID_HANDLE_VALUE );

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

		HANDLE watchHandle = m_platformData.watchHandle;

		DWORD requiredSize;
		uint8 infos[ 2048u ];

		while ( ReadDirectoryChangesW( watchHandle, &infos, sizeof(infos), TRUE, FILE_NOTIFY_CHANGE_LAST_WRITE, &requiredSize, 0, 0 ) )
		{
			const uint8* pBinaryData = infos;
			const FILE_NOTIFY_INFORMATION* pNotifyInfo = reinterpret_cast< const FILE_NOTIFY_INFORMATION* >( pBinaryData );

			do 
			{
				string fileName = convertString( pNotifyInfo->FileName );
				//path::combine(
				//	filePath,
				//	wstring(dataInfo->FileName, dataInfo->FileNameLength / sizeof(wchar_t))
				//);

				if ( file::exists( fileName ) )
				{
					uint32 counter = 0;

					HANDLE handle = INVALID_HANDLE_VALUE;
					while( handle == INVALID_HANDLE_VALUE && counter < 1000 )
					{
						handle = CreateFile( fileName.cStr(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
						counter++;
						Sleep( 1 );
					}

					if ( handle != INVALID_HANDLE_VALUE )
					{
						CloseHandle( handle );
					}

					{
						MutexStackLock lock( m_watcherMutex );

						FileWatcherEvent& fileEvent = m_events.push();
						fileEvent.eventType	= eventTypeMapping[ pNotifyInfo->Action ];
						fileEvent.pFileName = nullptr;
					}
				}

				pBinaryData += pNotifyInfo->NextEntryOffset;
				pNotifyInfo = reinterpret_cast< const FILE_NOTIFY_INFORMATION* >( pBinaryData );
			} 
			while ( pNotifyInfo->NextEntryOffset);
		}
	}

	int FileWatcher::threadStaticEntryPoint( const Thread& thread )
	{
		FileWatcher* pWatcher = static_cast< FileWatcher* >( thread.getArgument() );
		pWatcher->threadEntryPoint( thread );
		return 0;
	}
}