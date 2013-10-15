#pragma once
#ifndef TIKI_FILE_HPP
#define TIKI_FILE_HPP

#include "tiki/base/array.hpp"
#include "tiki/base/types.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/inline.hpp"

namespace tiki
{
	namespace file
	{
		bool	exists( const string& fileName );
		bool	copy( const string& from, const string& to, bool overwrite = true );
		bool	move( const string& from, const string& to, bool overwrite = true );
		bool	remove( const string& fileName );

		bool	readAllText( const string& fileName, string& contentTarget );
		bool	readAllBytes( const string& fileName, Array< uint8 >& buffer, size_t alignment = TIKI_DEFAULT_ALIGNMENT );

		bool	writeAllBytes( const string& fileName, const uint8* pData, size_t dataLength );

		crc32	getLastChangeCrc( const string& fileName );
	}
}

#endif // TIKI_FILE_HPP
