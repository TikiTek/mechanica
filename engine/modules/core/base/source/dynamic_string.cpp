
#include "tiki/base/dynamic_string.hpp"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

namespace tiki
{
	string formatDynamicString( const char* pFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pFormat );
		const string result = formatDynamicStringArgs( pFormat, argptr );
		va_end( argptr );

		return result;
	}

#if TIKI_ENABLED( TIKI_BUILD_MSVC )

	string formatDynamicStringArgs( const char* pFormat, va_list argptr )
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

#elif TIKI_ENABLED( TIKI_BUILD_GCC ) || TIKI_ENABLED( TIKI_BUILD_CLANG )

	string formatDynamicStringArgs( const char* pFormat, va_list argptr )
	{
		va_list argptrCount = { argptr[ 0 ] };
		va_list argptrFormat = { argptr[ 0 ] };

		const size_t len = vsnprintf( nullptr, 0u, pFormat, argptrCount );

		string str( len );
		vsnprintf( (char*)str.cStr(), len + 1u, pFormat, argptrFormat );

		return str;
	}

#endif
}


