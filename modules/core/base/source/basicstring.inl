#pragma once
#ifndef TIKI_BASICSTRING_INL_INCLUDED__
#define TIKI_BASICSTRING_INL_INCLUDED__

#include "tiki/base/memory.hpp"
#include "tiki/base/sizedarray.hpp"
#include "tiki/base/string.hpp"

namespace tiki
{
	BasicString::StringRefData::StringRefData()
		: pData( (char*)s_pEmptyString )
		, dataLength( 0u )
		, stringLength( 0u )
		, refCount( 1 )
	{
	}

	BasicString::StringRefData::StringRefData( uint strLen, uint dataLen )
		: refCount( 1 )
	{
		pData			= TIKI_MEMORY_NEW_ARRAY( char, dataLen );
		dataLength		= dataLen;
		stringLength	= strLen;

		pData[ strLen ] = 0;
	}

	BasicString::StringRefData::StringRefData( uint strLen, uint dataLen, const char* pBaseData, sint baseDataLen /*= -1*/ )
		: refCount( 1 )
	{
		pData			= TIKI_MEMORY_NEW_ARRAY( char, dataLen );
		dataLength		= dataLen;
		stringLength	= strLen;

		memory::copy( pData, pBaseData, sizeof(char) * (baseDataLen == -1 ? strLen : baseDataLen) );
		pData[ strLen ] = 0;
	}

	BasicString::StringRefData::~StringRefData()
	{
		if ( pData != nullptr && pData != s_pEmptyString )
		{
			TIKI_MEMORY_DELETE_ARRAY( pData, dataLength );
			pData = nullptr;
		}
	}

	TIKI_FORCE_INLINE sint BasicString::StringRefData::addRef()
	{
		return ++refCount;
	}

	TIKI_FORCE_INLINE sint BasicString::StringRefData::releaseRef()
	{
		if ( this == &BasicString::emptyData )
		{
			return 0;
		}

		if (--refCount < 1)
		{
			TIKI_MEMORY_DELETE_OBJECT( this );
			return 0;
		}

		return refCount;
	}

	TIKI_FORCE_INLINE BasicString::BasicString()
	{
		data = &emptyData;
	}

	TIKI_FORCE_INLINE BasicString::BasicString( uint len )
	{
		data = TIKI_MEMORY_NEW_OBJECT( StringRefData )(
			len,
			calcLength( len + 1 )
		);
	}

	TIKI_FORCE_INLINE BasicString::BasicString( const char* pString )
	{
		allocData( pString, -1 );
	}

	TIKI_FORCE_INLINE BasicString::BasicString( const char* pString, sint length )
	{
		allocData( pString, length );
	}

	TIKI_FORCE_INLINE BasicString::BasicString( const BasicString& copy )
		: data( copy.data )
	{
		data->addRef();
	}

	TIKI_FORCE_INLINE BasicString::~BasicString()
	{
		if ( data )
		{
			data->releaseRef();
			data = nullptr;
		}
	}

	TIKI_FORCE_INLINE void BasicString::allocData( const char* pString, sint length = -1 )
	{
		if ( pString == nullptr )
		{
			data = &emptyData;
		}
		else
		{
			uint len = (length == -1 ? getStringSize( pString ) : length);

			if ( len == 0 )
			{
				data = &emptyData;
				return;
			}

			data = TIKI_MEMORY_NEW_OBJECT( StringRefData )(
				len,
				calcLength( len + 1 ),
				pString
			);
		}
	}

	TIKI_FORCE_INLINE uint BasicString::getLength() const
	{
		return data->stringLength;
	}

	TIKI_FORCE_INLINE bool BasicString::isEmpty() const
	{
		return data->stringLength == 0;
	}

	TIKI_FORCE_INLINE const char* BasicString::cStr() const
	{
		return data->pData;
	}

	TIKI_FORCE_INLINE void BasicString::split( Array< BasicString >& output, const BasicString& seperator ) const
	{
		const uint count = countSubstring( seperator );

		SizedArray< BasicString > list;
		list.create( count + 1u );

		uint i = 0;
		uint lastIndex = 0;
		while (i < count)
		{
			const uint index = indexOf(seperator, lastIndex + seperator.data->stringLength);

			list.push(
				subString( lastIndex, sint( index - lastIndex ) )
			);

			lastIndex = index + seperator.data->stringLength;
			i++;
		}

		if (data->stringLength - lastIndex > 0)
		{
			list.push(
				subString( lastIndex, sint( data->stringLength - lastIndex ) )
			);
		}

		output.create( list.getBegin(), list.getCount() );
		list.dispose();
	}

	TIKI_FORCE_INLINE BasicString BasicString::replace(char oldValue, char newValue) const
	{
		BasicString str = *this;

		uint i = 0;
		while (i < data->stringLength)
		{
			if (str[i] == oldValue) str[i] = newValue;
			i++;
		}

		return str;
	}

	TIKI_FORCE_INLINE BasicString BasicString::replace(const BasicString& oldValue, const BasicString& newValue) const
	{
		const uint count = countSubstring(oldValue);
		const uint length = data->stringLength - (count * oldValue.data->stringLength) + (count * newValue.data->stringLength);

		if ( count == 0 )
		{
			return *this;
		}

		BasicString str = BasicString(length);

		uint i = 0;
		uint offsetOld = 0;
		uint offsetNew = 0;
		while (i < count)
		{
			const uint index		= indexOf(oldValue, offsetOld);
			const uint oldDifferent	= index - offsetOld;

			memory::copy(str.data->pData + offsetNew, data->pData + offsetOld, sizeof(char) * (index - offsetOld));
			offsetOld += oldDifferent;
			offsetNew += oldDifferent;

			memory::copy(str.data->pData + offsetNew, newValue.data->pData, sizeof(char) * newValue.data->stringLength);
			offsetOld += oldValue.data->stringLength;
			offsetNew += newValue.data->stringLength;

			i++;
		}

		memory::copy( str.data->pData + offsetNew, data->pData + offsetOld, sizeof(char) * (data->stringLength - offsetOld));

		return str;
	}

	TIKI_FORCE_INLINE BasicString BasicString::subString(uint startIndex, sint length /*= -1*/ ) const
	{
		if (length == -1 || startIndex + length > data->stringLength)
		{
			length = data->stringLength - startIndex;
		}
		TIKI_ASSERT ( startIndex < data->stringLength || length == 0 );

		if ( length == 0 )
		{
			return BasicString();
		}

		if ( length == data->stringLength && startIndex == 0u )
		{
			return BasicString( *this );
		}

		return BasicString(
			data->pData + startIndex,
			length
		);
	}

	TIKI_FORCE_INLINE BasicString BasicString::trim() const
	{
		uint start = 0u;
		uint length = data->stringLength;

		if ( length == 0u )
		{
			return *this;
		}

		bool isWhiteSpace = false;
		do
		{
			isWhiteSpace = false;

			for (uint i = 0u; i < TIKI_COUNT( whiteSpaces ); ++i)
			{
				if ( data->pData[ start ] == whiteSpaces[ i ] )
				{
					isWhiteSpace = true;
					start++;
					length--;
					break;
				}
			}
		}
		while ( isWhiteSpace );

		do
		{
			isWhiteSpace = false;

			for (uint i = 0u; i < TIKI_COUNT( whiteSpaces ); ++i)
			{
				if ( data->pData[ start + length - 1u ] == whiteSpaces[ i ] )
				{
					isWhiteSpace = true;
					length--;
					break;
				}
			}
		}
		while ( isWhiteSpace );

		return subString( start, length );
	}

	TIKI_FORCE_INLINE uint BasicString::countSubstring(const BasicString& str) const
	{
		if (str.data->stringLength > data->stringLength)
			return 0u;

		uint i = 0;
		uint c = 0;
		while (i < data->stringLength)
		{
			uint b = 0;
			bool found = true;
			while (b < str.data->stringLength)
			{
				if (data->pData[i + b] != str.data->pData[b])
				{
					found = false;
					break;
				}
				b++;
			}

			if (found)
			{
				c++;
				i += str.data->stringLength;
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
		BasicString oStr = BasicString(data->stringLength + str.data->stringLength);

		memory::copy(oStr.data->pData, data->pData, sizeof(char) * index);
		memory::copy(oStr.data->pData + index, str.data->pData, sizeof(char) * str.data->stringLength);
		memory::copy(oStr.data->pData + index + str.data->stringLength, data->pData + index, sizeof(char) * (data->stringLength - index));

		return oStr;
	}

	TIKI_FORCE_INLINE BasicString BasicString::remove( uint startIndex, uint len ) const
	{
		BasicString oStr = BasicString(data->stringLength - len);

		memory::copy(oStr.data->pData, data->pData, sizeof(char) * startIndex);
		memory::copy(oStr.data->pData + startIndex, data->pData + startIndex + len, sizeof(char) * (data->stringLength - startIndex - len));

		return oStr;
	}

	TIKI_FORCE_INLINE BasicString BasicString::toLower() const
	{
		BasicString str = *this;

		uint i = 0;
		while (i < data->stringLength)
		{
			if (str[i] >= letterBigA && str[i] <= letterBigZ)
				str[i] -= letterBigZ - letterLittleZ;

			i++;
		}

		return str;
	}

	TIKI_FORCE_INLINE BasicString BasicString::toUpper() const
	{
		BasicString str = *this;

		uint i = 0;
		while (i < data->stringLength)
		{
			if (str[i] >= letterLittleA && str[i] <= letterLittleZ)
				str[i] -= letterLittleZ - letterBigZ;

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
		while ( i < data->stringLength )
		{
			if ( data->pData[ i ] == c )
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
		if (str.data->stringLength > data->stringLength) return -1;

		uint i = index;
		uint c = data->stringLength - str.data->stringLength;

		do
		{
			uint b = 0;
			bool found = true;
			while (b < str.data->stringLength)
			{
				if (data->pData[ i + b ] != str.data->pData[ b ])
				{
					found = false;
					break;
				}
				b++;
			}

			if ( found )
			{
				return int( i );
			}

			i++;
		}
		while (i <= c);

		return -1;
	}

	TIKI_FORCE_INLINE int BasicString::lastIndexOf( char c ) const
	{
		return lastIndexOf( c, data->stringLength - 1u );
	}

	TIKI_FORCE_INLINE int BasicString::lastIndexOf( char c, uint index ) const
	{
		int i = int( index );
		while (i >= 0)
		{
			if ( data->pData[i] == c ) return i;
			i--;
		}

		return -1;
	}

	TIKI_FORCE_INLINE int BasicString::lastIndexOf( const BasicString& str ) const
	{
		return lastIndexOf( str, data->stringLength - str.data->stringLength );
	}

	TIKI_FORCE_INLINE int BasicString::lastIndexOf( const BasicString& str, uint index ) const
	{
		int i = (int)index;
		while (i >= 0)
		{
			int b = 0;
			bool found = true;
			while (b < str.data->stringLength)
			{
				if (data->pData[i + b] != str.data->pData[b])
				{
					found = false;
					break;
				}
				b++;
			}

			if (found)
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
		if (data->stringLength < 1) return false;

		return data->pData[0] == c;
	}

	TIKI_FORCE_INLINE bool BasicString::startsWith(const BasicString& str) const
	{
		if (data->stringLength < str.data->stringLength) return false;

		uint i = 0;
		while (i < str.data->stringLength)
		{
			if (data->pData[i] != str.data->pData[i]) return false;
			i++;
		}

		return true;
	}

	TIKI_FORCE_INLINE bool BasicString::endsWith(char c) const
	{
		if (data->stringLength < 1) return false;

		return data->pData[data->stringLength - 1] == c;
	}

	TIKI_FORCE_INLINE bool BasicString::endsWith(const BasicString& str) const
	{
		if (data->stringLength < str.data->stringLength) return false;

		uint b = 0;
		uint i = data->stringLength - str.data->stringLength;
		while (i < data->stringLength)
		{
			if (data->pData[i] != str.data->pData[b]) return false;
			i++;
			b++;
		}

		return true;
	}

	TIKI_FORCE_INLINE const char* BasicString::operator*() const
	{
		return data->pData;
	}

	TIKI_FORCE_INLINE char BasicString::operator[](uint index) const
	{
		if (index >= data->stringLength)
			throw "Index > Length";

		return data->pData[index];
	}

	TIKI_FORCE_INLINE char& BasicString::operator[](uint index)
	{
		if (index >= data->stringLength)
			throw "Index > Length";

		if (data->refCount > 1)
		{
			StringRefData* oldData = data;

			data = TIKI_MEMORY_NEW_OBJECT( StringRefData )(
				data->stringLength,
				data->dataLength,
				data->pData
			);

			oldData->releaseRef();
		}

		return data->pData[index];
	}

	TIKI_FORCE_INLINE bool BasicString::operator==( const BasicString& rhs ) const
	{
		if ( data == rhs.data )
		{
			return true;
		}

		if (data->stringLength != rhs.data->stringLength)
		{
			return false;
		}

		uint i = 0;
		while ( i < data->stringLength )
		{
			if ( data->pData[ i ] != rhs.data->pData[ i ] )
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

	TIKI_FORCE_INLINE BasicString& BasicString::operator=(const BasicString& rhs)
	{
		StringRefData* oldData = data;

		data = rhs.data;
		data->addRef();

		oldData->releaseRef();

		return *this;
	}

	TIKI_FORCE_INLINE BasicString BasicString::operator+(const BasicString& rhs) const
	{
		uint len = data->stringLength + rhs.data->stringLength;
		BasicString str = BasicString(len);

		memory::copy(str.data->pData, data->pData, sizeof(char) * data->stringLength);
		memory::copy(str.data->pData + data->stringLength, rhs.data->pData, sizeof(char) * rhs.data->stringLength);
		str.data->pData[len] = 0;

		return str;
	}

	TIKI_FORCE_INLINE BasicString& BasicString::operator+=(const BasicString& rhs)
	{
		uint sl = data->stringLength;
		uint len = data->stringLength + rhs.data->stringLength;

		if (data->refCount != 1 || data->dataLength <= len)
		{
			StringRefData* oldData = data;

			data = TIKI_MEMORY_NEW_OBJECT( StringRefData )(
				len,
				calcLength(len + 1),
				oldData->pData,
				oldData->stringLength
			);

			oldData->releaseRef();
		}

		memory::copy(data->pData + sl, rhs.data->pData, sizeof(char) * rhs.data->stringLength);
		data->pData[len] = 0;
		data->stringLength = len;

		return *this;
	}

	TIKI_FORCE_INLINE bool BasicString::operator>(const BasicString& rhs) const
	{
		if (data == rhs.data) return false;

		uint i = 0;
		uint c = (data->stringLength < rhs.data->stringLength ? data->stringLength : rhs.data->stringLength);
		while (i < c && data->pData[i] == rhs.data->pData[i])
		{
			i++;
		}

		return data->pData[i] > rhs.data->pData[i];
	}

	TIKI_FORCE_INLINE bool BasicString::operator>=(const BasicString& rhs) const
	{
		return (*this > rhs) || (*this == rhs);
	}

	TIKI_FORCE_INLINE bool BasicString::operator<(const BasicString& rhs) const
	{
		if (data == rhs.data) return false;

		uint i = 0;
		uint c = (data->stringLength < rhs.data->stringLength ? data->stringLength : rhs.data->stringLength);
		while (i < c && data->pData[i] == rhs.data->pData[i])
		{
			i++;
		}

		return data->pData[i] < rhs.data->pData[i];
	}

	TIKI_FORCE_INLINE bool BasicString::operator<=(const BasicString& rhs) const
	{
		return (*this < rhs) || (*this == rhs);
	}

	TIKI_FORCE_INLINE uint BasicString::calcLength( uint neededLen ) const
	{
		uint len = 2;
		while (len < neededLen) { len *= 2; }
		return len;
	}

	TIKI_FORCE_INLINE string operator+( const char* str1, const string& str2 )
	{
		return string(str1) + str2;
	}

}

#endif // TIKI_BASICSTRING_INL_INCLUDED__
