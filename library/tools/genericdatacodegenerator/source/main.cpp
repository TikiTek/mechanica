
#include "tiki/base/debug.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/base/types.hpp"

int tiki::mainEntryPoint()
{
	int retValue = 0;

	//debug::breakOnAlloc( 1449 );
	{

	}		
	
	debug::dumpMemoryStats();

	return retValue;
}