
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

#if TIKI_ENABLED( TIKI_BUILD_MSVC )

	string formatString( cstring format, ... )
	{
		va_list argptr;
		va_start( argptr, format );

#pragma warning( disable : 4996 )
		const size_t len = _vsnprintf( nullptr, 0u, format, argptr );
#pragma warning( default : 4996 )

		string str( len );

		va_start( argptr, format );

		_vsprintf_s_l(
			(char*)str.cStr(),
			str.getLength() + 1u,
			format,
			nullptr,
			argptr
		);

		va_end( argptr );

		return str;
	}

#elif TIKI_ENABLED( TIKI_BUILD_MINGW )

	string formatString( cstring format, ... )
	{
		va_list argptr;
		va_start( argptr, format );
		va_list startList = { argptr[ 0 ] };
		
		const size_t len = vsnprintf( nullptr, 0u, format, argptr );
		string str( len );
		
		argptr[ 0 ] = startList[ 0 ];
		vsnprintf( (char*)str.cStr(), len + 1u, format, argptr );

		va_end( argptr );

		return str;
	}

#endif
}


