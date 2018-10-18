#pragma once

#include "tiki/base/types.hpp"

namespace tiki
{
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

		TIKI_FORCE_INLINE char	toLower( char c );
		TIKI_FORCE_INLINE char	toUpper( char c );
	}
}

#include "../../../source/ascii.inl"