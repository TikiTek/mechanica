#include "tiki/io/file_watcher.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/string.hpp"
#include "tiki/io/file.hpp"
#include "tiki/io/path.hpp"

namespace tiki
{
	static FileWatcherEventType s_aEventTypeMapping[] =
	{
		FileWatcherEventType_Invalid,
		FileWatcherEventType_Created,
		FileWatcherEventType_Deleted,
		FileWatcherEventType_Modified,
		FileWatcherEventType_Deleted,
		FileWatcherEventType_Created
	};
	TIKI_COMPILETIME_ASSERT( FILE_ACTION_ADDED == 1 );
	TIKI_COMPILETIME_ASSERT( FILE_ACTION_REMOVED == 2 );
	TIKI_COMPILETIME_ASSERT( FILE_ACTION_MODIFIED == 3 );
	TIKI_COMPILETIME_ASSERT( FILE_ACTION_RENAMED_OLD_NAME == 4 );
	TIKI_COMPILETIME_ASSERT( FILE_ACTION_RENAMED_NEW_NAME == 5 );

	static void createEvents( Queue< FileWatcherEvent >& events, const char* pBasePath, const void* pData )
	{
		const uint8* pBinaryData = static_cast< const uint8* >( pData );
		const FILE_NOTIFY_INFORMATION* pNotifyInfo = reinterpret_cast< const FILE_NOTIFY_INFORMATION* >( pBinaryData );

		do
		{
			char filenameBuffer[ TIKI_MAX_PATH ];
			convertWidecharToUtf8String( filenameBuffer, TIKI_COUNT( filenameBuffer ), pNotifyInfo->FileName, pNotifyInfo->FileNameLength / 2u );

			Path filePath;
			filePath.setCombinedPath( pBasePath, filenameBuffer );
			if ( file::exists( filePath.getCompletePath() ) )
			{
				uint32 counter = 0;

				HANDLE handle = INVALID_HANDLE_VALUE;
				do
				{
					handle = CreateFileA( filePath.getCompletePath(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

					if ( handle == INVALID_HANDLE_VALUE )
					{
						Sleep( 1 );
						counter++;
					}
				}
				while( handle == INVALID_HANDLE_VALUE && counter < 1000u );

				if ( handle != INVALID_HANDLE_VALUE )
				{
					CloseHandle( handle );
					handle = INVALID_HANDLE_VALUE;
				}

				FileWatcherEvent& fileEvent = events.push();
				fileEvent.eventType	= s_aEventTypeMapping[ pNotifyInfo->Action ];
				fileEvent.filePath	= filePath;
			}

			pBinaryData += pNotifyInfo->NextEntryOffset;
			pNotifyInfo = reinterpret_cast< const FILE_NOTIFY_INFORMATION* >( pBinaryData );
		}
		while ( pNotifyInfo->NextEntryOffset );
	}

	FileWatcher::FileWatcher()
	{
		m_platformData.dirHandle	= INVALID_HANDLE_VALUE;
		m_platformData.portHandle	= INVALID_HANDLE_VALUE;
	}

	FileWatcher::~FileWatcher()
	{
		TIKI_ASSERT( m_platformData.dirHandle == INVALID_HANDLE_VALUE );
		TIKI_ASSERT( m_platformData.portHandle == INVALID_HANDLE_VALUE );
	}

	bool FileWatcher::create( const char* pPath, uint maxEventCount )
	{
		copyString( m_platformData.aPathBuffer, TIKI_COUNT( m_platformData.aPathBuffer ), pPath );

		m_platformData.running		= false;
		m_platformData.requiredSize	= 0u;

		wchar_t widePathBuffer[ TIKI_MAX_PATH ];
		if ( !convertUtf8ToWidecharString( widePathBuffer, TIKI_COUNT( widePathBuffer ), pPath ) )
		{
			return false;
		}

		m_platformData.dirHandle = CreateFileW(
			widePathBuffer,
			FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
			NULL
		);

		if ( m_platformData.dirHandle == INVALID_HANDLE_VALUE )
		{
			dispose();
			return false;
		}

		m_platformData.portHandle = CreateIoCompletionPort( m_platformData.dirHandle, nullptr, 0, 1u );
		if ( m_platformData.portHandle == INVALID_HANDLE_VALUE )
		{
			dispose();
			return false;
		}

		memory::zero( m_platformData.overlapped );
		m_events.create( maxEventCount );

		return true;
	}


	void FileWatcher::dispose()
	{
		if ( m_platformData.dirHandle != INVALID_HANDLE_VALUE )
		{
			CancelIo( m_platformData.dirHandle );
			CloseHandle( m_platformData.dirHandle );
			m_platformData.dirHandle = INVALID_HANDLE_VALUE;
		}

		if ( m_platformData.portHandle != INVALID_HANDLE_VALUE )
		{
			CloseHandle( m_platformData.portHandle );
			m_platformData.portHandle = INVALID_HANDLE_VALUE;
		}

		m_events.dispose();

		m_platformData.aPathBuffer[ 0u ] = '\0';
	}

	bool FileWatcher::popEvent( FileWatcherEvent& fileEvent )
	{
		if ( !m_platformData.running )
		{
			const BOOL result = ReadDirectoryChangesW(
				m_platformData.dirHandle,
				&m_platformData.dataBuffer,
				sizeof( m_platformData.dataBuffer ),
				true,
				FILE_NOTIFY_CHANGE_LAST_WRITE,
				&m_platformData.requiredSize,
				&m_platformData.overlapped,
				nullptr
			);

			if ( result )
			{
				m_platformData.running = true;
			}
			else
			{
				const DWORD lastError = GetLastError();
				TIKI_TRACE_ERROR( "[io] ReadDirectoryChangesW in FileWatcher has returned an error code: %u\n", lastError );
			}
		}

		if ( m_platformData.running )
		{
			waitForEvent( 1u );
		}

		if ( !m_events.isEmpty() )
		{
			return m_events.pop( fileEvent );
		}

		return false;
	}

	bool FileWatcher::waitForEvent( timems timeOut /* = TIKI_TIME_OUT_INFINITY */ )
	{
		ULONG_PTR key;
		OVERLAPPED* pOverlapped;
		BOOL success = GetQueuedCompletionStatus(
			m_platformData.portHandle,
			&m_platformData.requiredSize,
			&key,
			&pOverlapped,
			DWORD( timeOut )
		);

		const DWORD lastError = GetLastError();
		if ( success )
		{
			createEvents( m_events, m_platformData.aPathBuffer, m_platformData.dataBuffer );
			m_platformData.running = false;
			return true;
		}
		else if ( lastError != WAIT_TIMEOUT )
		{
			TIKI_TRACE_ERROR( "[io] GetQueuedCompletionStatus in FileWatcher has return an error code: %u\n", lastError );
		}

		return false;
	}
}
