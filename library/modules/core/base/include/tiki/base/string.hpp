#pragma once
#ifndef TIKI_STRING_HPP_INCLUDED__
#define TIKI_STRING_HPP_INCLUDED__

#include "tiki/base/types.hpp"

namespace tiki
{
	TIKI_FORCE_INLINE uint	getStringSize( const char* pSource );		///< returns the string size in bytes
	TIKI_FORCE_INLINE uint	getStringLength( const char* pSource );		///< returns the string length in chars(for UTF8)
	TIKI_FORCE_INLINE uint	copyString( char* pTargetBuffer, uint bufferSize, const char* pSource );

	TIKI_FORCE_INLINE bool	isStringEquals( const char* pString1, const char* pString2 );

	TIKI_FORCE_INLINE uint	stringIndexOf( const char* pString, char c, uint index = 0 );
	TIKI_FORCE_INLINE uint	stringIndexOf( const char* pString, const char* pSearch, uint index = 0 );
	TIKI_FORCE_INLINE uint	stringLastIndexOf( const char* pString, char c, uint index = TIKI_SIZE_T_MAX );
	TIKI_FORCE_INLINE uint	stringLastIndexOf( const char* pString, const char* pSearch, uint index = TIKI_SIZE_T_MAX );

	bool					convertUtf8ToWidecharString( wchar_t* pTargetBuffer, uint targetLengthInCharacters, const char* pSourceBuffer );
	bool					convertWidecharToUtf8String( char* pTargetBuffer, uint targetLengthInCharacters, const wchar_t* pSourceBuffer );

	void					formatStringBuffer( char* pTargetBuffer, uint targetLength, const char* format, ... );

}

#include "../../../source/string.inl"

#endif // TIKI_STRING_HPP_INCLUDED__
