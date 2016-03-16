
#include "tiki/base/basicstring.hpp"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

namespace tiki
{
	const char* BasicString::StringRefData::s_pEmptyString	= "";

	const char BasicString::letterBigA						= 'A';
	const char BasicString::letterBigZ						= 'Z';
	const char BasicString::letterLittleA					= 'a';
	const char BasicString::letterLittleZ					= 'z';
	const char BasicString::numberDot						= '.';
	const char BasicString::numberZero						= '0';
	const char BasicString::numberNine						= '9';
	const char BasicString::numberPlus						= '+';
	const char BasicString::numberMinus						= '-';
	const char BasicString::whiteSpaces[ 4u ]				= { ' ', '\t', '\r', '\n' };
	BasicString::StringRefData BasicString::emptyData		= StringRefData();

	string formatString( const char* pFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pFormat );
		const string result = formatStringArgs( pFormat, argptr );
		va_end( argptr );

		return result;
	}

#if TIKI_ENABLED( TIKI_BUILD_MSVC )

	string formatStringArgs( const char* pFormat, va_list argptr )
	{
		va_list argptrCount = argptr;
		va_list argptrFormat = argptr;

#pragma warning( disable : 4996 )
		const size_t len = _vsnprintf( nullptr, 0u, pFormat, argptrCount );
#pragma warning( default : 4996 )

		string str( len );
		_vsprintf_s_l(
			(char*)str.cStr(),
			str.getLength() + 1u,
			pFormat,
			nullptr,
			argptrFormat
		);

		return str;
	}

#elif TIKI_ENABLED( TIKI_BUILD_MINGW )

	string formatStringArgs( const char* pFormat, va_list argptr )
	{
		va_list argptrCount = { argptr[ 0 ] };
		va_list argptrFormat = { argptr[ 0 ] };

		const size_t len = vsnprintf( nullptr, 0u, pFormat, argptrCount );

		string str( len );
		vsnprintf( (char*)str.cStr(), len + 1u, pFormat, argptrFormat );

		va_end( argptr );

		return str;
	}

#endif
}


