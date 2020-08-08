#pragma once
#ifndef TIKI_FILE_HPP
#define TIKI_FILE_HPP

#include "tiki/base/dynamic_string.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/array.hpp"

namespace tiki
{
	enum FileWriteResult
	{
		FileWriteResult_Error,
		FileWriteResult_NoChanged,
		FileWriteResult_Ok
	};

	namespace file
	{
		bool				exists( const char* pFileName );
		bool				copy( const char* pFrom, const char* pTo, bool overwrite = true );
		bool				move( const char* pFrom, const char* pTo, bool overwrite = true );
		bool				remove( const char* pFileName );

		bool				readAllText( const char* pFileName, Array< char >& targetContent  );
		bool				readAllBytes( const char* pFileName, Array< uint8 >& buffer, uintreg alignment = TIKI_DEFAULT_ALIGNMENT );

		bool				writeAllBytes( const char* pFileName, const uint8* pData, uintreg dataLength );
		FileWriteResult		writeToFileIfNotEquals( const Path& filePath, const DynamicString& content );

		crc32				getLastChangeCrc( const char* pFileName );
	}
}

#endif // TIKI_FILE_HPP
