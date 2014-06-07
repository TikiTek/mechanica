
#include "tiki/base/debug.hpp"

#include "tiki/base/string.hpp"

#include <stdarg.h>
#include <stdio.h>

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )
#	include <crtdbg.h>
#	include <windows.h>
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )

#endif

namespace tiki
{
	debug::TraceCallback s_pGlobalTraceCallback = nullptr;

	void debug::trace( const char* pFormat, TraceLevel level, va_list pArgs )
	{
#pragma warning(disable: 4996)
		static cstring s_aTracePrefix[] =
		{
			"",
			"[DEBUG] ",
			"[INFO] ",
			"[WARNING] ",
			"[ERROR] "
		};
		const uint prefixStringLength		= strlen( s_aTracePrefix[ level ] );
		const uint formattedStringLength	= _vsnprintf( nullptr, 0u, pFormat, pArgs );

		string message( prefixStringLength + formattedStringLength );
		copyString( (char*)message.cStr(), prefixStringLength + 1u, s_aTracePrefix[ level ] );

		_vsprintf_s_l(
			(char*)( message.cStr() + prefixStringLength ),
			formattedStringLength + 1u,
			pFormat,
			nullptr,
			pArgs
		);

		debug::nativeTrace( message.cStr() );

		if ( s_pGlobalTraceCallback != nullptr )
		{
			s_pGlobalTraceCallback( message.cStr(), level );
		}
#pragma warning(default: 4996)
	}

	void debug::traceInfo( const char* pFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pFormat );
		debug::trace( pFormat, TraceLevel_Info, argptr );
		va_end( argptr );
	}

	void debug::traceWarning( const char* pFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pFormat );
		debug::trace( pFormat, TraceLevel_Warning, argptr );
		va_end( argptr );
	}

	void debug::traceError( const char* pFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pFormat );
		debug::trace( pFormat, TraceLevel_Error, argptr );
		va_end( argptr );
	}

	void debug::traceDebug( const char* pFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pFormat );

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		debug::trace( pFormat, TraceLevel_None, argptr );
#endif

		va_end( argptr );
	}

	void debug::setTraceCallback( TraceCallback pCallback )
	{
		s_pGlobalTraceCallback = pCallback;
	}
}