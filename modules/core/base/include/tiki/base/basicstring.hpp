#pragma once
#ifndef TIKI_BASICSTRING_HPP_INCLUDED__
#define TIKI_BASICSTRING_HPP_INCLUDED__

#include "tiki/container/array.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	class BasicString
	{
	public:

		TIKI_FORCE_INLINE					BasicString();
		TIKI_FORCE_INLINE explicit			BasicString( uint length );
		TIKI_FORCE_INLINE					BasicString( const char* pString );
		TIKI_FORCE_INLINE					BasicString( const char* pString, sint length );
		TIKI_FORCE_INLINE					BasicString( const BasicString& copy );
		TIKI_FORCE_INLINE					~BasicString();

		TIKI_FORCE_INLINE uint				getLength() const;
		TIKI_FORCE_INLINE bool				isEmpty() const;

		TIKI_FORCE_INLINE char*				getBuffer();
		TIKI_FORCE_INLINE const char*		cStr() const;

		TIKI_FORCE_INLINE void				split( Array< BasicString >& output, const BasicString& seperator ) const;
		TIKI_FORCE_INLINE BasicString		replace( char oldValue, char newValue ) const;
		TIKI_FORCE_INLINE BasicString		replace( const BasicString& oldValue, const BasicString& newValue ) const;
		TIKI_FORCE_INLINE BasicString		subString( uint startIndex, sint length = -1 ) const;
		TIKI_FORCE_INLINE BasicString		trim() const;

		TIKI_FORCE_INLINE BasicString		insert( const BasicString& str, uint index ) const;
		TIKI_FORCE_INLINE BasicString		remove( uint startIndex, uint length ) const;

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

		TIKI_FORCE_INLINE char*				getBegin() { return m_pData; }
		TIKI_FORCE_INLINE const char*		getBegin() const { return m_pData; }

		TIKI_FORCE_INLINE char*				getEnd() { return m_pData + m_stringSize; }
		TIKI_FORCE_INLINE const char*		getEnd() const { return m_pData + m_stringSize; }

		TIKI_FORCE_INLINE char&				getFirst() { return m_pData[ 0u ]; }
		TIKI_FORCE_INLINE const char&		getFirst() const { return m_pData[ 0u ]; }

		TIKI_FORCE_INLINE char&				getLast() { return m_pData[ m_stringSize - 1u ]; }
		TIKI_FORCE_INLINE const char&		getLast() const { return m_pData[ m_stringSize - 1u ]; }

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

		TIKI_FORCE_INLINE char*				begin() { return getBegin(); }
		TIKI_FORCE_INLINE const char*		begin() const { return getBegin(); }

		TIKI_FORCE_INLINE char*				end() { return getEnd(); }
		TIKI_FORCE_INLINE const char*		end() const { return getEnd(); }

	private:

		char*						m_pData;
		uint						m_dataSize;
		uint						m_stringSize;
		//uint						m_stringLength; // TODO

		static const char			whiteSpaces[ 4u ];

		TIKI_FORCE_INLINE void		allocateData( sint length );
		TIKI_FORCE_INLINE void		reallocateData( sint length );
		TIKI_FORCE_INLINE void		allocateDataForString( const char* pString, sint length = -1 );
		TIKI_FORCE_INLINE void		freeData();

		TIKI_FORCE_INLINE uint		calculateLength( uint neededLength ) const;

	};

	typedef BasicString string;

	TIKI_FORCE_INLINE string		operator+( const char* str1, const string& str2 );

	string							formatString( const char* pFormat, ... );
	string							formatStringArgs( const char* pFormat, va_list argptr );

}

#include "../../../source/basicstring.inl"

#endif // TIKI_BASICSTRING_HPP_INCLUDED__
