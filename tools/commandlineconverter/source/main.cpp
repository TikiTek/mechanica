
#include "tiki/base/types.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/toollibrarys/iassetconverter.hpp"

int tiki::mainEntryPoint()
{
	int retValue = 0;

	//debug::breakOnAlloc( 1449 );
	{
		AssetConverterParamter parameters;
		parameters.sourcePath	= "../../../../../../content";
		parameters.outputPath	= "../../../../../../gamebuild";
		parameters.forceRebuild	= hasArgument( "--rebuild" );

		IAssetConverter* pConverter = createAssetConverter();
		pConverter->create( parameters );

		retValue = pConverter->convertAll();

		pConverter->dispose();
		disposeAssetConverter( pConverter );
	}		
	//debug::dumpMemoryStats();

	return retValue;
}