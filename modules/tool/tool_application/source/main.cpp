#include "tiki/base/platform.hpp"
#include "tiki/toolapplication/tool_application.hpp"

#include "tiki/base/debug.hpp"

int tiki::mainEntryPoint()
{
	//debug::breakOnAlloc( 239 );

	return tool::getTool().run();
}
