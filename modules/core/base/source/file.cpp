
#include "tiki/base/file.hpp"

#include "tiki/base/basicstring.hpp"
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
	bool file::exists( cstring pFileName )
	{
		return (GetFileAttributesA( getPlatformFilename( pFileName ).cStr() ) != INVALID_FILE_ATTRIBUTES);
	}

	bool file::copy( cstring pFrom, cstring pTo, bool overwrite /* = true */ )
	{
		return CopyFileA( getPlatformFilename( pFrom ).cStr(), getPlatformFilename( pTo ).cStr(), !overwrite ) != 0;
	}

	bool file::move( cstring pFrom, cstring pTo, bool overwrite /* = true */ )
	{
		const string to = getPlatformFilename( pTo );
		if ( file::exists( to.cStr() ) && !overwrite )
		{
			return false;
		}

		return MoveFileA( getPlatformFilename( pFrom ).cStr(), to.cStr() ) != 0;
	}

	bool file::remove( cstring pFileName )
	{
		return DeleteFileA( getPlatformFilename( pFileName ).cStr() ) != 0;
	}

	bool file::readAllText( cstring pFileName, Array< char >& targetContent, size_t alignment /*= TIKI_DEFAULT_ALIGNMENT*/  )
	{
		FILE* pFile;
		if ( fopen_s( &pFile, getPlatformFilename( pFileName ).cStr(), "rb" ) )
		{
			return false;
		}

		fpos_t len;
		fseek( pFile, 0, SEEK_END );
		fgetpos( pFile, &len );
		fseek( pFile, 0, SEEK_SET );

		targetContent.create( (uint)len, alignment );
		fread_s( targetContent.getBegin(), (size_t)len, (size_t)len, 1u, pFile );
		fclose( pFile );

		return true;
	}

	bool file::readAllBytes( cstring pFileName, Array< uint8 >& buffer, size_t alignment /* = TIKI_DEFAULT_ALIGNMENT */ )
	{
		FILE* pFile;
		fpos_t len;

		if ( fopen_s( &pFile, getPlatformFilename( pFileName ).cStr(), "rb" ) )
		{
			return false;
		}

		fseek( pFile, 0, SEEK_END );
		fgetpos( pFile, &len );
		fseek( pFile, 0, SEEK_SET );

		buffer.create( (size_t)len, alignment );
		fread_s( buffer.getBegin(), (size_t)len, (size_t)len, 1u, pFile );
		fclose( pFile );

		return true;
	}

	bool file::writeAllBytes( cstring pFileName, const uint8* pData, size_t dataLength )
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

	crc32 file::getLastChangeCrc( cstring pFileName )
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

#elif TIKI_ENABLED( TIKI_BUILD_MINGW )

	bool file::exists( cstring pFileName )
	{
		return false;
	}

	bool file::copy( cstring pFrom, cstring pTo, bool overwrite /* = true */ )
	{
		return false;
	}

	bool file::move( cstring pFrom, cstring pTo, bool overwrite /* = true */ )
	{
		return false;
	}

	bool file::remove( cstring pFileName )
	{
		return false;
	}

	bool file::readAllText( cstring pFileName, Array< char >& targetContent, size_t alignment /* = TIKI_DEFAULT_ALIGNMENT */ )
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

	bool file::readAllBytes( cstring pFileName, Array< uint8 >& buffer, size_t alignment /* = TIKI_DEFAULT_ALIGNMENT */ )
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

	bool file::writeAllBytes( cstring pFileName, const uint8* pData, size_t dataLength )
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

	crc32 file::getLastChangeCrc( cstring pFileName )
	{
		return 0u;
	}

#endif
}
