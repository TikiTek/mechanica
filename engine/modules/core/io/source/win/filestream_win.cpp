
#include "tiki/io/filestream.hpp"

#include "tiki/base/assert.hpp"

#include "platformdata_win.hpp"

#include <windows.h>

namespace tiki
{
	union LowHighSeperation
	{
		struct 
		{
			LONG	lowLong;
			LONG	highLong;
		}; 
		struct 
		{
			DWORD	lowDoubleWord;
			DWORD	highDoubleWord;
		}; 
		FileSize	size;
		FileOffset	offset;
	};

	static DWORD s_accessMapping[] =
	{
		GENERIC_READ,
		GENERIC_WRITE,
		GENERIC_WRITE,
		( GENERIC_READ | GENERIC_WRITE ),
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_accessMapping ) == DataAccessMode_Count );

	static DWORD s_creationMapping[] =
	{
		OPEN_EXISTING,
		CREATE_ALWAYS,
		OPEN_ALWAYS,
		CREATE_ALWAYS
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_creationMapping ) == DataAccessMode_Count );

	static DWORD s_seekMapping[] =
	{
		FILE_BEGIN,
		FILE_CURRENT,
		FILE_END
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_seekMapping ) == DataStreamSeek_Count );

	FileStream::FileStream()
	{
		m_platformData.fileHandle = INVALID_HANDLE_VALUE;
	}

	FileStream::~FileStream()
	{
		TIKI_ASSERT( m_platformData.fileHandle == INVALID_HANDLE_VALUE );
	}

	bool FileStream::create( const char* pFileName, DataAccessMode accessMode )
	{
		wchar_t finalPath[ TIKI_MAX_PATH ];
		convertToPlatformPath( finalPath, TIKI_COUNT( finalPath ), pFileName );

		TIKI_ASSERT( m_platformData.fileHandle == INVALID_HANDLE_VALUE );
		m_platformData.fileHandle = CreateFileW( finalPath, s_accessMapping[ accessMode ], 0u, nullptr, s_creationMapping[ accessMode ], FILE_ATTRIBUTE_NORMAL, nullptr );

		return m_platformData.fileHandle != INVALID_HANDLE_VALUE;
	}

	void FileStream::dispose()
	{
		if ( m_platformData.fileHandle != INVALID_HANDLE_VALUE )
		{
			CloseHandle( m_platformData.fileHandle );
			m_platformData.fileHandle = INVALID_HANDLE_VALUE;
		}
	}

	bool FileStream::isOpen() const
	{
		return m_platformData.fileHandle != INVALID_HANDLE_VALUE;
	}

	FileSize FileStream::read( void* pTargetData, FileSize bytesToRead ) const
	{
		TIKI_ASSERT( isOpen() );

		DWORD bytesRead = 0u;
		
		if ( ReadFile( m_platformData.fileHandle, pTargetData, DWORD( bytesToRead ), &bytesRead, nullptr ) )
		{
			return bytesRead;
		}

		return 0u;
	}

	FileSize FileStream::write( const void* pSourceData, FileSize bytesToWrite )
	{
		TIKI_ASSERT( isOpen() );

		DWORD bytesWritten = 0u;
		TIKI_VERIFY( WriteFile( m_platformData.fileHandle, pSourceData, DWORD( bytesToWrite ), &bytesWritten, nullptr ) );

		return bytesWritten;
	}

	FileSize FileStream::getPosition() const
	{
		TIKI_ASSERT( isOpen() );
		return SetFilePointer( m_platformData.fileHandle, 0, nullptr, FILE_CURRENT );
	}

	void FileStream::setPosition( FileSize position )
	{
		TIKI_ASSERT( isOpen() );

		LowHighSeperation sepPos;
		sepPos.size = position;

		SetFilePointer( m_platformData.fileHandle, sepPos.lowLong, &sepPos.highLong, FILE_BEGIN );
	}

	FileSize FileStream::seekPosition( FileOffset offset, DataStreamSeek method /* = DataStreamSeek_Current */ )
	{
		TIKI_ASSERT( isOpen() );

		LowHighSeperation sepOffset;
		sepOffset.offset = offset;

		sepOffset.lowDoubleWord = SetFilePointer( m_platformData.fileHandle, sepOffset.lowLong, &sepOffset.highLong, s_seekMapping[ method ] );

		return sepOffset.size;
	}

	FileSize FileStream::getLength() const
	{
		TIKI_ASSERT( isOpen() );

		LowHighSeperation sepLength;
		sepLength.size = 0;

		sepLength.lowDoubleWord = GetFileSize( m_platformData.fileHandle, &sepLength.highDoubleWord );

		return sepLength.size;
	}

	void FileStream::setLength( FileSize length )
	{
		TIKI_ASSERT( isOpen() );

		LowHighSeperation sepLength;
		sepLength.size = 0;
		sepLength.lowDoubleWord = SetFilePointer( m_platformData.fileHandle, 0, &sepLength.highLong, FILE_END );

		FileSize currentLength = sepLength.size;

		uint8 data = 0;
		DWORD bytesWritten;
		while (currentLength < length)
		{
			if( !WriteFile( m_platformData.fileHandle, &data, 1, &bytesWritten, nullptr ) )
			{
				break;
			}

			currentLength += bytesWritten;
		}
	}
}