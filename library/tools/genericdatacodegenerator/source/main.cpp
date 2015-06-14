
#include "tiki/base/debug.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/base/types.hpp"
#include "tiki/genericdata/genericdatatypecollection.hpp"

int tiki::mainEntryPoint()
{
	int retValue = 0;

	//debug::breakOnAlloc( 1449 );
	{
		GenericDataTypeCollection collection;
		collection.create( "", true );

		collection.exportCode( GenericDataTypeMode_RuntimeOnly );
	}		
	
	debug::dumpMemoryStats();

	return retValue;
}