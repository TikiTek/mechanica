#include "tiki/base/debug.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/base/types.hpp"
#include "tiki/tool_generic_data/generic_data_type_collection.hpp"
#include "tiki/tool_project/package.hpp"
#include "tiki/tool_project/project.hpp"

int tiki::mainEntryPoint()
{
	int retValue = 0;

	//debug::breakOnAlloc( 1449 );
	{
		Path sourceDir( "../../../../../../content" );
		Path targetDir( "../../genericdatatypes" );

		const char* pValue = nullptr;
		if( platform::findArgumentValue( &pValue, "--content-dir" ) )
		{
			sourceDir.setCompletePath( pValue );
		}

		if( platform::findArgumentValue( &pValue, "--target-dir" ) )
		{
			targetDir.setCompletePath( pValue );
		}

		GenericDataTypeCollection collection;

		Project project( sourceDir );
		for( const Package& package : project.getPackages() )
		{
			collection.addPackage( package );
		}

		if ( !collection.exportCode( GenericDataTypeMode_RuntimeOnly, targetDir ) )
		{
			TIKI_TRACE_ERROR( "[genericdatacodegenerator] code generation finish with some errors.\n" );

			retValue = -1;
		}
		else
		{
			TIKI_TRACE_INFO( "[genericdatacodegenerator] code generation successfull.\n" );
		}
	}

	return retValue;
}