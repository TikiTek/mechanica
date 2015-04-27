
#include "tiki/base/string.hpp"

#include <cstdarg>
#include <cstdio>

namespace tiki
{
#if TIKI_ENABLED( TIKI_BUILD_MSVC )

	void formatStringBuffer( char* pTargetBuffer, uint bufferSize, cstring format, ... )
	{
		va_list argptr;
		va_start( argptr, format );

		_vsprintf_s_l(
			pTargetBuffer,
			bufferSize,
			format,
			nullptr,
			argptr
			);

		va_end( argptr );
	}

#elif TIKI_ENABLED( TIKI_BUILD_MINGW )

	void formatStringBuffer( char* pTargetBuffer, uint bufferSize, cstring format, ... )
	{
		va_list argptr;
		va_start( argptr, format );

		sprintf( pTargetBuffer, format, argptr );

		va_end( argptr );
	}

#endif
}