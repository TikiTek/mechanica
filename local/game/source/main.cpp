
#include "tiki/framework/gameframework.hpp"

#include "tiki/base/platform.hpp"

int tiki::mainEntryPoint()
{
	using namespace tiki;

	//debug::breakOnAlloc( 143 );
	{
		framework::run();
	}

	debug::dumpMemoryStats();

	return 0;
}
