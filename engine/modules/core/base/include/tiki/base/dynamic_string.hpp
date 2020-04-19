#pragma once

#include "tiki/base/ascii.hpp"
#include "tiki/base/types.hpp"
#include "tiki/container/array.hpp"

namespace tiki
{
	class DynamicString
	{
	public:

		TIKI_FORCE_INLINE					DynamicString();
		TIKI_FORCE_INLINE explicit			DynamicString( uintreg length );
		TIKI_FORCE_INLINE					DynamicString( const char* pString );
		TIKI_FORCE_INLINE explicit			DynamicString( const char* pString, sint length );
		TIKI_FORCE_INLINE					DynamicString( const DynamicString& copy );
		TIKI_FORCE_INLINE					~DynamicString();

		TIKI_FORCE_INLINE bool				isEmpty() const { return m_stringSize == 0; }
		TIKI_FORCE_INLINE void				clear();

		TIKI_FORCE_INLINE uintreg			getLength() const { return m_stringSize; }
		TIKI_FORCE_INLINE uintreg			getCapacity() const { return m_dataSize; }
		TIKI_FORCE_INLINE char*				getBuffer() { return m_pData; }
		TIKI_FORCE_INLINE const char*		cStr() const { return m_pData; }

		TIKI_FORCE_INLINE void				split( Array< DynamicString >& output, const DynamicString& seperator ) const;
		TIKI_FORCE_INLINE DynamicString		replace( char oldValue, char newValue ) const;
		TIKI_FORCE_INLINE DynamicString		replace( const DynamicString& oldValue, const DynamicString& newValue ) const;
		TIKI_FORCE_INLINE DynamicString		subString( uintreg startIndex, sint length = -1 ) const;
		TIKI_FORCE_INLINE DynamicString		trim() const;

		TIKI_FORCE_INLINE DynamicString		insert( const DynamicString& str, uintreg index ) const;
		TIKI_FORCE_INLINE DynamicString		remove( uintreg startIndex, uintreg length ) const;

		TIKI_FORCE_INLINE DynamicString		toLower() const;
		TIKI_FORCE_INLINE DynamicString		toUpper() const;

		TIKI_FORCE_INLINE uintreg			countSubstring( const DynamicString& str, AsciiCompareType matchType = AsciiCompareType::CaseSensitive ) const;
		TIKI_FORCE_INLINE int				indexOf( char c, uintreg index = 0u ) const;
		TIKI_FORCE_INLINE int				indexOf( const DynamicString& str, uintreg index = 0u, AsciiCompareType matchType = AsciiCompareType::CaseSensitive ) const;
		TIKI_FORCE_INLINE int				lastIndexOf( char c, uintreg index = 0xffffffffu ) const;
		TIKI_FORCE_INLINE int				lastIndexOf( const DynamicString& str, uintreg index = 0xffffffffu, AsciiCompareType matchType = AsciiCompareType::CaseSensitive ) const;

		TIKI_FORCE_INLINE bool				contains( char c ) const;
		TIKI_FORCE_INLINE bool				contains( const DynamicString& str, AsciiCompareType matchType = AsciiCompareType::CaseSensitive ) const;

		TIKI_FORCE_INLINE bool				startsWith( char c ) const;
		TIKI_FORCE_INLINE bool				startsWith( const DynamicString& str, AsciiCompareType matchType = AsciiCompareType::CaseSensitive ) const;
		TIKI_FORCE_INLINE bool				endsWith( char c ) const;
		TIKI_FORCE_INLINE bool				endsWith( const DynamicString& str, AsciiCompareType matchType = AsciiCompareType::CaseSensitive ) const;

		TIKI_FORCE_INLINE char*				getBegin() { return m_pData; }
		TIKI_FORCE_INLINE const char*		getBegin() const { return m_pData; }

		TIKI_FORCE_INLINE char*				getEnd() { return m_pData + m_stringSize; }
		TIKI_FORCE_INLINE const char*		getEnd() const { return m_pData + m_stringSize; }

		TIKI_FORCE_INLINE char&				getFirst() { return m_pData[ 0u ]; }
		TIKI_FORCE_INLINE const char&		getFirst() const { return m_pData[ 0u ]; }

		TIKI_FORCE_INLINE char&				getLast() { return m_pData[ m_stringSize - 1u ]; }
		TIKI_FORCE_INLINE const char&		getLast() const { return m_pData[ m_stringSize - 1u ]; }

		TIKI_FORCE_INLINE const char*		operator*() const;
		TIKI_FORCE_INLINE char				operator[]( uintreg index ) const;
		TIKI_FORCE_INLINE char&				operator[]( uintreg index );

		TIKI_FORCE_INLINE bool				operator==( const DynamicString& rhs ) const;
		TIKI_FORCE_INLINE bool				operator!=( const DynamicString& rhs ) const;
		TIKI_FORCE_INLINE DynamicString&	operator=( const DynamicString& rhs );

		TIKI_FORCE_INLINE DynamicString		operator+( const DynamicString& rhs ) const;
		TIKI_FORCE_INLINE DynamicString&	operator+=( const DynamicString& rhs );
		TIKI_FORCE_INLINE DynamicString&	operator+=( char c );

		TIKI_FORCE_INLINE bool				operator>( const DynamicString& rhs ) const;
		TIKI_FORCE_INLINE bool				operator>=( const DynamicString& rhs ) const;
		TIKI_FORCE_INLINE bool				operator<( const DynamicString& rhs ) const;
		TIKI_FORCE_INLINE bool				operator<=( const DynamicString& rhs ) const;

		TIKI_FORCE_INLINE char*				begin() { return getBegin(); }
		TIKI_FORCE_INLINE const char*		begin() const { return getBegin(); }

		TIKI_FORCE_INLINE char*				end() { return getEnd(); }
		TIKI_FORCE_INLINE const char*		end() const { return getEnd(); }

	private:

		char*								m_pData;
		uintreg								m_dataSize;
		uintreg								m_stringSize;
		//uintreg								m_stringLength; // TODO: for utf8

		TIKI_FORCE_INLINE void				allocateData( sint length );
		TIKI_FORCE_INLINE void				reallocateData( sint length );
		TIKI_FORCE_INLINE void				allocateDataForString( const char* pString, sint length = -1 );
		TIKI_FORCE_INLINE void				freeData();

		TIKI_FORCE_INLINE uintreg			calculateLength( uintreg neededLength ) const;

	};

	TIKI_FORCE_INLINE bool					operator==( const char* str1, const DynamicString& str2 );
	TIKI_FORCE_INLINE bool					operator==( const DynamicString& str1, const char* str2 );
	TIKI_FORCE_INLINE DynamicString			operator+( const char* str1, const DynamicString& str2 );

	DynamicString							formatDynamicString( const char* pFormat, ... );
	DynamicString							formatDynamicStringArgs( const char* pFormat, va_list argptr );

	DynamicString							operator ""_s( const char* pString, uintreg length );
}

#include "../../../source/dynamic_string.inl"
