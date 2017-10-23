#include "tiki/base/debug.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/base/types.hpp"
#include "tiki/toolgenericdata/generic_data_type_collection.hpp"
#include "tiki/toolpackage/package.hpp"
#include "tiki/toolpackage/package_manager.hpp"

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
		collection.create();

		PackageManager packageManager;
		packageManager.create( sourceDir );

		for( const Package& package : packageManager.getPackages() )
		{
			collection.addPackage( &package );
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

		collection.dispose();
		packageManager.dispose();
	}

	return retValue;
}