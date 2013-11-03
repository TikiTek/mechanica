
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
	namespace debug
	{
		static void internalTrace( const string& format, debug::TraceLevel level, va_list pArgs );
	}

	debug::TraceCallback s_pGlobalTraceCallback = nullptr;

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

#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		internalTrace( format, TraceLevel_Debug, argptr );
#endif

		va_end( argptr );
	}

	void debug::setTraceCallback( TraceCallback pCallback )
	{
		s_pGlobalTraceCallback = pCallback;
	}

#if TIKI_ENABLED( TIKI_PLATFORM_WIN )

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

	static void debug::internalTrace( const string& format, debug::TraceLevel level, va_list pArgs )
	{
		static cstring traceBegins[] =
		{
			"",
			"[DEBUG] ",
			"[INFO] ",
			"[WARNING] ",
			"[ERROR] "
		};

		const size_t len = _vsnprintf( nullptr, 0u, format.cStr(), pArgs );
		string message( len );

		_vsprintf_s_l(
			(char*)message.cStr(),
			message.getLength() + 1u,
			format.cStr(),
			nullptr,
			pArgs
		);

		trace( traceBegins[ level ] + message );

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
		_CrtSetBreakAlloc( number );
	}

	void debug::dumpMemoryStats()
	{
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
	}

#elif TIKI_ENABLED( TIKI_PLATFORM_LINUX )

	static void debug::internalTrace( const string& format, debug::TraceLevel level, va_list pArgs )
	{
	}

	#pragma warning(default: 4996)

	bool debug::isDebuggerActive()
	{
		return false;
	}

	void debug::breakPoint()
	{
		if ( isDebuggerActive() )
		{
			/*__debugbreak();*/
		}
		else
		{
			trace( "Breakpoint has triggered.\n" );
			while (true);
		}
	}

	void debug::breakOnAlloc( int number )
	{
		/*_CrtSetBreakAlloc( number );*/
	}

	void debug::dumpMemoryStats()
	{
	}

#else
#	error Platform not impemented
#endif
}