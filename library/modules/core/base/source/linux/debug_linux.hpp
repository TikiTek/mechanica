
#include "tiki/base/debug.hpp"

namespace tiki
{
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
}