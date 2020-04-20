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

		pConverter->queueAll();

		while( pConverter->isConvertionRunning() )
		{
			Thread::sleepCurrentThread( 100 );
		}

		disposeAssetConverter( pConverter );
	}

	debug::dumpMemoryStats();

	return retValue;
}