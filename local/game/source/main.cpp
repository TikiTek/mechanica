
#include "tiki/framework/gameframework.hpp"

#include "tiki/base/debug.hpp"

int tiki::mainEntryPoint()
{
	//debug::breakOnAlloc( 239 );
	
	return framework::run();
}
