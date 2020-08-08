
#include "tiki/io/file.hpp"

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/crc32.hpp"

#include <stdio.h>

#if TIKI_ENABLED( TIKI_BUILD_MSVC )
#   include <Windows.h>
#endif

namespace tiki
{
	static DynamicString getPlatformFilename( const DynamicString& fileName )
	{
#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
		return fileName.replace( '/', '\\' );
#else
		return fileName.replace( '\\', '/' );
#endif
	}

#if TIKI_ENABLED( TIKI_BUILD_MSVC )
	bool file::exists( const char* pFileName )
	{
		const HANDLE fileHandle = CreateFileA( pFileName, 0u, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, nullptr );
		if( fileHandle == INVALID_HANDLE_VALUE )
		{
			return false;
		}

		BY_HANDLE_FILE_INFORMATION information;
		const bool doesExist = GetFileInformationByHandle( fileHandle, &information ) ? true : false;
		CloseHandle( fileHandle );
		return doesExist;
	}

	bool file::copy( const char* pFrom, const char* pTo, bool overwrite /* = true */ )
	{
		return CopyFileA( getPlatformFilename( pFrom ).cStr(), getPlatformFilename( pTo ).cStr(), !overwrite ) != 0;
	}

	bool file::move( const char* pFrom, const char* pTo, bool overwrite /* = true */ )
	{
		const DynamicString to = getPlatformFilename( pTo );
		if ( file::exists( to.cStr() ) && !overwrite )
		{
			return false;
		}

		return MoveFileA( getPlatformFilename( pFrom ).cStr(), to.cStr() ) != 0;
	}

	bool file::remove( const char* pFileName )
	{
		return DeleteFileA( getPlatformFilename( pFileName ).cStr() ) != 0;
	}

	bool file::readAllText( const char* pFileName, Array< char >& targetContent  )
	{
		HANDLE fileHandle = CreateFileA( pFileName, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0u, nullptr );
		if ( fileHandle == INVALID_HANDLE_VALUE )
		{
			const DWORD lastError = GetLastError();
			if( lastError == ERROR_SHARING_VIOLATION )
			{
				Thread::sleepCurrentThread( 50 );
				fileHandle = CreateFileA( pFileName, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0u, nullptr );
			}

			if( fileHandle == INVALID_HANDLE_VALUE )
			{
				TIKI_TRACE_ERROR( "[file::readAllText] Failed to open '%s'. Error: %08x\n", pFileName, GetLastError() );
				return false;
			}
		}

		const DWORD fileSize = GetFileSize( fileHandle, nullptr );
		targetContent.create( uintreg( fileSize ) + 1u );

		DWORD bytesRead;
		ReadFile( fileHandle, targetContent.getBegin(), fileSize, &bytesRead, nullptr );
		TIKI_ASSERT( bytesRead == fileSize );

		CloseHandle( fileHandle );

		targetContent[ fileSize ] = '\0';

		return true;
	}

	bool file::readAllBytes( const char* pFileName, Array< uint8 >& buffer, uintreg alignment /* = TIKI_DEFAULT_ALIGNMENT */ )
	{
		const HANDLE fileHandle = CreateFileA( pFileName, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0u, nullptr );
		if( fileHandle == INVALID_HANDLE_VALUE )
		{
			TIKI_TRACE_ERROR( "[file::readAllText] Failed to open '%s'. Error: %08x\n", pFileName, GetLastError() );
			return false;
		}

		const DWORD fileSize = GetFileSize( fileHandle, nullptr );
		buffer.create( fileSize );

		DWORD bytesRead;
		ReadFile( fileHandle, buffer.getBegin(), fileSize, &bytesRead, nullptr );
		TIKI_ASSERT( bytesRead == fileSize );

		CloseHandle( fileHandle );

		return true;
	}

	bool file::writeAllBytes( const char* pFileName, const uint8* pData, uintreg dataLength )
	{
		FILE* pFile;

		if ( fopen_s( &pFile, getPlatformFilename( pFileName ).cStr(), "wb" ) )
		{
			return false;
		}

		fwrite( pData, dataLength, 1u, pFile );

		fclose( pFile );
		return true;
	}

	crc32 file::getLastChangeCrc( const char* pFileName )
	{
		HANDLE handle = CreateFileA( getPlatformFilename( pFileName ).cStr(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr );

		if ( handle != INVALID_HANDLE_VALUE )
		{
			FILETIME time;

			if ( GetFileTime( handle, nullptr, nullptr, &time ) == false )
			{
				return 0u;
			}

			CloseHandle( handle );
			return crcTemplate( time );
		}

		return 0u;
	}

#elif TIKI_ENABLED( TIKI_BUILD_GCC ) || TIKI_ENABLED( TIKI_BUILD_CLANG )

	bool file::exists( const char* pFileName )
	{
		return false;
	}

	bool file::copy( const char* pFrom, const char* pTo, bool overwrite /* = true */ )
	{
		return false;
	}

	bool file::move( const char* pFrom, const char* pTo, bool overwrite /* = true */ )
	{
		return false;
	}

	bool file::remove( const char* pFileName )
	{
		return false;
	}

	bool file::readAllText( const char* pFileName, Array< char >& targetContent )
	{
		FILE* pFile = fopen( getPlatformFilename( pFileName ).cStr(), "r" );
		if ( pFile == nullptr )
		{
			return false;
		}

		fpos_t len;
		fseek( pFile, 0, SEEK_END );
		fgetpos( pFile, &len );
		fseek( pFile, 0, SEEK_SET );

		targetContent.create( len.__pos );
		fread( const_cast< char* >( targetContent.getBegin() ), (uintreg)len.__pos, 1u, pFile );
		fclose( pFile );

		return true;
	}

	bool file::readAllBytes( const char* pFileName, Array< uint8 >& buffer, uintreg alignment /* = TIKI_DEFAULT_ALIGNMENT */ )
	{
		FILE* pFile = fopen( getPlatformFilename( pFileName ).cStr(), "rb" );
		if ( pFile == nullptr )
		{
			return false;
		}

		fpos_t len;
		fseek( pFile, 0, SEEK_END );
		fgetpos( pFile, &len );
		fseek( pFile, 0, SEEK_SET );

		buffer.create( (uintreg)len.__pos, alignment );
		fread( buffer.getBegin(), (uintreg)len.__pos, 1u, pFile );
		fclose( pFile );

		return true;
	}

	bool file::writeAllBytes( const char* pFileName, const uint8* pData, uintreg dataLength )
	{
		FILE* pFile = fopen( getPlatformFilename( pFileName ).cStr(), "wb" );
		if ( pFile == nullptr )
		{
			return false;
		}

		fwrite( pData, dataLength, 1u, pFile );

		fclose( pFile );
		return true;
	}

	crc32 file::getLastChangeCrc( const char* pFileName )
	{
		return 0u;
	}

#endif
}
