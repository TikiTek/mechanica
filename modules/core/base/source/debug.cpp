
#include "tiki/base/debug.hpp"

#include "tiki/base/string.hpp"

#include <stdarg.h>
#include <stdio.h>
#include <crtdbg.h>

#if TIKI_PLATFORM_WIN
#	include <windows.h>
#elif TIKI_PLATFORM_LINUX
#	warning TODO: implement __debugbreak on linux
void __debugbreak()
{
}
#endif

namespace tiki
{
	debug::TraceCallback s_pGlobalTraceCallback = nullptr;

	void trace( const string& text )
	{
		if ( debug::isDebuggerActive() )
		{
			OutputDebugStringA( text.cStr() );
		}
		else
		{
			printf( text.cStr() );
		}
	}

#pragma warning(disable: 4996)

	void internalTrace( const string& format, debug::TraceLevel level, va_list pArgs )
	{
		static cstring traceBegins[] =
		{
			"",
			"[DEBUG] ",
			"[INFO] ",
			"[WARNING] ",
			"[ERROR] "
		};

#if TIKI_PLATFORM_WIN
		const size_t len = _vsnprintf( nullptr, 0u, format.cStr(), pArgs );
		string message( len );

		_vsprintf_s_l(
			(char*)message.cStr(),
			message.length() + 1u,
			format.cStr(),
			nullptr,
			pArgs
		);

		trace( traceBegins[ level ] + message );
#elif TIKI_PLATFORM_LINUX
		const size_t len = snprintf( nullptr, 0u, format.cStr(), pArgs );
		string message( len );

		printf( (traceBegins[ level ] + message).cStr() );
#endif

		if ( s_pGlobalTraceCallback != nullptr )
		{
			s_pGlobalTraceCallback( message.cStr(), level );
		}
	}

#pragma warning(default: 4996)

	bool debug::isDebuggerActive()
	{
		return IsDebuggerPresent() != 0u;
	}

	void debug::trace( cstring format, ... )
	{
		va_list argptr;
		va_start( argptr, format );
		internalTrace( format, TraceLevel_None, argptr );
		va_end( argptr );
	}

	void debug::traceInfo( cstring format, ... )
	{
		va_list argptr;
		va_start( argptr, format );
		internalTrace( format, TraceLevel_Info, argptr );
		va_end( argptr );
	}

	void debug::traceWarning( cstring format, ... )
	{
		va_list argptr;
		va_start( argptr, format );
		internalTrace( format, TraceLevel_Warning, argptr );
		va_end( argptr );
	}

	void debug::traceError( cstring format, ... )
	{
		va_list argptr;
		va_start( argptr, format );
		internalTrace( format, TraceLevel_Error, argptr );
		va_end( argptr );
	}

	void debug::traceDebug( cstring format, ... )
	{
		va_list argptr;
		va_start( argptr, format );

#if TIKI_BUILD_DEBUG
		internalTrace( format, TraceLevel_Debug, argptr );
#endif

		va_end( argptr );
	}

	void debug::setTraceCallback( TraceCallback pCallback )
	{
		s_pGlobalTraceCallback = pCallback;
	}

	void debug::breakPoint()
	{
		if ( isDebuggerActive() )
		{
			__debugbreak();
		}
		else
		{
			trace( "Breakpoint has triggered.\n" );
			while (true);
		}
	}
	
	void debug::breakOnAlloc( int number )
	{
#if TIKI_PLATFORM_WIN
		_CrtSetBreakAlloc( number );
#elif TIKI_PLATFORM_LINUX
#warning	TODO: implement
#endif
	}

	void debug::dumpMemoryStats()
	{
#if TIKI_PLATFORM_WIN
		_CrtMemState memState;
		_CrtMemCheckpoint( &memState );

		trace( "\nMemory Leaks:\n\n" );

		if ( memState.lCounts[ 1u ] == 0u )
		{
			trace( "No Leaks!\n" );
		}
		else
		{
			_CrtDumpMemoryLeaks();
		}

		trace( "\nMemory Stats:\n\n" );
		_CrtMemDumpStatistics( &memState );
		trace( "\n" );
#elif TIKI_PLATFORM_LINUX
#warning	TODO: implement dump memory leaks on linux
#endif
	}

}