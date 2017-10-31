#include "tiki/qtassetconverter/qt_asset_converter_status_widget.hpp"

#include "tiki/asset_converter_interface/asset_converter_status.hpp"

namespace tiki
{
	QtAssetConverterStatusWidget::QtAssetConverterStatusWidget()
	{
		m_pStatus = nullptr;
	}

	QtAssetConverterStatusWidget::~QtAssetConverterStatusWidget()
	{
	}

	void QtAssetConverterStatusWidget::setAssetConverterStatus( const AssetConverterStatus* pStatus )
	{
		AssetConverterStatus::ChangedDelegate changedDelegate;
		changedDelegate.setInstanceMethod< QtAssetConverterStatusWidget, &QtAssetConverterStatusWidget::updateStatus >( this );

		if( m_pStatus != nullptr )
		{
			m_pStatus->removeChangedListener( changedDelegate );
		}
	}

	void QtAssetConverterStatusWidget::updateStatus( int test )
	{

	}
}
