
#include "tiki/base/platform.hpp"
#include "tiki/framework/gameapplication.hpp"

#include "tiki/base/debug.hpp"

int tiki::mainEntryPoint()
{
	//debug::breakOnAlloc( 239 );
	
	return framework::run();
}
