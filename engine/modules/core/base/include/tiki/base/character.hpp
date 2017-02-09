#pragma once
#ifndef TIKI_CHARACTER_HPP_INCLUDED
#define TIKI_CHARACTER_HPP_INCLUDED

namespace tiki
{
	namespace character
	{
		TIKI_FORCE_INLINE bool		isNumber( char c );
		TIKI_FORCE_INLINE bool		isLetter( char c );
		TIKI_FORCE_INLINE bool		isMinuscule( char c );
		TIKI_FORCE_INLINE bool		isCapital( char c );
		TIKI_FORCE_INLINE bool		isAlphaNumeric( char c );
		TIKI_FORCE_INLINE bool		isWhitespace( char c );
		TIKI_FORCE_INLINE bool		isSmybol( char c );

		TIKI_FORCE_INLINE char		toLowerCase( char c );
		TIKI_FORCE_INLINE char		toUpperCase( char c );
	}
}

#include "../../../source/character.inl"

#endif // TIKI_CHARACTER_HPP_INCLUDED
