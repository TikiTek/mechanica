#pragma once

#include "tiki/base/types.hpp"

#include <vadefs.h>

namespace tiki
{
	static const uintreg InvalidStringIndex = (uintreg)-1;

	TIKI_FORCE_INLINE uintreg	getStringSize( const char* pSource );		///< returns the string size in bytes
	//TIKI_FORCE_INLINE uintreg	getStringLength( const char* pSource );		///< returns the string length in chars(for UTF8)

	TIKI_FORCE_INLINE uintreg	copyString( char* pTargetBuffer, uintreg bufferSize, const char* pSource );
	TIKI_FORCE_INLINE uintreg	appendString( char* pTargetbuffer, uintreg bufferSize, const char* pSource );
	TIKI_FORCE_INLINE char*		dublicateString( const char* pSource );

	TIKI_FORCE_INLINE bool		isStringEmpty( const char* pString );
	TIKI_FORCE_INLINE bool		isStringEquals( const char* pString1, const char* pString2 );

	TIKI_FORCE_INLINE bool		doesStringStartWith( const char* pString, const char* pValue );
	TIKI_FORCE_INLINE bool		doesStringEndWith( const char* pString, const char* pValue );

	TIKI_FORCE_INLINE void		stringReplace( char* pString, char oldChar, char newChar );

	TIKI_FORCE_INLINE uintreg	stringIndexOf( const char* pString, char c, uintreg index = 0 );
	TIKI_FORCE_INLINE uintreg	stringIndexOf( const char* pString, const char* pSearch, uintreg index = 0 );
	TIKI_FORCE_INLINE uintreg	stringLastIndexOf( const char* pString, char c, uintreg index = TIKI_SIZE_T_MAX );
	TIKI_FORCE_INLINE uintreg	stringLastIndexOf( const char* pString, const char* pSearch, uintreg index = TIKI_SIZE_T_MAX );

	bool						convertUtf8ToWidecharString( wchar_t* pTargetBuffer, uintreg targetLengthInCharacters, const char* pSourceBuffer );
	bool						convertWidecharToUtf8String( char* pTargetBuffer, uintreg targetLengthInCharacters, const wchar_t* pSourceBuffer, uintreg length = 0u );

	void						formatStringBuffer( char* pTargetBuffer, uintreg targetLength, const char* pFormat, ... );
	void						formatStringBufferArgs( char* pTargetBuffer, uintreg targetLength, const char* pFormat, va_list args );
}

#include "../../../source/string.inl"
