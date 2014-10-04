
#include "tiki/framework/gameframework.hpp"

#include "tiki/base/debug.hpp"

int tiki::mainEntryPoint()
{
	//debug::breakOnAlloc( 268 );
	
	framework::run();

	return 0;
}
