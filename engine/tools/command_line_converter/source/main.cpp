#include "tiki/asset_converter_interface/asset_converter_interface.hpp"
#include "tiki/base/platform.hpp"
#include "tiki/threading/thread.hpp"

int tiki::mainEntryPoint()
{
	int retValue = 0;

	//debug::breakOnAlloc( 1449 );
	{
		AssetConverterParamter parameters;
		parameters.waitForConversion = true;

		AssetConverterInterface* pConverter = createAssetConverter( parameters );
		if( pConverter == nullptr )
		{
			return 1;
		}

		pConverter->startWatch();
		while( !pConverter->popFinishConversion() )
		{
			Thread::sleepCurrentThread( 100 );
		}

		retValue = pConverter->wasLastBuildSuccessful() ? 0 : 1;

		pConverter->stopWatch();
		disposeAssetConverter( pConverter );
	}

	debug::dumpMemoryStats();

	return retValue;
}