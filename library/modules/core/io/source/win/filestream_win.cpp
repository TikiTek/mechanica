
#include "tiki/io/filestream.hpp"

#include "tiki/base/assert.hpp"

#include <windows.h>

namespace tiki
{
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

	FileStream::FileStream()
	{
		m_fileHandle = INVALID_HANDLE_VALUE;
	}

	FileStream::~FileStream()
	{
		TIKI_ASSERT( m_fileHandle == INVALID_HANDLE_VALUE );
	}

	bool FileStream::open( cstring pFileName, DataAccessMode accessMode )
	{
		TIKI_ASSERT( m_fileHandle == INVALID_HANDLE_VALUE );
		m_fileHandle = CreateFileA( pFileName, s_accessMapping[ accessMode ], 0u, nullptr, s_creationMapping[ accessMode ], FILE_ATTRIBUTE_NORMAL, nullptr );

		return m_fileHandle != INVALID_HANDLE_VALUE;
	}

	void FileStream::close()
	{
		if ( m_fileHandle != INVALID_HANDLE_VALUE )
		{
			CloseHandle( m_fileHandle );
			m_fileHandle = INVALID_HANDLE_VALUE;
		}
	}

	bool FileStream::isOpen() const
	{
		return m_fileHandle != INVALID_HANDLE_VALUE;
	}

	FileSize FileStream::read( void* pTargetData, uint bytesToRead )
	{
		DWORD bytesRead = 0u;
		
		if ( ReadFile( m_fileHandle, pTargetData, bytesToRead, &bytesRead, nullptr ) )
		{
			return bytesRead;
		}

		return 0u;
	}

	FileSize FileStream::write( const void* pSourceData, uint bytesToWrite )
	{
		DWORD bytesWritten = 0u;

		if ( WriteFile( m_fileHandle, pSourceData, bytesToWrite, &bytesWritten, nullptr ) )
		{
			return bytesWritten;
		}

		return 0u;
	}

	FileSize FileStream::getPosition()
	{
		return 0;
	}

	void FileStream::setPosition( FileSize position )
	{
		SetFilePointer( m_fileHandle, (DWORD)position, nullptr, FILE_BEGIN );
	}

	FileSize FileStream::seekPosition( FileSize seek )
	{
		return 0;
	}

	FileSize FileStream::getLength()
	{
		return GetFileSize( m_fileHandle, nullptr );
	}

	void FileStream::setLength( FileSize length )
	{
	}
}