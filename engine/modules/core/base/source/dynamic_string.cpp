
#include "tiki/base/dynamic_string.hpp"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

namespace tiki
{
	DynamicString formatDynamicString( const char* pFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pFormat );
		const DynamicString result = formatDynamicStringArgs( pFormat, argptr );
		va_end( argptr );

		return result;
	}

#if TIKI_ENABLED( TIKI_BUILD_MSVC )

	DynamicString formatDynamicStringArgs( const char* pFormat, va_list argptr )
	{
		va_list argptrCount = argptr;
		va_list argptrFormat = argptr;

#pragma warning( disable : 4996 )
		const uintreg len = _vsnprintf( nullptr, 0u, pFormat, argptrCount );
#pragma warning( default : 4996 )

		DynamicString str( len );
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

	DynamicString formatDynamicStringArgs( const char* pFormat, va_list argptr )
	{
		va_list argptrCount = { argptr[ 0 ] };
		va_list argptrFormat = { argptr[ 0 ] };

		const uintreg len = vsnprintf( nullptr, 0u, pFormat, argptrCount );

		DynamicString str( len );
		vsnprintf( (char*)str.cStr(), len + 1u, pFormat, argptrFormat );

		return str;
	}

#endif

	DynamicString operator ""_s( const char* pString, uintreg length )
	{
		return DynamicString( pString, length );
	}
}


