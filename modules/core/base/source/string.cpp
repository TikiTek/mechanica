
#include "tiki/base/string.hpp"

#include <string>

namespace tiki
{
	template<> const char BasicString<char>::letterBigA				= 'A';
	template<> const char BasicString<char>::letterBigZ				= 'Z';
	template<> const char BasicString<char>::letterLittleA			= 'a';
	template<> const char BasicString<char>::letterLittleZ			= 'z';
	template<> const char BasicString<char>::numberDot				= '.';
	template<> const char BasicString<char>::numberZero				= '0';
	template<> const char BasicString<char>::numberNine				= '9';
	template<> const char BasicString<char>::numberPlus				= '+';
	template<> const char BasicString<char>::numberMinus			= '-';
	template<> const char BasicString<char>::whiteSpaces[ 4u ]		= { ' ', '\t', '\r', '\n' };
	template<> StringRefData<char> BasicString<char>::emptyData		= StringRefData<char>();

	template<> const wchar_t BasicString<wchar_t>::letterBigA			= L'A';
	template<> const wchar_t BasicString<wchar_t>::letterBigZ			= L'Z';
	template<> const wchar_t BasicString<wchar_t>::letterLittleA		= L'a';
	template<> const wchar_t BasicString<wchar_t>::letterLittleZ		= L'z';
	template<> const wchar_t BasicString<wchar_t>::numberDot			= L'.';
	template<> const wchar_t BasicString<wchar_t>::numberZero			= L'0';
	template<> const wchar_t BasicString<wchar_t>::numberNine			= L'9';
	template<> const wchar_t BasicString<wchar_t>::numberPlus			= L'+';
	template<> const wchar_t BasicString<wchar_t>::numberMinus			= L'-';
	template<> const wchar_t BasicString<wchar_t>::whiteSpaces[ 4u ]	= { L' ', L'\t', L'\r', L'\n' };
	template<> StringRefData<wchar_t> BasicString<wchar_t>::emptyData	= StringRefData<wchar_t>();

	string formatString( cstring format, ... )
	{
		va_list argptr;
		va_start( argptr, format );

#if TIKI_PLATFORM_WIN

#pragma warning( disable : 4996 ) //maybe unsafe
		const size_t len = _vsnprintf( nullptr, 0u, format, argptr );
#pragma warning( default : 4996 )
		
		string str( len );

		_vsprintf_s_l(
			(char*)str.cStr(),
			str.length() + 1u,
			format,
			nullptr,
			argptr
		);
#elif TIKI_PLATFORM_LINUX
		size_t len = snprintf( nullptr, 0u, 0u, format, argptr );
		string str( len );

		sprintf( (char*)str.cStr(), format, argptr );
#endif

		va_end( argptr );

		return str;
	}

	wstring convertString( const string& str )
	{
		wstring result( str.length() );

		for (size_t i = 0u; i < str.length(); ++i)
		{
			result[ i ] = str[ i ];
		}

		return result;
	}

	string convertString( const wstring& str )
	{
		string result( str.length() );

		for (size_t i = 0u; i < str.length(); ++i)
		{
			result[ i ] = (char)str[ i ];
		}

		return result;
	}

}


