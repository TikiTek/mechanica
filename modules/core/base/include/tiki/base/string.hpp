#pragma once
#ifndef TIKI_STRING_HPP
#define TIKI_STRING_HPP

#include "tiki/base/array.hpp"
#include "tiki/base/inline.hpp"
#include "tiki/base/types.hpp"

namespace tiki
{
	template<typename TChar>
	struct StringRefData;

	template<typename TChar>
	class BasicString
	{
		friend class ParseString;
		friend class StringConvert;
		friend struct StringRefData< TChar >;

	public:

														BasicString();
														BasicString( TChar c );
														BasicString( size_t len );
														BasicString( const TChar* string );
														BasicString( const TChar* string, sint32 length );
														BasicString( const BasicString<TChar>& copy );
														~BasicString();

		TIKI_FORCE_INLINE uint32						length() const;
		TIKI_FORCE_INLINE bool							isEmpty() const;

		TIKI_FORCE_INLINE const TChar*					cStr() const;
		TIKI_FORCE_INLINE TChar*						createCStr() const;

		TIKI_FORCE_INLINE void							split( Array< BasicString< TChar > >& output, const BasicString<TChar>& seperator ) const;
		TIKI_FORCE_INLINE BasicString<TChar>			replace( TChar oldValue, TChar newValue ) const;
		TIKI_FORCE_INLINE BasicString<TChar>			replace( const BasicString<TChar>& oldValue, const BasicString<TChar>& newValue ) const;
		TIKI_FORCE_INLINE BasicString<TChar>			substring( uint32 startIndex, sint32 length = -1 ) const;
		TIKI_FORCE_INLINE BasicString<TChar>			trim() const;

		TIKI_FORCE_INLINE BasicString<TChar>			insert( const BasicString<TChar>& str, uint32 index ) const;
		TIKI_FORCE_INLINE BasicString<TChar>			remove( uint32 startIndex, uint32 len ) const;

		TIKI_FORCE_INLINE BasicString<TChar>			toLower() const;
		TIKI_FORCE_INLINE BasicString<TChar>			toUpper() const;

		TIKI_FORCE_INLINE uint32						countSubstring( const BasicString<TChar>& str ) const;
		TIKI_FORCE_INLINE sint32						indexOf( TChar c ) const;
		TIKI_FORCE_INLINE sint32						indexOf( TChar c, uint32 index ) const;
		TIKI_FORCE_INLINE sint32						indexOf( const BasicString<TChar>& str ) const;
		TIKI_FORCE_INLINE sint32						indexOf( const BasicString<TChar>& str, uint32 index ) const;
		TIKI_FORCE_INLINE sint32						lastIndexOf( TChar c ) const;
		TIKI_FORCE_INLINE sint32						lastIndexOf( const BasicString<TChar>& str ) const;

		TIKI_FORCE_INLINE bool							contains(TChar c) const;
		TIKI_FORCE_INLINE bool							contains(const BasicString<TChar>& str) const;

		TIKI_FORCE_INLINE bool							startsWith(TChar c) const;
		TIKI_FORCE_INLINE bool							startsWith(const BasicString<TChar>& str) const;
		TIKI_FORCE_INLINE bool							endsWith(TChar c) const;
		TIKI_FORCE_INLINE bool							endsWith(const BasicString<TChar>& str) const;

		TIKI_FORCE_INLINE const TChar*					operator*() const;
		TIKI_FORCE_INLINE TChar							operator[](uint32 index) const;
		TIKI_FORCE_INLINE TChar&						operator[](uint32 index);

		TIKI_FORCE_INLINE bool							operator==(const BasicString<TChar>& rhs) const;
		TIKI_FORCE_INLINE bool							operator!=(const BasicString<TChar>& rhs) const;
		TIKI_FORCE_INLINE BasicString< TChar >&			operator=(const BasicString<TChar>& rhs);

		TIKI_FORCE_INLINE BasicString<TChar>			operator+(const BasicString<TChar>& rhs) const;
		TIKI_FORCE_INLINE BasicString<TChar>&			operator+=(const BasicString<TChar>& rhs);

		TIKI_FORCE_INLINE bool							operator>(const BasicString<TChar>& rhs) const;
		TIKI_FORCE_INLINE bool							operator>=(const BasicString<TChar>& rhs) const;
		TIKI_FORCE_INLINE bool							operator<(const BasicString<TChar>& rhs) const;
		TIKI_FORCE_INLINE bool							operator<=(const BasicString<TChar>& rhs) const;

	private:

		StringRefData< TChar >*							data;

		static const TChar								letterBigA;
		static const TChar								letterBigZ;
		static const TChar								letterLittleA;
		static const TChar								letterLittleZ;
		static const TChar								numberDot;
		static const TChar								numberZero;
		static const TChar								numberNine;
		static const TChar								numberPlus;
		static const TChar								numberMinus;
		static const TChar								whiteSpaces[ 4u ];
		static StringRefData< TChar >					emptyData;

		TIKI_FORCE_INLINE void							allocData( const TChar* string, sint32 length );

		TIKI_FORCE_INLINE uint32						stringLength( const TChar* string ) const;
		TIKI_FORCE_INLINE uint32						calcLength( uint32 neededLen ) const;

	};

	TIKI_FORCE_INLINE BasicString< char >				operator+( const char* str1, const BasicString< char >& str2 );
	TIKI_FORCE_INLINE BasicString< wchar_t >			operator+( const wchar_t* str1, const BasicString< wchar_t >& str2 );

	typedef BasicString<char>		string;
	typedef BasicString<char>		String;

	typedef BasicString<wchar_t>	wstring;
	typedef BasicString<wchar_t>	WString;

	string							formatString( cstring format, ... );

	wstring							convertString( const string& str );
	string							convertString( const wstring& str );
}

#include "string.inl"

#endif // TIKI_STRING_HPP
