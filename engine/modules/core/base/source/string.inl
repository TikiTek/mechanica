#pragma once
#ifndef TIKI_STRING_INL_INCLUDED__
#define TIKI_STRING_INL_INCLUDED__

#include "tiki/base/assert.hpp"
#include "tiki/base/memory.hpp"

namespace tiki
{
	TIKI_FORCE_INLINE uint getStringSize( const char* pSource )
	{
		if( pSource == nullptr )
		{
			return 0u;
		}

		uint length = (uint)-1;		
		while ( pSource[ ++length ] != '\0' );

		return length;
	}

	TIKI_FORCE_INLINE uint getStringLength( const char* pSource )
	{
		if( pSource == nullptr )
		{
			return 0u;
		}

		uint index = 0;
		uint length = 0;
		while ( pSource[ index ] != '\0' )
		{
			length += !(pSource[ index ] & 0x80); // wrong!
			index++;
		}

		return length;
	}

	TIKI_FORCE_INLINE uint copyString( char* pTargetBuffer, uint bufferSize, const char* pSourceBuffer )
	{
		TIKI_ASSERT( pTargetBuffer != nullptr );
		
		uint length = 0u;
		if ( pSourceBuffer != nullptr )
		{
			const uint sourceLength = TIKI_MIN( bufferSize - 1u, getStringSize( pSourceBuffer ) );

			uint64* pTarget64 = reinterpret_cast< uint64* >( pTargetBuffer );
			const uint64* pSource64 = reinterpret_cast< const uint64* >( pSourceBuffer );

			while ( length < sourceLength )
			{
				if ( ( sourceLength -  length ) >= sizeof( uint64 ) )
				{
					const uint lengthOver = length / sizeof( uint64 );
					pTarget64[ lengthOver ] = pSource64[ lengthOver ];

					length += sizeof( uint64 );
				}
				else
				{
					pTargetBuffer[ length ] = pSourceBuffer[ length ];
					++length;
				}
			}
		}
		pTargetBuffer[ length ] = '\0';

		return length;
	}

	TIKI_FORCE_INLINE uint appendString( char* pTargetbuffer, uint bufferSize, const char* pSource )
	{
		const uint index = getStringSize( pTargetbuffer );
		return copyString( pTargetbuffer + index, bufferSize - index, pSource );
	}

	TIKI_FORCE_INLINE char* dublicateString( const char* pSource )
	{
		const uint sourceLength = getStringSize( pSource );

		char* pNewString = (char*)TIKI_ALLOC( sourceLength + 1u );
		copyString( pNewString, sourceLength + 1u, pSource );

		return pNewString;
	}

	TIKI_FORCE_INLINE bool isStringEmpty( const char* pString )
	{
		return pString == nullptr || pString[ 0u ] == '\0';
	}
	
	TIKI_FORCE_INLINE bool isStringEquals( const char* pString1, const char* pString2 )
	{
		if ( pString1 == pString2 )
		{
			return true;
		}
		else if ( pString1 == nullptr || pString2 == nullptr )
		{
			return false;
		}

		uint index = 0u;
		while ( pString1[ index ] != '\0' && pString2[ index ] != '\0' )
		{
			if ( pString1[ index ] != pString2[ index ] )
			{
				return false;
			}

			index++;
		}

		return pString1[ index ] == pString2[ index ] != '\0';
	}

	TIKI_FORCE_INLINE void stringReplace( char* pString, char oldChar, char newChar )
	{
		const uint stringSize = getStringSize( pString );
		for( uint i = 0u; i < stringSize; ++i )
		{
			if( pString[ i ] == oldChar )
			{
				pString[ i ] = newChar;
			}
		}
	}

	TIKI_FORCE_INLINE uint stringIndexOf( const char* pString, char c, uint index /* = 0 */ )
	{
		uint i = index;
		while ( pString[ i ] != '\0' )
		{
			if ( pString[ i ] == c )
			{
				return i;
			}

			i++;
		}

		return TIKI_SIZE_T_MAX;
	}

	TIKI_FORCE_INLINE uint stringIndexOf( const char* pString, const char* pSearch, uint index /*= 0*/ )
	{
		const uint stringLength = getStringSize( pString );
		const uint searchLength = getStringSize( pSearch );

		if ( searchLength > stringLength )
		{
			return TIKI_SIZE_T_MAX;
		}

		uint stringFoundIndex = 0u;
		uint stringIndex = index;
		uint searchIndex = 0u;
		while ( pString[ stringIndex ] != '\0' )
		{
			if ( pString[ stringIndex ] == pSearch[ searchIndex ] )
			{
				if ( searchIndex == 0u )
				{
					stringFoundIndex = stringIndex;
				}

				searchIndex++;

				if ( searchIndex == searchLength )
				{
					return stringFoundIndex;
				}
			}
			else
			{
				if ( searchIndex > 0 )
				{
					stringIndex = stringFoundIndex + 1u;
				}

				searchIndex = 0u;
			}

			stringIndex++;
		}

		return TIKI_SIZE_T_MAX;
	}

	TIKI_FORCE_INLINE uint stringLastIndexOf( const char* pString, char c, uint index /*= TIKI_SIZE_T_MAX*/ )
	{
		const uint stringLength = getStringSize( pString );

		index = TIKI_MIN( index, stringLength - 1 );
		while (index < stringLength)
		{
			if ( pString[ index ] == c )
			{
				return index;
			}

			index--;
		}

		return TIKI_SIZE_T_MAX;
	}

	TIKI_FORCE_INLINE uint stringLastIndexOf( const char* pString, const char* pSearch, uint index /*= TIKI_SIZE_T_MAX*/ )
	{
		const uint stringLength = getStringSize( pString );
		const uint searchLength = getStringSize( pSearch );

		const uint maxSearchIndex = searchLength - 1u;

		if ( searchLength > stringLength )
		{
			return TIKI_SIZE_T_MAX;
		}

		uint stringFoundIndex = 0u;
		uint stringIndex = TIKI_MIN( index, stringLength - 1 );
		uint searchIndex = 0u;
		while ( index < stringLength )
		{
			if ( pString[ stringIndex ] == pSearch[ searchIndex ] )
			{
				if ( searchIndex == maxSearchIndex )
				{
					stringFoundIndex = stringIndex;
				}

				searchIndex--;

				if ( searchIndex == 0u )
				{
					return stringFoundIndex;
				}
			}
			else
			{
				if ( searchIndex < maxSearchIndex )
				{
					stringIndex = stringFoundIndex - 1u;
				}

				searchIndex = maxSearchIndex;
			}

			stringIndex--;
		}

		return -1;
	}
}

#endif // TIKI_STRING_INL_INCLUDED__
