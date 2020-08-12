
#include "tiki/base/types.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/toollibraries/iassetconverter.hpp"

int tiki::mainEntryPoint()
{
	int retValue = 0;

	//debug::breakOnAlloc( 1449 );
	{
		AssetConverterParamter parameters;
		parameters.sourcePath	= "../../../../../../content";
		parameters.outputPath	= "../../../../../../gamebuild";
		parameters.forceRebuild	= platform::hasArgument( "--rebuild" );

		for( uint i = 0u; i < platform::getArguments().getCount(); ++i )
		{
			const string arg = platform::getArguments()[ i ];

			if( arg.startsWith( "--content-dir=" ) )
			{
				parameters.sourcePath = arg.subString( getStringSize( "--content-dir=" ) );
			}
			else if( arg.startsWith( "--target-dir=" ) )
			{
				parameters.outputPath = arg.subString( getStringSize( "--target-dir=" ) );
			}
		}

		IAssetConverter* pConverter = createAssetConverter();
		pConverter->create( parameters );

		retValue = pConverter->convertAll();

		pConverter->dispose();
		disposeAssetConverter( pConverter );
	}		
	
	debug::dumpMemoryStats();

	return retValue;
}