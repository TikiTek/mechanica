#pragma once
#ifndef TIKI_STRING_INL
#define TIKI_STRING_INL

#include "tiki/base/memory.hpp"
#include "tiki/base/sizedarray.hpp"

namespace tiki
{
	template<typename TChar>
	struct StringRefData
	{
		TChar* pData;
		size_t dataLength;
		size_t stringLength;

		sint32 refCount;

		StringRefData()
			: pData( nullptr ), dataLength( 0u ), stringLength( 0u ), refCount( 1 )
		{
		}

		StringRefData( uint32 strLen, uint32 dataLen )
			: refCount( 1 )
		{
			pData			= TIKI_NEW TChar[ dataLen ];
			dataLength		= dataLen;
			stringLength	= strLen;

			pData[ strLen ] = 0;
		}

		StringRefData( uint32 strLen, uint32 dataLen, const TChar* pBaseData, sint32 baseDataLen = -1 )
			: refCount( 1 )
		{
			pData			= TIKI_NEW TChar[dataLen];
			dataLength		= dataLen;
			stringLength	= strLen;

			memory::copy( pData, pBaseData, sizeof(TChar) * (baseDataLen == -1 ? strLen : baseDataLen) );
			pData[ strLen ] = 0;
		}

		~StringRefData()
		{
			if (pData)
			{
				delete[] pData;
				pData = nullptr;
			}
		}

		TIKI_FORCE_INLINE sint32 addRef()
		{
			return ++refCount;
		}

		TIKI_FORCE_INLINE sint32 release()
		{
			if ( this == &BasicString<TChar>::emptyData ) return 0;

			if (--refCount < 1)
			{
				delete this;
				return 0;
			}

			return refCount;
		}
	};

	template<typename TChar>
	BasicString<TChar>::BasicString()
	{
		data = &emptyData;
	}

	template<typename TChar>
	BasicString<TChar>::BasicString(TChar c)
	{
		data = TIKI_NEW StringRefData<TChar>(1, 2, &c, 1);
	}

	template<typename TChar>
	BasicString<TChar>::BasicString(const TChar* string)
	{
		allocData( string, -1 );
	}

	template<typename TChar>
	BasicString<TChar>::BasicString(const TChar* string, sint32 length)
	{
		allocData( string, length );
	}

	template<typename TChar>
	BasicString<TChar>::BasicString(const BasicString<TChar>& copy)
		: data(copy.data)
	{
		data->addRef();
	}

	template<typename TChar>
	BasicString<TChar>::BasicString( size_t  len )
	{
		data = TIKI_NEW StringRefData<TChar>(
			len,
			calcLength(len + 1)
		);
	}

	template<typename TChar>
	BasicString<TChar>::~BasicString()
	{
		if ( data )
		{
			data->release();
			data = nullptr;
		}
	}

	template<typename TChar>
	TIKI_FORCE_INLINE void tiki::BasicString<TChar>::allocData( const TChar* string, sint32 length = -1 )
	{
		if (string == 0)
		{
			data = &emptyData;
		}
		else
		{
			uint32 len = (length == -1 ? stringLength(string) : length);

			if ( len == 0)
			{
				data = &emptyData;
				return;
			}

			data = TIKI_NEW StringRefData<TChar>(
				len,
				calcLength(len + 1),
				string
			);
		}
	}

	template<typename TChar>
	TIKI_FORCE_INLINE uint32 BasicString<TChar>::length() const
	{
		return data->stringLength;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE bool BasicString<TChar>::isEmpty() const
	{
		return data->stringLength == 0;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE const TChar* BasicString<TChar>::cStr() const
	{
		return data->pData;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE TChar* BasicString<TChar>::createCStr() const
	{
		TChar* str = TIKI_NEW TChar[data->stringLength + 1];
		memory::copy(str, data->pData, sizeof(TChar) * (data->stringLength + 1));

		return str;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE void BasicString<TChar>::split( Array< BasicString< TChar > >& output, const BasicString<TChar>& seperator ) const
	{
		const uint32 count = countSubstring( seperator );

		SizedArray< BasicString< TChar > > list;
		list.create( count + 1u );

		uint32 i = 0;
		uint32 lastIndex = 0;
		while (i < count)
		{
			uint32 index = indexOf(seperator, lastIndex + seperator.data->stringLength);

			list.push(
				substring( lastIndex, index - lastIndex )
			);

			lastIndex = index + seperator.data->stringLength;
			i++;
		}

		if (data->stringLength - lastIndex > 0)
		{
			list.push(
				substring( lastIndex, data->stringLength - lastIndex )
			);
		}

		output.create( list.getData(), list.getCount() );
		list.dispose();
	}

	template<typename TChar>
	TIKI_FORCE_INLINE BasicString<TChar> BasicString<TChar>::replace(TChar oldValue, TChar newValue) const
	{
		BasicString str = *this;

		uint32 i = 0;
		while (i < data->stringLength)
		{
			if (str[i] == oldValue) str[i] = newValue;
			i++;
		}

		return str;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE BasicString<TChar> BasicString<TChar>::replace(const BasicString<TChar>& oldValue, const BasicString<TChar>& newValue) const
	{
		uint32 count = countSubstring(oldValue);
		uint32 length = data->stringLength - (count * oldValue.data->stringLength) + (count * newValue.data->stringLength);

		if ( count == 0 )
		{
			return *this;
		}

		BasicString<TChar> str = BasicString<TChar>(length);

		uint32 i = 0;
		uint32 offsetOld = 0;
		uint32 offsetNew = 0;
		while (i < count)
		{
			uint32 index = indexOf(oldValue, offsetOld);

			memory::copy(str.data->pData + offsetNew, data->pData + offsetOld, sizeof(TChar) * (index - offsetOld));
			offsetOld += index - offsetOld;
			offsetNew += index - offsetNew;

			memory::copy(str.data->pData + offsetNew, newValue.data->pData, sizeof(TChar) * newValue.data->stringLength);
			offsetOld += oldValue.data->stringLength;
			offsetNew += newValue.data->stringLength;

			i++;
		}

		memory::copy( str.data->pData + offsetNew, data->pData + offsetOld, sizeof(TChar) * (str.data->stringLength - offsetNew));

		return str;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE BasicString<TChar> BasicString<TChar>::substring(uint32 startIndex, sint32 length /*= -1*/ ) const
	{
		if (length == -1 || startIndex + length > data->stringLength)
		{
			length = data->stringLength - startIndex;
		}
		TIKI_ASSERT ( startIndex < data->stringLength || length == 0 );

		if ( length == 0 )
		{
			return BasicString< TChar >();
		}

		if ( length == data->stringLength && startIndex == 0u )
		{
			return BasicString<TChar>( *this );
		}

		return BasicString<TChar>(
			data->pData + startIndex,
			length
		);
	}

	template<typename TChar>
	TIKI_FORCE_INLINE BasicString<TChar> BasicString<TChar>::trim() const
	{
		size_t start = 0u;
		size_t length = data->stringLength;

		bool isWhiteSpace = false;
		do 
		{
			isWhiteSpace = false;

			for (size_t i = 0u; i < TIKI_COUNT( whiteSpaces ); ++i)
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

			for (size_t i = 0u; i < TIKI_COUNT( whiteSpaces ); ++i)
			{
				if ( data->pData[ start + length ] == whiteSpaces[ i ] )
				{
					isWhiteSpace = true;
					length--;
					break;
				}
			}
		}
		while ( isWhiteSpace );

		return substring( start, length );
	}

	template<typename TChar>
	TIKI_FORCE_INLINE uint32 BasicString<TChar>::countSubstring(const BasicString<TChar>& str) const
	{
		if (str.data->stringLength > data->stringLength)
			return 0u;

		uint32 i = 0;
		uint32 c = 0;
		while (i < data->stringLength)
		{
			uint32 b = 0;
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

	template<typename TChar>
	TIKI_FORCE_INLINE BasicString<TChar> BasicString<TChar>::insert(const BasicString<TChar>& str, uint32 index) const
	{
		BasicString<TChar> oStr = BasicString<TChar>(data->stringLength + str.data->stringLength);

		memory::copy(oStr.data->pData, data->pData, sizeof(TChar) * index);
		memory::copy(oStr.data->pData + index, str.data->pData, sizeof(TChar) * str.data->stringLength);
		memory::copy(oStr.data->pData + index + str.data->stringLength, data->pData + index, sizeof(TChar) * (data->stringLength - index));

		return oStr;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE BasicString<TChar> BasicString<TChar>::remove(uint32 startIndex, uint32 len) const
	{
		BasicString<TChar> oStr = BasicString<TChar>(data->stringLength - len);

		memory::copy(oStr.data->pData, data->pData, sizeof(TChar) * startIndex);
		memory::copy(oStr.data->pData + startIndex, data->pData + startIndex + len, sizeof(TChar) * (data->stringLength - startIndex - len));

		return oStr;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE BasicString<TChar> BasicString<TChar>::toLower() const
	{
		BasicString<TChar> str = *this;

		uint32 i = 0;
		while (i < data->stringLength)
		{
			if (str[i] >= letterBigA && str[i] <= letterBigZ)
				str[i] -= letterBigZ - letterLittleZ;

			i++;
		}

		return str;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE BasicString<TChar> BasicString<TChar>::toUpper() const
	{
		BasicString<TChar> str = *this;

		uint32 i = 0;
		while (i < data->stringLength)
		{
			if (str[i] >= letterLittleA && str[i] <= letterLittleZ)
				str[i] -= letterLittleZ - letterBigZ;

			i++;
		}

		return str;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE sint32 BasicString<TChar>::indexOf( TChar c ) const
	{
		return indexOf( c, 0 );
	}

	template<typename TChar>
	TIKI_FORCE_INLINE sint32 BasicString<TChar>::indexOf(TChar c, uint32 index) const
	{
		uint32 i = index;
		while (i < data->stringLength)
		{
			if (data->pData[i] == c) return i;
			i++;
		}
		return -1;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE sint32 BasicString<TChar>::indexOf( const BasicString<TChar>& str ) const
	{
		return indexOf( str, 0 );
	}

	template<typename TChar>
	TIKI_FORCE_INLINE sint32 BasicString<TChar>::indexOf(const BasicString<TChar>& str, uint32 index) const
	{
		if (str.data->stringLength > data->stringLength) return -1;

		uint32 i = index;
		uint32 c = data->stringLength - str.data->stringLength;

		do
		{
			uint32 b = 0;
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

			i++;
		}
		while (i <= c);

		return -1;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE sint32 BasicString<TChar>::lastIndexOf( TChar c ) const
	{
		sint32 i = data->stringLength - 1;
		while (i >= 0)
		{
			if ( data->pData[i] == c ) return i;
			i--;
		}

		return -1;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE sint32 BasicString<TChar>::lastIndexOf(const BasicString<TChar>& str) const
	{
		sint32 i = (data->stringLength - str.data->stringLength);
		while (i >= 0)
		{
			uint32 b = 0;
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

	template<typename TChar>
	TIKI_FORCE_INLINE bool BasicString<TChar>::contains( TChar c ) const
	{
		return this->IndexOf(c) != -1;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE bool BasicString<TChar>::contains( const BasicString<TChar>& str ) const
	{
		return this->IndexOf(str) != -1;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE bool BasicString<TChar>::startsWith( TChar c ) const
	{
		if (data->stringLength < 1) return false;

		return data->pData[0] == c;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE bool BasicString<TChar>::startsWith(const BasicString<TChar>& str) const
	{
		if (data->stringLength < str.data->stringLength) return false;

		uint32 i = 0;
		while (i < str.data->stringLength)
		{
			if (data->pData[i] != str.data->pData[i]) return false;
			i++;
		}

		return true;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE bool BasicString<TChar>::endsWith(TChar c) const
	{
		if (data->stringLength < 1) return false;

		return data->pData[data->stringLength - 1] == c;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE bool BasicString<TChar>::endsWith(const BasicString<TChar>& str) const
	{
		if (data->stringLength < str.data->stringLength) return false;

		uint32 b = 0;
		uint32 i = data->stringLength - str.data->stringLength;
		while (i < data->stringLength)
		{
			if (data->pData[i] != str.data->pData[b]) return false;
			i++;
			b++;
		}

		return true;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE const TChar* BasicString<TChar>::operator*() const
	{
		return data->pData;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE TChar BasicString<TChar>::operator[](uint32 index) const
	{
		if (index >= data->stringLength) 
			throw "Index > Length";

		return data->pData[index];
	}

	template<typename TChar>
	TIKI_FORCE_INLINE TChar& BasicString<TChar>::operator[](uint32 index)
	{
		if (index >= data->stringLength)
			throw "Index > Length";

		if (data->refCount > 1)
		{
			StringRefData<TChar>* oldData = data;

			data = TIKI_NEW StringRefData<TChar>(
				data->stringLength,
				data->dataLength,
				data->pData
			);

			oldData->release();
		}

		return data->pData[index];
	}

	template<typename TChar>
	TIKI_FORCE_INLINE bool BasicString<TChar>::operator==(const BasicString<TChar>& rhs) const
	{
		if (data == rhs.data) return true;
		if (data->stringLength != rhs.data->stringLength) return false;

		uint32 i = 0;
		while (i < data->stringLength)
		{
			if (data->pData[i] != rhs.data->pData[i]) return false;
			i++;
		}

		return true;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE bool BasicString<TChar>::operator!=(const BasicString<TChar>& rhs) const
	{
		return !(*this == rhs);
	}

	template<typename TChar>
	TIKI_FORCE_INLINE BasicString<TChar>& BasicString<TChar>::operator=(const BasicString<TChar>& rhs)
	{
		StringRefData<TChar>* oldData = data;

		data = rhs.data;
		data->addRef();

		oldData->release();

		return *this;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE BasicString<TChar> BasicString<TChar>::operator+(const BasicString<TChar>& rhs) const
	{
		uint32 len = data->stringLength + rhs.data->stringLength;
		BasicString<TChar> str = BasicString<TChar>(len);

		memory::copy(str.data->pData, data->pData, sizeof(TChar) * data->stringLength);
		memory::copy(str.data->pData + data->stringLength, rhs.data->pData, sizeof(TChar) * rhs.data->stringLength);
		str.data->pData[len] = 0;

		return str;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE BasicString<TChar>& BasicString<TChar>::operator+=(const BasicString<TChar>& rhs)
	{
		uint32 sl = data->stringLength;
		uint32 len = data->stringLength + rhs.data->stringLength;

		if (data->refCount != 1 || data->dataLength <= len)
		{
			StringRefData<TChar>* oldData = data;

			data = TIKI_NEW StringRefData<TChar>(
				len,
				calcLength(len + 1),
				oldData->pData,
				oldData->stringLength
			);

			oldData->release();
		}

		memory::copy(data->pData + sl, rhs.data->pData, sizeof(TChar) * rhs.data->stringLength);
		data->pData[len] = 0;
		data->stringLength = len;

		return *this;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE bool BasicString<TChar>::operator>(const BasicString<TChar>& rhs) const
	{
		if (data == rhs.data) return false;

		uint32 i = 0;
		uint32 c = (data->stringLength < rhs.data->stringLength ? data->stringLength : rhs.data->stringLength);
		while (i < c && data->pData[i] == rhs.data->pData[i])
		{
			i++;
		}

		return data->pData[i] > rhs.data->pData[i];
	}

	template<typename TChar>
	TIKI_FORCE_INLINE bool BasicString<TChar>::operator>=(const BasicString<TChar>& rhs) const
	{
		return (*this > rhs) || (*this == rhs);
	}

	template<typename TChar>
	TIKI_FORCE_INLINE bool BasicString<TChar>::operator<(const BasicString<TChar>& rhs) const
	{
		if (data == rhs.data) return false;

		uint32 i = 0;
		uint32 c = (data->stringLength < rhs.data->stringLength ? data->stringLength : rhs.data->stringLength);
		while (i < c && data->pData[i] == rhs.data->pData[i])
		{
			i++;
		}

		return data->pData[i] < rhs.data->pData[i];
	}

	template<typename TChar>
	TIKI_FORCE_INLINE bool BasicString<TChar>::operator<=(const BasicString<TChar>& rhs) const
	{
		return (*this < rhs) || (*this == rhs);
	}

	template<typename TChar>
	TIKI_FORCE_INLINE uint32 BasicString<TChar>::stringLength(const TChar* string) const
	{
		if (string == 0) return 0;

		uint32 c = 0;
		const TChar* str = string;
		while (*str != 0) { str++; c++; }

		return c;
	}

	template<typename TChar>
	TIKI_FORCE_INLINE uint32 BasicString<TChar>::calcLength( uint32 neededLen ) const
	{
		uint32 len = 2;
		while (len < neededLen) { len *= 2; }
		return len;
	}

	TIKI_FORCE_INLINE BasicString< char > operator+( const char* str1, const BasicString< char >& str2 )
	{
		return BasicString< char >(str1) + str2;
	}

	TIKI_FORCE_INLINE BasicString< wchar_t > operator+( const wchar_t* str1, const BasicString< wchar_t >& str2 )
	{
		return BasicString< wchar_t >(str1) + str2;
	}
}

#endif // TIKI_STRING_INL