#pragma once
#ifndef TIKI_BASICSTRING_INL_INCLUDED__
#define TIKI_BASICSTRING_INL_INCLUDED__

#include "tiki/base/memory.hpp"
#include "tiki/container/sizedarray.hpp"
#include "tiki/base/string.hpp"

namespace tiki
{
	TIKI_FORCE_INLINE BasicString::BasicString()
	{
		m_pData			= nullptr;
		m_dataSize		= 0u;
		m_stringSize	= 0u;
	}

	TIKI_FORCE_INLINE BasicString::BasicString( uint length )
	{
		m_pData = nullptr;

		allocateData( length );
		m_stringSize = length;
	}

	TIKI_FORCE_INLINE BasicString::BasicString( const char* pString )
	{
		m_pData = nullptr;

		allocateDataForString( pString );
	}

	TIKI_FORCE_INLINE BasicString::BasicString( const char* pString, sint length )
	{
		m_pData = nullptr;

		allocateDataForString( pString, length );
		m_stringSize = length;
	}

	TIKI_FORCE_INLINE BasicString::BasicString( const BasicString& copy )
	{
		m_pData = nullptr;
		
		allocateDataForString( copy.m_pData );
	}

	TIKI_FORCE_INLINE BasicString::~BasicString()
	{
		freeData();
	}

	TIKI_FORCE_INLINE uint BasicString::getLength() const
	{
		return m_stringSize;
	}

	TIKI_FORCE_INLINE bool BasicString::isEmpty() const
	{
		return m_stringSize == 0;
	}

	TIKI_FORCE_INLINE char* BasicString::getBuffer()
	{
		return m_pData;
	}

	TIKI_FORCE_INLINE const char* BasicString::cStr() const
	{
		return m_pData;
	}

	TIKI_FORCE_INLINE void BasicString::split( Array< BasicString >& output, const BasicString& seperator ) const
	{
		const uint count = countSubstring( seperator );

		SizedArray< BasicString > list;
		list.create( count + 1u );

		uint i = 0;
		uint lastIndex = 0;
		while( i < count )
		{
			const uint index = indexOf( seperator, lastIndex + seperator.m_stringSize );

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

	TIKI_FORCE_INLINE BasicString BasicString::replace( char oldValue, char newValue ) const
	{
		BasicString str = *this;

		uint i = 0;
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

	TIKI_FORCE_INLINE BasicString BasicString::replace( const BasicString& oldValue, const BasicString& newValue ) const
	{
		const uint count = countSubstring( oldValue );
		const uint length = m_stringSize - (count * oldValue.m_stringSize) + (count * newValue.m_stringSize);

		if( count == 0 )
		{
			return *this;
		}

		BasicString str( length );

		uint i = 0;
		uint offsetOld = 0;
		uint offsetNew = 0;
		while( i < count )
		{
			const uint index		= indexOf( oldValue, offsetOld );
			const uint oldDifferent	= index - offsetOld;

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

	TIKI_FORCE_INLINE BasicString BasicString::subString( uint startIndex, sint length /*= -1*/ ) const
	{
		if( length == -1 || startIndex + length > m_stringSize )
		{
			length = m_stringSize - startIndex;
		}

		if( length <= 0 )
		{
			return BasicString();
		}
		TIKI_ASSERT( startIndex < m_stringSize );

		if( length == m_stringSize && startIndex == 0u )
		{
			return BasicString( *this );
		}

		return BasicString(
			m_pData + startIndex,
			length
		);
	}

	TIKI_FORCE_INLINE BasicString BasicString::trim() const
	{
		uint start = 0u;
		uint length = m_stringSize;

		if( length == 0u )
		{
			return *this;
		}

		bool isWhiteSpace = false;
		do
		{
			isWhiteSpace = false;

			for( uint i = 0u; i < TIKI_COUNT( whiteSpaces ); ++i )
			{
				if( m_pData[ start ] == whiteSpaces[ i ] )
				{
					isWhiteSpace = true;
					start++;
					length--;
					break;
				}
			}
		}
		while( isWhiteSpace );

		do
		{
			isWhiteSpace = false;

			for( uint i = 0u; i < TIKI_COUNT( whiteSpaces ); ++i )
			{
				if( m_pData[ start + length - 1u ] == whiteSpaces[ i ] )
				{
					isWhiteSpace = true;
					length--;
					break;
				}
			}
		}
		while( isWhiteSpace );

		return subString( start, length );
	}

	TIKI_FORCE_INLINE uint BasicString::countSubstring( const BasicString& str ) const
	{
		if( str.m_stringSize > m_stringSize )
			return 0u;

		uint i = 0;
		uint c = 0;
		while( i < m_stringSize )
		{
			uint b = 0;
			bool found = true;
			while( b < str.m_stringSize )
			{
				if( m_pData[ i + b ] != str.m_pData[ b ] )
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

	TIKI_FORCE_INLINE BasicString BasicString::insert( const BasicString& str, uint index ) const
	{
		BasicString oStr = BasicString( m_stringSize + str.m_stringSize );

		memory::copy( oStr.m_pData, m_pData, sizeof( char ) * index );
		memory::copy( oStr.m_pData + index, str.m_pData, sizeof( char ) * str.m_stringSize );
		memory::copy( oStr.m_pData + index + str.m_stringSize, m_pData + index, sizeof( char ) * (m_stringSize - index) );
		oStr.m_pData[ oStr.m_stringSize ] = '\0';

		return oStr;
	}

	TIKI_FORCE_INLINE BasicString BasicString::remove( uint startIndex, uint length ) const
	{
		BasicString str = BasicString( m_stringSize - length );

		memory::copy( str.m_pData, m_pData, sizeof( char ) * startIndex );
		memory::copy( str.m_pData + startIndex, m_pData + startIndex + length, sizeof( char ) * (m_stringSize - startIndex - length) );
		str.m_pData[ str.m_stringSize ] = '\0';

		return str;
	}

	TIKI_FORCE_INLINE BasicString BasicString::toLower() const
	{
		BasicString str = *this;

		uint i = 0;
		while( i < m_stringSize )
		{
			if( str[ i ] >= 'A' && str[ i ] <= 'Z' )
				str[ i ] -= 'Z' - 'z';

			i++;
		}

		return str;
	}

	TIKI_FORCE_INLINE BasicString BasicString::toUpper() const
	{
		BasicString str = *this;

		uint i = 0;
		while( i < m_stringSize )
		{
			if( str[ i ] >= 'a' && str[ i ] <= 'z' )
				str[ i ] -= 'z' - 'Z';

			i++;
		}

		return str;
	}

	TIKI_FORCE_INLINE int BasicString::indexOf( char c ) const
	{
		return indexOf( c, 0 );
	}

	TIKI_FORCE_INLINE int BasicString::indexOf( char c, uint index ) const
	{
		uint i = index;
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

	TIKI_FORCE_INLINE int BasicString::indexOf( const BasicString& str ) const
	{
		return indexOf( str, 0 );
	}

	TIKI_FORCE_INLINE int BasicString::indexOf( const BasicString& str, uint index ) const
	{
		if( str.m_stringSize > m_stringSize ) return -1;

		uint i = index;
		uint c = m_stringSize - str.m_stringSize;

		do
		{
			uint b = 0;
			bool found = true;
			while( b < str.m_stringSize )
			{
				if( m_pData[ i + b ] != str.m_pData[ b ] )
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

	TIKI_FORCE_INLINE int BasicString::lastIndexOf( char c ) const
	{
		return lastIndexOf( c, m_stringSize - 1u );
	}

	TIKI_FORCE_INLINE int BasicString::lastIndexOf( char c, uint index ) const
	{
		int i = int( index );
		while( i >= 0 )
		{
			if( m_pData[ i ] == c ) return i;
			i--;
		}

		return -1;
	}

	TIKI_FORCE_INLINE int BasicString::lastIndexOf( const BasicString& str ) const
	{
		return lastIndexOf( str, m_stringSize - str.m_stringSize );
	}

	TIKI_FORCE_INLINE int BasicString::lastIndexOf( const BasicString& str, uint index ) const
	{
		int i = (int)index;
		while( i >= 0 )
		{
			int b = 0;
			bool found = true;
			while( b < (int)str.m_stringSize )
			{
				if( m_pData[ i + b ] != str.m_pData[ b ] )
				{
					found = false;
					break;
				}
				b++;
			}

			if( found )
			{
				return i;
			}

			i--;
		}

		return -1;
	}

	TIKI_FORCE_INLINE bool BasicString::contains( char c ) const
	{
		return indexOf( c ) != -1;
	}

	TIKI_FORCE_INLINE bool BasicString::contains( const BasicString& str ) const
	{
		return indexOf( str ) != -1;
	}

	TIKI_FORCE_INLINE bool BasicString::startsWith( char c ) const
	{
		if( m_stringSize < 1 ) return false;

		return m_pData[ 0 ] == c;
	}

	TIKI_FORCE_INLINE bool BasicString::startsWith( const BasicString& str ) const
	{
		if( m_stringSize < str.m_stringSize ) return false;

		uint i = 0;
		while( i < str.m_stringSize )
		{
			if( m_pData[ i ] != str.m_pData[ i ] ) return false;
			i++;
		}

		return true;
	}

	TIKI_FORCE_INLINE bool BasicString::endsWith( char c ) const
	{
		if( m_stringSize < 1 ) return false;

		return m_pData[ m_stringSize - 1 ] == c;
	}

	TIKI_FORCE_INLINE bool BasicString::endsWith( const BasicString& str ) const
	{
		if( m_stringSize < str.m_stringSize ) return false;

		uint b = 0;
		uint i = m_stringSize - str.m_stringSize;
		while( i < m_stringSize )
		{
			if( m_pData[ i ] != str.m_pData[ b ] ) return false;
			i++;
			b++;
		}

		return true;
	}

	TIKI_FORCE_INLINE const char* BasicString::operator*() const
	{
		return m_pData;
	}

	TIKI_FORCE_INLINE char BasicString::operator[]( uint index ) const
	{
		TIKI_ASSERT( index < m_stringSize );
		return m_pData[ index ];
	}

	TIKI_FORCE_INLINE char& BasicString::operator[]( uint index )
	{
		TIKI_ASSERT( index < m_stringSize );
		return m_pData[ index ];
	}

	TIKI_FORCE_INLINE bool BasicString::operator==( const BasicString& rhs ) const
	{
		if( m_stringSize != rhs.m_stringSize )
		{
			return false;
		}

		uint i = 0;
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

	TIKI_FORCE_INLINE bool BasicString::operator!=( const BasicString& rhs ) const
	{
		return !(*this == rhs);
	}

	TIKI_FORCE_INLINE BasicString& BasicString::operator=( const BasicString& rhs )
	{
		freeData();
		allocateDataForString( rhs.m_pData );

		return *this;
	}

	TIKI_FORCE_INLINE BasicString BasicString::operator+( const BasicString& rhs ) const
	{
		uint length = m_stringSize + rhs.m_stringSize;
		BasicString str = BasicString( length );

		memory::copy( str.m_pData, m_pData, m_stringSize );
		memory::copy( str.m_pData + m_stringSize, rhs.m_pData, rhs.m_stringSize );
		str.m_pData[ length ] = '\0';

		return str;
	}

	TIKI_FORCE_INLINE BasicString& BasicString::operator+=( const BasicString& rhs )
	{
		const uint sl = m_stringSize;
		const uint length = m_stringSize + rhs.m_stringSize;

		if( m_dataSize <= length )
		{
			reallocateData( length );
		}

		memory::copy( m_pData + sl, rhs.m_pData, rhs.m_stringSize );
		m_pData[ length ] = 0;
		m_stringSize = length;

		return *this;
	}

	TIKI_FORCE_INLINE bool BasicString::operator>( const BasicString& rhs ) const
	{
		uint i = 0;
		uint c = (m_stringSize < rhs.m_stringSize ? m_stringSize : rhs.m_stringSize);
		while( i < c && m_pData[ i ] == rhs.m_pData[ i ] )
		{
			i++;
		}

		return m_pData[ i ] > rhs.m_pData[ i ];
	}

	TIKI_FORCE_INLINE bool BasicString::operator>=( const BasicString& rhs ) const
	{
		return (*this == rhs) || (*this > rhs);
	}

	TIKI_FORCE_INLINE bool BasicString::operator<( const BasicString& rhs ) const
	{
		uint i = 0;
		uint c = (m_stringSize < rhs.m_stringSize ? m_stringSize : rhs.m_stringSize);
		while( i < c && m_pData[ i ] == rhs.m_pData[ i ] )
		{
			i++;
		}

		return m_pData[ i ] < rhs.m_pData[ i ];
	}

	TIKI_FORCE_INLINE bool BasicString::operator<=( const BasicString& rhs ) const
	{
		return (*this == rhs) || (*this < rhs);
	}

	TIKI_FORCE_INLINE string operator+( const char* str1, const string& str2 )
	{
		return string( str1 ) + str2;
	}

	TIKI_FORCE_INLINE void BasicString::allocateData( sint length )
	{
		TIKI_ASSERT( m_pData == nullptr );

		m_dataSize		= calculateLength( length );
		m_stringSize	= 0u;

		m_pData			= (char*)TIKI_ALLOC( m_dataSize );
	}
	
	TIKI_FORCE_INLINE void BasicString::reallocateData( sint length )
	{
		char* pOldData = m_pData;
		
		m_dataSize	= calculateLength( length );
		m_pData		= (char*)TIKI_ALLOC( m_dataSize );

		memory::copy( m_pData, pOldData, m_stringSize );
		m_pData[ m_stringSize ] = '\0';

		TIKI_FREE( pOldData );
	}

	TIKI_FORCE_INLINE void BasicString::allocateDataForString( const char* pString, sint length /* = -1 */ )
	{
		if( pString == nullptr )
		{
			freeData();
		}
		else
		{
			uint stringSize = getStringSize( pString );
			stringSize = TIKI_MIN( stringSize, (uint)length );
			TIKI_ASSERT( length == -1 || stringSize <= (uint)length );

			const uint dataSize = (length == -1 ? stringSize : (uint)length);

			allocateData( dataSize );
			memory::copy( m_pData, pString, dataSize );
			m_pData[ dataSize ] = '\0';

			m_stringSize = dataSize;
		}
	}

	TIKI_FORCE_INLINE void BasicString::freeData()
	{
		TIKI_FREE( m_pData );
		m_pData			= nullptr;
		m_dataSize		= 0u;
		m_stringSize	= 0u;
	}

	TIKI_FORCE_INLINE uint BasicString::calculateLength( uint neededLength ) const
	{
		const uint nextSize = getNextPowerOfTwo( neededLength + 1u );
		TIKI_ASSERT( nextSize > neededLength );
		return nextSize;
	}
}

#endif // TIKI_BASICSTRING_INL_INCLUDED__
