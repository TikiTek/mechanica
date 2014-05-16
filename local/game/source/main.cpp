
#include "tiki/framework/gameframework.hpp"

#include "tiki/base/debug.hpp"

int tiki::mainEntryPoint()
{
	//debug::breakOnAlloc( 2895 );
	
	framework::run();

	return 0;
}
