
#include "tiki/base/types.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/tikiassets/tikiassets.hpp"

int tiki::mainEntryPoint()
{
	int retValue = 0;

	//debug::breakOnAlloc( 1449 );
	{
		TikiAssets context;
		context.create( "../../../../../../content", "../../../../../../gamebuild" );

		retValue = context.run();

		context.dispose();
	}		
	//debug::dumpMemoryStats();

	return retValue;
}