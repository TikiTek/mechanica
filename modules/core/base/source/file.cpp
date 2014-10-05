
#include "tiki/base/file.hpp"

#include "tiki/base/crc32.hpp"

#include <stdio.h>

#if TIKI_ENABLED( TIKI_BUILD_MSVC )
#   include <Windows.h>
#endif

namespace tiki
{
	static string getPlatformFilename( const string& fileName )
	{
#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
		return fileName.replace( '/', '\\' );
#else
		return fileName.replace( '\\', '/' );
#endif
	}

#if TIKI_ENABLED( TIKI_BUILD_MSVC )
	bool file::exists( const string& fileName )
	{
		return (GetFileAttributesA( getPlatformFilename( fileName ).cStr() ) != INVALID_FILE_ATTRIBUTES);
	}

	bool file::copy( const string& from, const string& to, bool overwrite /*= true*/ )
	{
		return CopyFileA( getPlatformFilename( from ).cStr(), getPlatformFilename( to ).cStr(), !overwrite ) != 0;
	}

	bool file::move( const string& from, const string& to, bool overwrite /*= true*/ )
	{
		if ( file::exists( getPlatformFilename( to ) ) && !overwrite )
		{
			return false;
		}

		return MoveFileA( getPlatformFilename( from ).cStr(), getPlatformFilename( to ).cStr() ) != 0;
	}

	bool file::remove( const string& fileName )
	{
		return DeleteFileA( getPlatformFilename( fileName ).cStr() ) != 0;
	}

	bool file::readAllText( const string& fileName, string& contentTarget )
	{
		FILE* pFile;
		if ( fopen_s( &pFile, getPlatformFilename( fileName ).cStr(), "rb" ) )
		{
			return false;
		}

		fpos_t len;
		fseek( pFile, 0, SEEK_END );
		fgetpos( pFile, &len );
		fseek( pFile, 0, SEEK_SET );

		contentTarget = string( (uint)len );
		fread_s( const_cast< char* >( contentTarget.cStr() ), (size_t)len, (size_t)len, 1u, pFile );
		fclose( pFile );

		return true;
	}

	bool file::readAllBytes( const string& fileName, Array< uint8 >& buffer, size_t aligment /*= TIKI_DEFAULT_ALIGNMENT*/ )
	{
		FILE* pFile;
		fpos_t len;

		if ( fopen_s( &pFile, getPlatformFilename( fileName ).cStr(), "rb" ) )
		{
			return false;
		}

		fseek( pFile, 0, SEEK_END );
		fgetpos( pFile, &len );
		fseek( pFile, 0, SEEK_SET );

		buffer.create( (size_t)len, aligment );
		fread_s( buffer.getBegin(), (size_t)len, (size_t)len, 1u, pFile );
		fclose( pFile );

		return true;
	}

	bool file::writeAllBytes( const string& fileName, const uint8* pData, size_t dataLength )
	{
		FILE* pFile;

		if ( fopen_s( &pFile, getPlatformFilename( fileName ).cStr(), "wb" ) )
		{
			return false;
		}

		fwrite( pData, dataLength, 1u, pFile );

		fclose( pFile );
		return true;
	}

	crc32 file::getLastChangeCrc( const string& fileName )
	{
		HANDLE handle = CreateFileA( getPlatformFilename( fileName ).cStr(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr );

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

#elif TIKI_ENABLED( TIKI_BUILD_MINGW )

	bool file::exists( const string& fileName )
	{
		return false;
	}

	bool file::copy( const string& from, const string& to, bool overwrite /*= true*/ )
	{
		return false;
	}

	bool file::move( const string& from, const string& to, bool overwrite /*= true*/ )
	{
		return false;
	}

	bool file::remove( const string& fileName )
	{
		return false;
	}

	bool file::readAllText( const string& fileName, string& contentTarget )
	{
		FILE* pFile = fopen( getPlatformFilename( fileName ).cStr(), "r" );
		if ( pFile == nullptr )
		{
			return false;
		}

		fpos_t len;
		fseek( pFile, 0, SEEK_END );
		fgetpos( pFile, &len );
		fseek( pFile, 0, SEEK_SET );

		contentTarget = string( (uint)len );
		fread( const_cast< char* >( contentTarget.cStr() ), (size_t)len, 1u, pFile );
		fclose( pFile );

		return true;
	}

	bool file::readAllBytes( const string& fileName, Array< uint8 >& buffer, size_t aligment /*= TIKI_DEFAULT_ALIGNMENT*/ )
	{
		FILE* pFile = fopen( getPlatformFilename( fileName ).cStr(), "rb" );
		if ( pFile == nullptr )
		{
			return false;
		}

		fpos_t len;
		fseek( pFile, 0, SEEK_END );
		fgetpos( pFile, &len );
		fseek( pFile, 0, SEEK_SET );

		buffer.create( (size_t)len, aligment );
		fread( buffer.getBegin(), (size_t)len, 1u, pFile );
		fclose( pFile );

		return true;
	}

	bool file::writeAllBytes( const string& fileName, const uint8* pData, size_t dataLength )
	{
		FILE* pFile = fopen( getPlatformFilename( fileName ).cStr(), "wb" );
		if ( pFile == nullptr )
		{
			return false;
		}

		fwrite( pData, dataLength, 1u, pFile );

		fclose( pFile );
		return true;
	}

	crc32 file::getLastChangeCrc( const string& fileName )
	{
		return 0u;
	}

#endif
}
