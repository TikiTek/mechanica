#pragma once
#ifndef TIKI_STRING_INL_INCLUDED__
#define TIKI_STRING_INL_INCLUDED__

namespace tiki
{
	TIKI_FORCE_INLINE uint getStringSize( cstring pSource )
	{
		uint length = (uint)-1;		
		while ( pSource[ ++length ] != '\0' );

		return length;
	}

	TIKI_FORCE_INLINE uint getStringLength( const char* pSource )
	{
		uint index = 0;
		uint length = 0;

		while ( pSource[ index ] != '\0' )
		{
			length += !(pSource[ index ] & 0x80);
			index++;
		}

		return length;
	}

	TIKI_FORCE_INLINE uint copyString( char* pTargetBuffer, uint bufferSize, cstring pSourceBuffer )
	{
		const uint sourceLength = TIKI_MIN( bufferSize - 1u, getStringSize( pSourceBuffer ) );

		uint64* pTarget64 = reinterpret_cast< uint64* >( pTargetBuffer );
		const uint64* pSource64 = reinterpret_cast< const uint64* >( pSourceBuffer );

		uint length = 0u;
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
		pTargetBuffer[ length ] = '\0';

		return length;
	}
	
	TIKI_FORCE_INLINE bool isStringEquals( cstring pString1, cstring pString2 )
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

	TIKI_FORCE_INLINE uint stringIndexOf( cstring pString, char c, uint index /* = 0 */ )
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

	TIKI_FORCE_INLINE uint stringIndexOf( cstring pString, cstring pSearch, uint index /*= 0*/ )
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

	TIKI_FORCE_INLINE uint stringLastIndexOf( cstring pString, char c, uint index /*= TIKI_SIZE_T_MAX*/ )
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

	TIKI_FORCE_INLINE uint stringLastIndexOf( cstring pString, cstring pSearch, uint index /*= TIKI_SIZE_T_MAX*/ )
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
