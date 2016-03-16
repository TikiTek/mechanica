#pragma once
#ifndef TIKI_BASICSTRING_HPP_INCLUDED__
#define TIKI_BASICSTRING_HPP_INCLUDED__

#include "tiki/container/array.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class BasicString
	{
		friend class ParseString;
		friend class StringConvert;

	public:

		TIKI_FORCE_INLINE					BasicString();
		TIKI_FORCE_INLINE explicit			BasicString( uint len );
		TIKI_FORCE_INLINE					BasicString( const char* pString );
		TIKI_FORCE_INLINE					BasicString( const char* pString, sint length );
		TIKI_FORCE_INLINE					BasicString( const BasicString& copy );
		TIKI_FORCE_INLINE					~BasicString();

		TIKI_FORCE_INLINE uint				getLength() const;
		TIKI_FORCE_INLINE bool				isEmpty() const;

		TIKI_FORCE_INLINE const char*		cStr() const;

		TIKI_FORCE_INLINE void				split( Array< BasicString >& output, const BasicString& seperator ) const;
		TIKI_FORCE_INLINE BasicString		replace( char oldValue, char newValue ) const;
		TIKI_FORCE_INLINE BasicString		replace( const BasicString& oldValue, const BasicString& newValue ) const;
		TIKI_FORCE_INLINE BasicString		subString( uint startIndex, sint length = -1 ) const;
		TIKI_FORCE_INLINE BasicString		trim() const;

		TIKI_FORCE_INLINE BasicString		insert( const BasicString& str, uint index ) const;
		TIKI_FORCE_INLINE BasicString		remove( uint startIndex, uint len ) const;

		TIKI_FORCE_INLINE BasicString		toLower() const;
		TIKI_FORCE_INLINE BasicString		toUpper() const;

		TIKI_FORCE_INLINE uint				countSubstring( const BasicString& str ) const;
		TIKI_FORCE_INLINE int				indexOf( char c ) const;
		TIKI_FORCE_INLINE int				indexOf( char c, uint index ) const;
		TIKI_FORCE_INLINE int				indexOf( const BasicString& str ) const;
		TIKI_FORCE_INLINE int				indexOf( const BasicString& str, uint index ) const;
		TIKI_FORCE_INLINE int				lastIndexOf( char c ) const;
		TIKI_FORCE_INLINE int				lastIndexOf( char c, uint index ) const;
		TIKI_FORCE_INLINE int				lastIndexOf( const BasicString& str ) const;
		TIKI_FORCE_INLINE int				lastIndexOf( const BasicString& str, uint index ) const;

		TIKI_FORCE_INLINE bool				contains(char c) const;
		TIKI_FORCE_INLINE bool				contains(const BasicString& str) const;

		TIKI_FORCE_INLINE bool				startsWith(char c) const;
		TIKI_FORCE_INLINE bool				startsWith(const BasicString& str) const;
		TIKI_FORCE_INLINE bool				endsWith(char c) const;
		TIKI_FORCE_INLINE bool				endsWith(const BasicString& str) const;

		TIKI_FORCE_INLINE const char*		operator*() const;
		TIKI_FORCE_INLINE char				operator[](uint index) const;
		TIKI_FORCE_INLINE char&				operator[](uint index);

		TIKI_FORCE_INLINE bool				operator==(const BasicString& rhs) const;
		TIKI_FORCE_INLINE bool				operator!=(const BasicString& rhs) const;
		TIKI_FORCE_INLINE BasicString&		operator=(const BasicString& rhs);

		TIKI_FORCE_INLINE BasicString		operator+(const BasicString& rhs) const;
		TIKI_FORCE_INLINE BasicString&		operator+=(const BasicString& rhs);

		TIKI_FORCE_INLINE bool				operator>(const BasicString& rhs) const;
		TIKI_FORCE_INLINE bool				operator>=(const BasicString& rhs) const;
		TIKI_FORCE_INLINE bool				operator<(const BasicString& rhs) const;
		TIKI_FORCE_INLINE bool				operator<=(const BasicString& rhs) const;

	private:

		struct StringRefData
		{
			char*					pData;
			uint					dataLength;
			uint					stringLength;

			sint	    			refCount;

			static const char*		s_pEmptyString;

			TIKI_FORCE_INLINE		StringRefData();
			TIKI_FORCE_INLINE		StringRefData( uint strLen, uint dataLen );
			TIKI_FORCE_INLINE		StringRefData( uint strLen, uint dataLen, const char* pBaseData, sint baseDataLen = -1 );
			TIKI_FORCE_INLINE		~StringRefData();

			TIKI_FORCE_INLINE sint	addRef();
			TIKI_FORCE_INLINE sint	releaseRef();
		};

		StringRefData*				data;

		static const char			letterBigA;
		static const char			letterBigZ;
		static const char			letterLittleA;
		static const char			letterLittleZ;
		static const char			numberDot;
		static const char			numberZero;
		static const char			numberNine;
		static const char			numberPlus;
		static const char			numberMinus;
		static const char			whiteSpaces[ 4u ];
		static StringRefData		emptyData;

		TIKI_FORCE_INLINE void		allocData( const char* pString, sint length );
		TIKI_FORCE_INLINE uint		calcLength( uint neededLen ) const;

	};

	typedef BasicString string;

	TIKI_FORCE_INLINE string		operator+( const char* str1, const string& str2 );

	string							formatString( const char* pFormat, ... );
	string							formatStringArgs( const char* pFormat, va_list argptr );

}

#include "../../../source/basicstring.inl"

#endif // TIKI_BASICSTRING_HPP_INCLUDED__
