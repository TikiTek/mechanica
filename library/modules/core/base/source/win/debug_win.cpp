
#include "tiki/base/debug.hpp"

#include <stdio.h>
#include <windows.h>

#if TIKI_ENABLED( TIKI_BUILD_MSVC )
#   include <crtdbg.h>
#endif

namespace tiki
{
	void debug::nativeTrace( const char* pText )
	{
#ifdef _MANAGED
		System::Console::WriteLine( gcnew System::String( pText ) );
#else
		OutputDebugStringA( pText );
#endif

		if ( !debug::isDebuggerActive() )
		{
			printf( pText );
		}
	}

	bool debug::isDebuggerActive()
	{
		return IsDebuggerPresent() != 0u;
	}

	void debug::breakPoint()
	{
#if TIKI_ENABLED( TIKI_BUILD_MSVC )
		if ( isDebuggerActive() )
		{
			__debugbreak();
		}
		else
#endif
		{
			debug::nativeTrace( "Breakpoint has triggered.\n" );
			while (true);
		}
	}

	void debug::breakOnAlloc( int number )
	{
#if TIKI_ENABLED( TIKI_BUILD_MSVC )
		_CrtSetBreakAlloc( number );
#endif
	}

	void debug::dumpMemoryStats()
	{
#if TIKI_ENABLED( TIKI_BUILD_DEBUG ) && TIKI_ENABLED( TIKI_BUILD_MSVC )
		_CrtMemState memState;
		_CrtMemCheckpoint( &memState );

		debug::nativeTrace( "\nMemory Stats:\n\n" );
		_CrtMemDumpStatistics( &memState );

		debug::nativeTrace( "\nMemory Leaks:\n\n" );

		if ( memState.lCounts[ 1u ] == 0u )
		{
			debug::nativeTrace( "No Leaks!\n" );
		}
		else
		{
			_CrtDumpMemoryLeaks();
		}

		debug::nativeTrace( "\n" );
#endif
	}
}
