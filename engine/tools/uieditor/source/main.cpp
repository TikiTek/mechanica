#include "tiki/base/platform.hpp"
#include "tiki/toolapplication/toolapplication.hpp"

#include "tiki/base/debug.hpp"

int tiki::mainEntryPoint()
{
	//debug::breakOnAlloc( 239 );

	return framework::getTool().run();
}
