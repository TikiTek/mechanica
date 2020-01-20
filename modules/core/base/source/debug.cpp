
#include "tiki/base/debug.hpp"

#include "tiki/base/string.hpp"
#include "tiki/base/dynamic_string.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#if TIKI_ENABLED( TIKI_BUILD_MSVC )
#	include <crtdbg.h>
#	include <windows.h>
#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )

#endif

namespace tiki
{
	TraceCallback s_pGlobalTraceCallback = nullptr;
	UserData s_globalTraceCallbackUserData;

	namespace debug
	{
		void traceInternal( const char* pFormat, TraceLevel level, va_list pArgs );
	}

	void debug::traceInternal( const char* pFormat, TraceLevel level, va_list pArgs )
	{
#if TIKI_ENABLED( TIKI_BUILD_MSVC )
#	pragma warning(push)
#	pragma warning(disable: 4996)
		const uint formattedStringLength = _vsnprintf( nullptr, 0u, pFormat, pArgs );
#else
		va_list startList = { *pArgs };
		const uint formattedStringLength = vsnprintf( nullptr, 0u, pFormat, pArgs );
		pArgs = startList;
#endif

		string message( formattedStringLength );
#if TIKI_ENABLED( TIKI_BUILD_MSVC )
		_vsprintf_s_l(
			(char*)message.cStr(),
			formattedStringLength + 1u,
			pFormat,
			nullptr,
			pArgs
		);
#	pragma warning(pop)
#else
		vsnprintf(
			(char*)message.cStr(),
			formattedStringLength + 1u,
			pFormat,
			pArgs
		);
#endif

		debug::nativeTrace( message.cStr() );

		if ( s_pGlobalTraceCallback != nullptr )
		{
			s_pGlobalTraceCallback( message.cStr(), level, s_globalTraceCallbackUserData );
		}
	}

	void debug::trace( const char* pFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pFormat );
		debug::traceInternal( pFormat, TraceLevel_None, argptr );
		va_end( argptr );
	}

	void debug::traceInfo( const char* pFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pFormat );
		debug::traceInternal( pFormat, TraceLevel_Info, argptr );
		va_end( argptr );
	}

	void debug::traceWarning( const char* pFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pFormat );
		debug::traceInternal( pFormat, TraceLevel_Warning, argptr );
		va_end( argptr );
	}

	void debug::traceError( const char* pFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pFormat );
		debug::traceInternal( pFormat, TraceLevel_Error, argptr );
		va_end( argptr );
	}

	void debug::traceDebug( const char* pFormat, ... )
	{
		va_list argptr;
		va_start( argptr, pFormat );

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		debug::traceInternal( pFormat, TraceLevel_Debug, argptr );
#endif

		va_end( argptr );
	}

	void debug::setTraceCallback( TraceCallback pCallback, UserData userData )
	{
		s_pGlobalTraceCallback = pCallback;
		s_globalTraceCallbackUserData = userData;
	}
}
