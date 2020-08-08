#include "tiki/base/debug.hpp"
#include "tiki/base/path.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/base/types.hpp"
#include "tiki/tool_generic_data/generic_data_type_collection.hpp"
#include "tiki/tool_project/package.hpp"
#include "tiki/tool_project/project.hpp"

namespace tiki
{
	void printUsage()
	{
		TIKI_TRACE_ERROR( "Usage: generic_data_code_generator --content-dir=%CONTENT_DIR% --target-dir=%TARGET_DIR%\n" );
	}

	int mainEntryPoint()
	{
		const char* pValue = nullptr;
		if( !platform::findArgumentValue( &pValue, "--content-dir" ) )
		{
			printUsage();
			return 1;
		}
		Path sourceDir( pValue );

		if( !platform::findArgumentValue( &pValue, "--target-dir" ) )
		{
			printUsage();
			return 1;
		}
		Path targetDir( pValue );

		GenericDataTypeCollection collection;

		Project project;
		if( !project.isValid() )
		{
			return 1;
		}

		for( const Package& package : project.getPackages() )
		{
			collection.addPackage( package );
		}

		if( !collection.exportCode( GenericDataTypeMode_RuntimeOnly, targetDir ) )
		{
			TIKI_TRACE_ERROR( "code generation finished with errors.\n" );
			return 1;
		}

		return 0;
	}
}