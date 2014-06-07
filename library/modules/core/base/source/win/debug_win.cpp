
#include "tiki/base/debug.hpp"

namespace tiki
{
	void debug::nativeTrace( const char* pText )
	{
		if ( debug::isDebuggerActive() )
		{
			OutputDebugStringA( pText );
		}
		else
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
		if ( isDebuggerActive() )
		{
			__debugbreak();
		}
		else
		{
			debug::nativeTrace( "Breakpoint has triggered.\n" );
			while (true);
		}
	}

	void debug::breakOnAlloc( int number )
	{
		_CrtSetBreakAlloc( number );
	}

	void debug::dumpMemoryStats()
	{
#if TIKI_ENABLED( TIKI_BUILD_DEBUG )
		_CrtMemState memState;
		_CrtMemCheckpoint( &memState );

		debug::nativeTrace( "\nMemory Leaks:\n\n" );

		if ( memState.lCounts[ 1u ] == 0u )
		{
			debug::nativeTrace( "No Leaks!\n" );
		}
		else
		{
			_CrtDumpMemoryLeaks();
		}

		debug::nativeTrace( "\nMemory Stats:\n\n" );
		_CrtMemDumpStatistics( &memState );
		debug::nativeTrace( "\n" );
#endif
	}	
}