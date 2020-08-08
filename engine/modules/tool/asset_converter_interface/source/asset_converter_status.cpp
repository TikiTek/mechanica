#include "tiki/asset_converter_interface/asset_converter_status.hpp"

namespace tiki
{
	void AssetConverterStatus::addChangedListener( const ChangedDelegate& listener )
	{
		m_changedListener.add( listener );
	}

	void AssetConverterStatus::removeChangedListener( const ChangedDelegate& listener )
	{

	}

	void AssetConverterStatus::notifyChangedListner()
	{
		for( const ChangedDelegate& listener : m_changedListener )
		{
			listener.invoke( 0 );
		}
	}
}