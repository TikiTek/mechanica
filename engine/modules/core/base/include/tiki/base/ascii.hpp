#pragma once

#include "tiki/base/types.hpp"

namespace tiki
{
	enum class AsciiCompareType
	{
		CaseSensitive,
		CaseInsensitive
	};

	namespace ascii
	{
		TIKI_FORCE_INLINE bool	isAscii( char c );
		TIKI_FORCE_INLINE bool	isAlphaNumeric( char c );
		TIKI_FORCE_INLINE bool	isLetter( char c );
		TIKI_FORCE_INLINE bool	isCapital( char c );
		TIKI_FORCE_INLINE bool	isMinuscule( char c );
		TIKI_FORCE_INLINE bool	isNumber( char c );
		TIKI_FORCE_INLINE bool	isSymbol( char c );

		TIKI_FORCE_INLINE bool	isWhitespace( char c );
		TIKI_FORCE_INLINE bool	isSpace( char c );
		TIKI_FORCE_INLINE bool	isTab( char c );
		TIKI_FORCE_INLINE bool	isNewLine( char c );

		TIKI_FORCE_INLINE bool	isCharacterEqual( char c1, char c2, AsciiCompareType compareType );
		TIKI_FORCE_INLINE bool	isCharacterEqualCaseSensitive( char c1, char c2 );
		TIKI_FORCE_INLINE bool	isCharacterEqualCaseInsensitive( char c1, char c2 );

		TIKI_FORCE_INLINE char	toLower( char c );
		TIKI_FORCE_INLINE char	toUpper( char c );
	}
}

#include "../../../source/ascii.inl"