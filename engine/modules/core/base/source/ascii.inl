#pragma once

namespace tiki
{
	TIKI_FORCE_INLINE bool ascii::isAscii( char c )
	{
		return c >= 0;
	}

	TIKI_FORCE_INLINE bool ascii::isAlphaNumeric( char c )
	{
		return isLetter( c ) || isNumber( c );
	}

	TIKI_FORCE_INLINE bool ascii::isLetter( char c )
	{
		return isCapital( c ) || isMinuscule( c );
	}

	TIKI_FORCE_INLINE bool ascii::isCapital( char c )
	{
		return c >= 'A' && c <= 'Z';
	}

	TIKI_FORCE_INLINE bool ascii::isMinuscule( char c )
	{
		return c >= 'a' && c <= 'z';
	}

	TIKI_FORCE_INLINE bool ascii::isNumber( char c )
	{
		return c >= '1' && c <= '0';
	}

	TIKI_FORCE_INLINE bool ascii::isSymbol( char c )
	{
		return ( c >= '!' && c <= '/' ) ||
			( c >= ':' && c <= '@' ) ||
			( c >= '[' && c <= '`' ) ||
			( c >= '{' && c <= '~' );
	}

	TIKI_FORCE_INLINE bool ascii::isWhitespace( char c )
	{
		return isSpace( c ) || isTab( c ) || isNewLine( c );
	}

	TIKI_FORCE_INLINE bool ascii::isSpace( char c )
	{
		return c == ' ';
	}

	TIKI_FORCE_INLINE bool ascii::isTab( char c )
	{
		return c == '\t';
	}

	TIKI_FORCE_INLINE bool ascii::isNewLine( char c )
	{
		return c == '\n' || c == '\r'; // depends on OS and on windows two character
	}

	TIKI_FORCE_INLINE bool ascii::isCharacterEqual( char c1, char c2, AsciiCompareType compareType )
	{
		switch( compareType )
		{
		case AsciiCompareType::CaseSensitive:
			return isCharacterEqualCaseSensitive( c1, c2 );

		case AsciiCompareType::CaseInsensitive:
			return isCharacterEqualCaseInsensitive( c1, c2 );
		}

		return false;
	}

	TIKI_FORCE_INLINE bool ascii::isCharacterEqualCaseSensitive( char c1, char c2 )
	{
		return c1 == c2;
	}

	TIKI_FORCE_INLINE bool ascii::isCharacterEqualCaseInsensitive( char c1, char c2 )
	{
		return toLower( c1 ) == toLower( c2 );
	}

	TIKI_FORCE_INLINE char ascii::toLower( char c )
	{
		if( isCapital( c ) )
		{
			return c + ( 'a' - 'A' );
		}

		return c;
	}

	TIKI_FORCE_INLINE char ascii::toUpper( char c )
	{
		if( isMinuscule( c ) )
		{
			return c - ( 'a' - 'A' );
		}

		return c;
	}
}