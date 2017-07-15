#pragma once
#ifndef TIKI_FILE_HPP
#define TIKI_FILE_HPP

#include "tiki/container/array.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	namespace file
	{
		bool	exists( const char* pFileName );
		bool	copy( const char* pFrom, const char* pTo, bool overwrite = true );
		bool	move( const char* pFrom, const char* pTo, bool overwrite = true );
		bool	remove( const char* pFileName );

		bool	readAllText( const char* pFileName, Array< char >& targetContent  );
		bool	readAllBytes( const char* pFileName, Array< uint8 >& buffer, size_t alignment = TIKI_DEFAULT_ALIGNMENT );

		bool	writeAllBytes( const char* pFileName, const uint8* pData, size_t dataLength );

		crc32	getLastChangeCrc( const char* pFileName );
	}
}

#endif // TIKI_FILE_HPP
