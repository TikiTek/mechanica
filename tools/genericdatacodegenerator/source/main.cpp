
#include "tiki/base/debug.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"
#include "tiki/genericdata/genericdatatypecollection.hpp"

int tiki::mainEntryPoint()
{
	int retValue = 0;

	//debug::breakOnAlloc( 1449 );
	{
		string sourceDir = "../../../../../../content";
		string targetDir = "../../genericdatatypes";

		for (uint i = 0u; i < platform::getArguments().getCount(); ++i)
		{
			const string arg = platform::getArguments()[ i ];

			if ( arg.startsWith( "--content-dir=" ) )
			{
				sourceDir = arg.subString( getStringLength( "--content-dir=" ) + 1 );
			}
			else if ( arg.startsWith( "--target-dir=" ) )
			{
				targetDir = arg.subString( getStringLength( "--target-dir=" ) + 1 );
			}
		}

		GenericDataTypeCollection collection;
		collection.create( sourceDir, true );
		if ( !collection.exportCode( GenericDataTypeMode_RuntimeOnly, targetDir ) )
		{
			TIKI_TRACE_ERROR( "[genericdatacodegenerator] code generation finish with some errors.\n" );
		}
		else
		{
			TIKI_TRACE_INFO( "[genericdatacodegenerator] code generation successfully.\n" );
		}

		collection.dispose();
	}		
	
	return retValue;
}