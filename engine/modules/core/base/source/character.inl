#pragma once
#ifndef TIKI_CHARACTER_INL_INCLUDED
#define TIKI_CHARACTER_INL_INCLUDED

namespace tiki
{
	TIKI_FORCE_INLINE bool character::isNumber( char c )
	{
		return c >= '0' && c <= '9';
	}

	TIKI_FORCE_INLINE bool character::isLetter( char c )
	{
		return isMinuscule( c ) || isCapital( c );
	}

	TIKI_FORCE_INLINE bool character::isMinuscule( char c )
	{
		return c >= 'a' && c <= 'z';
	}

	TIKI_FORCE_INLINE bool character::isCapital( char c )
	{
		return c >= 'A' && c <= 'Z';
	}

	TIKI_FORCE_INLINE bool character::isAlphaNumeric( char c )
	{
		return isNumber( c ) || isLetter( c );
	}

	TIKI_FORCE_INLINE bool character::isWhitespace( char c )
	{
		return c == '\t' || c == ' ' || c == '\n' || c == '\r';
	}

	TIKI_FORCE_INLINE bool isSmybol( char c )
	{
		return (c >= '!' && c <= '/') ||
			(c >= ':' && c <= '@') ||
			(c >= '[' && c <= '`') ||
			(c >= '{' && c <= '~');
	}

	TIKI_FORCE_INLINE char character::toLowerCase( char c )
	{
		TIKI_ASSERT( isCapital( c ) );
		return c + ('a' - 'A');
	}

	TIKI_FORCE_INLINE char character::toUpperCase( char c )
	{
		TIKI_ASSERT( isMinuscule( c ) );
		return c - ('a' - 'A');
	}
}

#endif // TIKI_CHARACTER_INL_INCLUDED
