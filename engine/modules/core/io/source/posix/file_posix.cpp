
#include "tiki/io/file.hpp"

#include "tiki/base/basicstring.hpp"
#include "tiki/base/crc32.hpp"

#include <stdio.h>
#include <sys/stat.h>

namespace tiki
{
	static string getPlatformFilename( const string& fileName )
	{
		return fileName.replace( '\\', '/' );
	}

	bool file::exists( const char* pFileName )
	{
		struct stat stats = { 0 };
		if ( stat( pFileName, &stats ) == -1 )
		{
			return false;
		}
		
		return !isBitSet( stats.st_mode, S_IFDIR );
	}

	bool file::copy( const char* pFrom, const char* pTo, bool overwrite /* = true */ )
	{
		return false;
	}

	bool file::move( const char* pFrom, const char* pTo, bool overwrite /* = true */ )
	{
		if( !overwrite && file::exists( pTo ) )
		{
			return false;
		}
		
		return rename( pFrom, pTo ) == 0;
	}

	bool file::remove( const char* pFileName )
	{
		return remove( pFileName ) == 0;
	}

	bool file::readAllText( const char* pFileName, Array< char >& targetContent, uintreg alignment /* = TIKI_DEFAULT_ALIGNMENT */ )
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

		targetContent.create( len.__pos, alignment );
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
		struct stat stats = { 0 };
		if( stat( pFileName, &stats ) == -1 )
		{
			return TIKI_INVALID_CRC32;
		}
		
		return crcTemplate( stats.st_mtime );
	}
}
