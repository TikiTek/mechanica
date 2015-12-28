#pragma once
#ifndef TIKI_FILE_HPP
#define TIKI_FILE_HPP

#include "tiki/container/array.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	namespace file
	{
		bool	exists( cstring pFileName );
		bool	copy( cstring pFrom, cstring pTo, bool overwrite = true );
		bool	move( cstring pFrom, cstring pTo, bool overwrite = true );
		bool	remove( cstring pFileName );

		bool	readAllText( cstring pFileName, Array< char >& targetContent, size_t alignment = TIKI_DEFAULT_ALIGNMENT  );
		bool	readAllBytes( cstring pFileName, Array< uint8 >& buffer, size_t alignment = TIKI_DEFAULT_ALIGNMENT );

		bool	writeAllBytes( cstring pFileName, const uint8* pData, size_t dataLength );

		crc32	getLastChangeCrc( cstring pFileName );
	}
}

#endif // TIKI_FILE_HPP
