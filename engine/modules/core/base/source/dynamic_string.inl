#pragma once

#include "tiki/base/memory.hpp"
#include "tiki/base/string.hpp"
#include "tiki/container/sized_array.hpp"

namespace tiki
{
	TIKI_FORCE_INLINE DynamicString::DynamicString()
	{
		m_pData			= nullptr;
		m_dataSize		= 0u;
		m_stringSize	= 0u;
	}

	TIKI_FORCE_INLINE DynamicString::DynamicString( uintreg length )
	{
		m_pData = nullptr;

		allocateData( length );
		m_stringSize = length;
	}

	TIKI_FORCE_INLINE DynamicString::DynamicString( const char* pString )
	{
		m_pData = nullptr;

		allocateDataForString( pString );
	}

	TIKI_FORCE_INLINE DynamicString::DynamicString( const char* pString, sint length )
	{
		m_pData = nullptr;

		allocateDataForString( pString, length );
		m_stringSize = length;
	}

	TIKI_FORCE_INLINE DynamicString::DynamicString( const DynamicString& copy )
	{
		m_pData = nullptr;

		allocateDataForString( copy.m_pData );
	}

	TIKI_FORCE_INLINE DynamicString::~DynamicString()
	{
		freeData();
	}

	TIKI_FORCE_INLINE void DynamicString::clear()
	{
		if( m_pData == nullptr )
		{
			return;
		}

		m_pData[ 0u ] = '\0';
		m_stringSize = 0u;
	}

	TIKI_FORCE_INLINE void DynamicString::split( Array< DynamicString >& output, const DynamicString& seperator ) const
	{
		const uintreg count = countSubstring( seperator );

		SizedArray< DynamicString > list;
		list.create( count + 1u );

		uintreg i = 0;
		uintreg lastIndex = 0;
		while( i < count )
		{
			const uintreg index = indexOf( seperator, lastIndex + seperator.m_stringSize );

			list.push(
				subString( lastIndex, sint( index - lastIndex ) )
			);

			lastIndex = index + seperator.m_stringSize;
			i++;
		}

		if( m_stringSize - lastIndex > 0 )
		{
			list.push(
				subString( lastIndex, sint( m_stringSize - lastIndex ) )
			);
		}

		output.create( list.getBegin(), list.getCount() );
		list.dispose();
	}

	TIKI_FORCE_INLINE DynamicString DynamicString::replace( char oldValue, char newValue ) const
	{
		DynamicString str = *this;

		uintreg i = 0;
		while( i < m_stringSize )
		{
			if( str[ i ] == oldValue )
			{
				str[ i ] = newValue;
			}

			i++;
		}

		return str;
	}

	TIKI_FORCE_INLINE DynamicString DynamicString::replace( const DynamicString& oldValue, const DynamicString& newValue ) const
	{
		const uintreg count = countSubstring( oldValue );
		const uintreg length = m_stringSize - (count * oldValue.m_stringSize) + (count * newValue.m_stringSize);

		if( count == 0 )
		{
			return *this;
		}

		DynamicString str( length );

		uintreg i = 0;
		uintreg offsetOld = 0;
		uintreg offsetNew = 0;
		while( i < count )
		{
			const uintreg index		= indexOf( oldValue, offsetOld );
			const uintreg oldDifferent	= index - offsetOld;

			memory::copy( str.m_pData + offsetNew, m_pData + offsetOld, index - offsetOld );
			offsetOld += oldDifferent;
			offsetNew += oldDifferent;

			memory::copy( str.m_pData + offsetNew, newValue.m_pData, newValue.m_stringSize );
			offsetOld += oldValue.m_stringSize;
			offsetNew += newValue.m_stringSize;

			i++;
		}

		memory::copy( str.m_pData + offsetNew, m_pData + offsetOld, m_stringSize - offsetOld );
		str.m_pData[ str.m_stringSize ] = '\0';

		return str;
	}

	TIKI_FORCE_INLINE DynamicString DynamicString::subString( uintreg startIndex, sint length /*= -1*/ ) const
	{
		if( length == -1 || startIndex + length > m_stringSize )
		{
			length = m_stringSize - startIndex;
		}

		if( length <= 0 )
		{
			return DynamicString();
		}
		TIKI_ASSERT( startIndex < m_stringSize );

		if( length == m_stringSize && startIndex == 0u )
		{
			return DynamicString( *this );
		}

		return DynamicString(
			m_pData + startIndex,
			length
		);
	}

	TIKI_FORCE_INLINE DynamicString DynamicString::trim() const
	{
		if( isEmpty() )
		{
			return *this;
		}

		uintreg start = 0u;
		uintreg length = m_stringSize;
		while( ascii::isWhitespace( m_pData[ start ] ) )
		{
			start++;
			length--;
		}

		while( ascii::isWhitespace( m_pData[ start + length - 1u ] ) )
		{
			length--;
		}

		return subString( start, length );
	}

	TIKI_FORCE_INLINE uintreg DynamicString::countSubstring( const DynamicString& str, AsciiCompareType matchType /*= AsciiCompareType::CaseSensitive*/ ) const
	{
		if( str.m_stringSize > m_stringSize )
		{
			return 0u;
		}

		uintreg i = 0;
		uintreg c = 0;
		while( i < m_stringSize )
		{
			uintreg b = 0;
			bool found = true;
			while( b < str.m_stringSize )
			{
				if( !ascii::isCharacterEqual( m_pData[ i + b ], str.m_pData[ b ], matchType ) )
				{
					found = false;
					break;
				}
				b++;
			}

			if( found )
			{
				c++;
				i += str.m_stringSize;
			}
			else
			{
				i++;
			}
		}

		return c;
	}

	TIKI_FORCE_INLINE DynamicString DynamicString::insert( const DynamicString& str, uintreg index ) const
	{
		DynamicString oStr = DynamicString( m_stringSize + str.m_stringSize );

		memory::copy( oStr.m_pData, m_pData, sizeof( char ) * index );
		memory::copy( oStr.m_pData + index, str.m_pData, sizeof( char ) * str.m_stringSize );
		memory::copy( oStr.m_pData + index + str.m_stringSize, m_pData + index, sizeof( char ) * (m_stringSize - index) );
		oStr.m_pData[ oStr.m_stringSize ] = '\0';

		return oStr;
	}

	TIKI_FORCE_INLINE DynamicString DynamicString::remove( uintreg startIndex, uintreg length ) const
	{
		DynamicString str = DynamicString( m_stringSize - length );

		memory::copy( str.m_pData, m_pData, sizeof( char ) * startIndex );
		memory::copy( str.m_pData + startIndex, m_pData + startIndex + length, sizeof( char ) * (m_stringSize - startIndex - length) );
		str.m_pData[ str.m_stringSize ] = '\0';

		return str;
	}

	TIKI_FORCE_INLINE DynamicString DynamicString::toLower() const
	{
		DynamicString str = *this;

		for( uintreg i = 0u; i < m_stringSize; ++i )
		{
			if( !ascii::isCapital( str[ i ] ) )
			{
				continue;
			}

			str[ i ] = ascii::toLower( str[ i ] );
		}

		return str;
	}

	TIKI_FORCE_INLINE DynamicString DynamicString::toUpper() const
	{
		DynamicString str = *this;

		for( uintreg i = 0u; i < m_stringSize; ++i )
		{
			if( !ascii::isMinuscule( str[ i ] ) )
			{
				continue;
			}

			str[ i ] = ascii::toUpper( str[ i ] );
		}

		return str;
	}

	TIKI_FORCE_INLINE int DynamicString::indexOf( char c, uintreg index /*= 0u*/ ) const
	{
		TIKI_ASSERT( index < m_stringSize || m_stringSize == 0u );

		uintreg i = index;
		while( i < m_stringSize )
		{
			if( m_pData[ i ] == c )
			{
				return int( i );
			}
			i++;
		}

		return -1;
	}

	TIKI_FORCE_INLINE int DynamicString::indexOf( const DynamicString& str, uintreg index /*= 0u*/, AsciiCompareType matchType /*= AsciiCompareType::CaseSensitive*/ ) const
	{
		if( str.m_stringSize > m_stringSize )
		{
			return -1;
		}

		uintreg i = index;
		uintreg c = m_stringSize - str.m_stringSize;

		do
		{
			uintreg b = 0;
			bool found = true;
			while( b < str.m_stringSize )
			{
				if( !ascii::isCharacterEqual( m_pData[ i + b ], str.m_pData[ b ], matchType ) )
				{
					found = false;
					break;
				}
				b++;
			}

			if( found )
			{
				return int( i );
			}

			i++;
		}
		while( i <= c );

		return -1;
	}

	TIKI_FORCE_INLINE int DynamicString::lastIndexOf( char c, uintreg index /*= 0xffffffffu*/ ) const
	{
		index = TIKI_MIN( index, m_stringSize - 1u );
		while( index < m_stringSize )
		{
			if( m_pData[ index ] == c )
			{
				return (int)index;
			}

			index--;
		}

		return -1;
	}

	TIKI_FORCE_INLINE int DynamicString::lastIndexOf( const DynamicString& str, uintreg index /*= 0xffffffffu*/, AsciiCompareType matchType /*= AsciiCompareType::CaseSensitive*/ ) const
	{
		index = TIKI_MIN( index, m_stringSize - 1u );
		while( index < m_stringSize )
		{
			uintreg b = 0;
			bool found = true;
			while( b < str.m_stringSize )
			{
				if( !ascii::isCharacterEqual( m_pData[ index + b ], str.m_pData[ b ], matchType ) )
				{
					found = false;
					break;
				}

				b++;
			}

			if( found )
			{
				return (int)index;
			}

			index--;
		}

		return -1;
	}

	TIKI_FORCE_INLINE bool DynamicString::contains( char c ) const
	{
		return indexOf( c ) != -1;
	}

	TIKI_FORCE_INLINE bool DynamicString::contains( const DynamicString& str, AsciiCompareType matchType /*= AsciiCompareType::CaseSensitive*/ ) const
	{
		return indexOf( str, 0u, matchType ) != -1;
	}

	TIKI_FORCE_INLINE bool DynamicString::startsWith( char c ) const
	{
		if( m_stringSize < 1 ) return false;

		return m_pData[ 0 ] == c;
	}

	TIKI_FORCE_INLINE bool DynamicString::startsWith( const DynamicString& str, AsciiCompareType matchType /*= AsciiCompareType::CaseSensitive*/ ) const
	{
		if( m_stringSize < str.m_stringSize )
		{
			return false;
		}

		uintreg i = 0;
		while( i < str.m_stringSize )
		{
			if( !ascii::isCharacterEqual( m_pData[ i ], str.m_pData[ i ], matchType ) )
			{
				return false;
			}
			i++;
		}

		return true;
	}

	TIKI_FORCE_INLINE bool DynamicString::endsWith( char c ) const
	{
		if( m_stringSize < 1 ) return false;

		return m_pData[ m_stringSize - 1 ] == c;
	}

	TIKI_FORCE_INLINE bool DynamicString::endsWith( const DynamicString& str, AsciiCompareType matchType /*= AsciiCompareType::CaseSensitive*/ ) const
	{
		if( m_stringSize < str.m_stringSize )
		{
			return false;
		}

		uintreg b = 0;
		uintreg i = m_stringSize - str.m_stringSize;
		while( i < m_stringSize )
		{
			if( !ascii::isCharacterEqual( m_pData[ i ], str.m_pData[ b ], matchType ) )
			{
				return false;
			}
			i++;
			b++;
		}

		return true;
	}

	TIKI_FORCE_INLINE const char* DynamicString::operator*() const
	{
		return m_pData;
	}

	TIKI_FORCE_INLINE char DynamicString::operator[]( uintreg index ) const
	{
		TIKI_ASSERT( index < m_stringSize );
		return m_pData[ index ];
	}

	TIKI_FORCE_INLINE char& DynamicString::operator[]( uintreg index )
	{
		TIKI_ASSERT( index < m_stringSize );
		return m_pData[ index ];
	}

	TIKI_FORCE_INLINE bool DynamicString::operator==( const DynamicString& rhs ) const
	{
		if( m_stringSize != rhs.m_stringSize )
		{
			return false;
		}

		uintreg i = 0;
		while( i < m_stringSize )
		{
			if( m_pData[ i ] != rhs.m_pData[ i ] )
			{
				return false;
			}
			i++;
		}

		return true;
	}

	TIKI_FORCE_INLINE bool DynamicString::operator!=( const DynamicString& rhs ) const
	{
		return !(*this == rhs);
	}

	TIKI_FORCE_INLINE DynamicString& DynamicString::operator=( const DynamicString& rhs )
	{
		freeData();
		allocateDataForString( rhs.m_pData );

		return *this;
	}

	TIKI_FORCE_INLINE DynamicString DynamicString::operator+( const DynamicString& rhs ) const
	{
		uintreg length = m_stringSize + rhs.m_stringSize;
		DynamicString str = DynamicString( length );

		memory::copy( str.m_pData, m_pData, m_stringSize );
		memory::copy( str.m_pData + m_stringSize, rhs.m_pData, rhs.m_stringSize );
		str.m_pData[ length ] = '\0';

		return str;
	}

	TIKI_FORCE_INLINE DynamicString& DynamicString::operator+=( const DynamicString& rhs )
	{
		const uintreg sl = m_stringSize;
		const uintreg length = m_stringSize + rhs.m_stringSize;

		if( m_dataSize <= length )
		{
			reallocateData( length );
		}

		memory::copy( m_pData + sl, rhs.m_pData, rhs.m_stringSize );
		m_pData[ length ] = 0;
		m_stringSize = length;

		return *this;
	}

	TIKI_FORCE_INLINE DynamicString& DynamicString::operator+=( char c )
	{
		if( m_dataSize == m_stringSize )
		{
			reallocateData( m_stringSize + 1u );
		}

		m_pData[ m_stringSize++ ] = c;
		m_pData[ m_stringSize ] = '\0';

		return *this;
	}

	TIKI_FORCE_INLINE bool DynamicString::operator>( const DynamicString& rhs ) const
	{
		uintreg i = 0;
		uintreg c = (m_stringSize < rhs.m_stringSize ? m_stringSize : rhs.m_stringSize);
		while( i < c && m_pData[ i ] == rhs.m_pData[ i ] )
		{
			i++;
		}

		return m_pData[ i ] > rhs.m_pData[ i ];
	}

	TIKI_FORCE_INLINE bool DynamicString::operator>=( const DynamicString& rhs ) const
	{
		return (*this == rhs) || (*this > rhs);
	}

	TIKI_FORCE_INLINE bool DynamicString::operator<( const DynamicString& rhs ) const
	{
		uintreg i = 0;
		uintreg c = (m_stringSize < rhs.m_stringSize ? m_stringSize : rhs.m_stringSize);
		while( i < c && m_pData[ i ] == rhs.m_pData[ i ] )
		{
			i++;
		}

		return m_pData[ i ] < rhs.m_pData[ i ];
	}

	TIKI_FORCE_INLINE bool DynamicString::operator<=( const DynamicString& rhs ) const
	{
		return (*this == rhs) || (*this < rhs);
	}

	TIKI_FORCE_INLINE void DynamicString::allocateData( sint length )
	{
		TIKI_ASSERT( m_pData == nullptr );

		m_dataSize		= calculateLength( length );
		m_stringSize	= 0u;

		m_pData			= (char*)TIKI_ALLOC( m_dataSize );
	}

	TIKI_FORCE_INLINE void DynamicString::reallocateData( sint length )
	{
		char* pOldData = m_pData;

		m_dataSize	= calculateLength( length );
		m_pData		= (char*)TIKI_ALLOC( m_dataSize );

		memory::copy( m_pData, pOldData, m_stringSize );
		m_pData[ m_stringSize ] = '\0';

		TIKI_FREE( pOldData );
	}

	TIKI_FORCE_INLINE void DynamicString::allocateDataForString( const char* pString, sint length /* = -1 */ )
	{
		if( pString == nullptr )
		{
			freeData();
		}
		else
		{
			uintreg stringSize = getStringSize( pString );
			stringSize = TIKI_MIN( stringSize, (uintreg)length );
			TIKI_ASSERT( length == -1 || stringSize <= (uintreg)length );

			const uintreg dataSize = (length == -1 ? stringSize : (uintreg)length);

			allocateData( dataSize );
			memory::copy( m_pData, pString, dataSize );
			m_pData[ dataSize ] = '\0';

			m_stringSize = dataSize;
		}
	}

	TIKI_FORCE_INLINE void DynamicString::freeData()
	{
		TIKI_FREE( m_pData );
		m_pData			= nullptr;
		m_dataSize		= 0u;
		m_stringSize	= 0u;
	}

	TIKI_FORCE_INLINE uintreg DynamicString::calculateLength( uintreg neededLength ) const
	{
		const uintreg nextSize = getNextPowerOfTwo( neededLength + 1u );
		TIKI_ASSERT( nextSize > neededLength );
		return nextSize;
	}

	TIKI_FORCE_INLINE bool operator==( const char* str1, const DynamicString& str2 )
	{
		return isStringEquals( str1, str2.cStr() );
	}

	TIKI_FORCE_INLINE bool operator==( const DynamicString& str1, const char* str2 )
	{
		return isStringEquals( str1.cStr(), str2 );
	}

	TIKI_FORCE_INLINE DynamicString operator+( const char* str1, const DynamicString& str2 )
	{
		return DynamicString( str1 ) + str2;
	}
}
