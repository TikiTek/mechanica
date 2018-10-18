#pragma once
#ifndef TIKI_BASICSTRING_INL_INCLUDED__
#define TIKI_BASICSTRING_INL_INCLUDED__

#include "tiki/base/character.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/base/string.hpp"
#include "tiki/container/sizedarray.hpp"

namespace tiki
{
	TIKI_FORCE_INLINE DynamicString::DynamicString()
	{
		m_pData			= nullptr;
		m_dataSize		= 0u;
		m_stringSize	= 0u;
	}

	TIKI_FORCE_INLINE DynamicString::DynamicString( uint length )
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

	TIKI_FORCE_INLINE bool DynamicString::isEmpty() const
	{
		return m_stringSize == 0;
	}

	TIKI_FORCE_INLINE uint DynamicString::getLength() const
	{
		return m_stringSize;
	}

	TIKI_FORCE_INLINE char* DynamicString::getBuffer()
	{
		return m_pData;
	}

	TIKI_FORCE_INLINE const char* DynamicString::cStr() const
	{
		return m_pData;
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
		const uint count = countSubstring( seperator );

		SizedArray< DynamicString > list;
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

	TIKI_FORCE_INLINE DynamicString DynamicString::replace( char oldValue, char newValue ) const
	{
		DynamicString str = *this;

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

	TIKI_FORCE_INLINE DynamicString DynamicString::replace( const DynamicString& oldValue, const DynamicString& newValue ) const
	{
		const uint count = countSubstring( oldValue );
		const uint length = m_stringSize - (count * oldValue.m_stringSize) + (count * newValue.m_stringSize);

		if( count == 0 )
		{
			return *this;
		}

		DynamicString str( length );

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

	TIKI_FORCE_INLINE DynamicString DynamicString::subString( uint startIndex, sint length /*= -1*/ ) const
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

		uint start = 0u;
		uint length = m_stringSize;
		while( character::isWhitespace( m_pData[ start ] ) )
		{
			start++;
			length--;
		}

		while( character::isWhitespace( m_pData[ start + length - 1u ] ) )
		{
			length--;
		}

		return subString( start, length );
	}

	TIKI_FORCE_INLINE uint DynamicString::countSubstring( const DynamicString& str ) const
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

	TIKI_FORCE_INLINE DynamicString DynamicString::insert( const DynamicString& str, uint index ) const
	{
		DynamicString oStr = DynamicString( m_stringSize + str.m_stringSize );

		memory::copy( oStr.m_pData, m_pData, sizeof( char ) * index );
		memory::copy( oStr.m_pData + index, str.m_pData, sizeof( char ) * str.m_stringSize );
		memory::copy( oStr.m_pData + index + str.m_stringSize, m_pData + index, sizeof( char ) * (m_stringSize - index) );
		oStr.m_pData[ oStr.m_stringSize ] = '\0';

		return oStr;
	}

	TIKI_FORCE_INLINE DynamicString DynamicString::remove( uint startIndex, uint length ) const
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

		for( uint i = 0u; i < m_stringSize; ++i )
		{
			if( !character::isCapital( str[ i ] ) )
			{
				continue;
			}

			str[ i ] = character::toLowerCase( str[ i ] );
		}

		return str;
	}

	TIKI_FORCE_INLINE DynamicString DynamicString::toUpper() const
	{
		DynamicString str = *this;

		for( uint i = 0u; i < m_stringSize; ++i )
		{
			if( !character::isMinuscule( str[ i ] ) )
			{
				continue;
			}

			str[ i ] = character::toUpperCase( str[ i ] );
		}

		return str;
	}

	TIKI_FORCE_INLINE int DynamicString::indexOf( char c, uint index /*= 0u*/ ) const
	{
		TIKI_ASSERT( index < m_stringSize );

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

	TIKI_FORCE_INLINE int DynamicString::indexOf( const DynamicString& str, uint index /*= 0u*/ ) const
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

	TIKI_FORCE_INLINE int DynamicString::lastIndexOf( char c, uint index /*= 0xffffffffu*/ ) const
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

	TIKI_FORCE_INLINE int DynamicString::lastIndexOf( const DynamicString& str, uint index /*= 0xffffffffu*/ ) const
	{
		index = TIKI_MIN( index, m_stringSize - 1u );
		while( index < m_stringSize )
		{
			uint b = 0;
			bool found = true;
			while( b < str.m_stringSize )
			{
				if( m_pData[ index + b ] != str.m_pData[ b ] )
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

	TIKI_FORCE_INLINE bool DynamicString::contains( const DynamicString& str ) const
	{
		return indexOf( str ) != -1;
	}

	TIKI_FORCE_INLINE bool DynamicString::startsWith( char c ) const
	{
		if( m_stringSize < 1 ) return false;

		return m_pData[ 0 ] == c;
	}

	TIKI_FORCE_INLINE bool DynamicString::startsWith( const DynamicString& str ) const
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

	TIKI_FORCE_INLINE bool DynamicString::endsWith( char c ) const
	{
		if( m_stringSize < 1 ) return false;

		return m_pData[ m_stringSize - 1 ] == c;
	}

	TIKI_FORCE_INLINE bool DynamicString::endsWith( const DynamicString& str ) const
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

	TIKI_FORCE_INLINE const char* DynamicString::operator*() const
	{
		return m_pData;
	}

	TIKI_FORCE_INLINE char DynamicString::operator[]( uint index ) const
	{
		TIKI_ASSERT( index < m_stringSize );
		return m_pData[ index ];
	}

	TIKI_FORCE_INLINE char& DynamicString::operator[]( uint index )
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
		uint length = m_stringSize + rhs.m_stringSize;
		DynamicString str = DynamicString( length );

		memory::copy( str.m_pData, m_pData, m_stringSize );
		memory::copy( str.m_pData + m_stringSize, rhs.m_pData, rhs.m_stringSize );
		str.m_pData[ length ] = '\0';

		return str;
	}

	TIKI_FORCE_INLINE DynamicString& DynamicString::operator+=( const DynamicString& rhs )
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
		uint i = 0;
		uint c = (m_stringSize < rhs.m_stringSize ? m_stringSize : rhs.m_stringSize);
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
		uint i = 0;
		uint c = (m_stringSize < rhs.m_stringSize ? m_stringSize : rhs.m_stringSize);
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

	TIKI_FORCE_INLINE void DynamicString::freeData()
	{
		TIKI_FREE( m_pData );
		m_pData			= nullptr;
		m_dataSize		= 0u;
		m_stringSize	= 0u;
	}

	TIKI_FORCE_INLINE uint DynamicString::calculateLength( uint neededLength ) const
	{
		const uint nextSize = getNextPowerOfTwo( neededLength + 1u );
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

	TIKI_FORCE_INLINE string operator+( const char* str1, const string& str2 )
	{
		return string( str1 ) + str2;
	}
}

#endif // TIKI_BASICSTRING_INL_INCLUDED__
