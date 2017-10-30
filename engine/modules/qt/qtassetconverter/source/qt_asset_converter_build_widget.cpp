#include "tiki/qtassetconverter/qt_asset_converter_build_widget.hpp"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QVBoxLayout>

namespace tiki
{
	QtAssetConverterBuildWidget::QtAssetConverterBuildWidget( AssetConverterInterface* pConverter )
		: QDockWidget( "Asset Converter" )
	{
		m_pSearchText = new QLineEdit();
		m_pRescanButton = new QPushButton( QIcon( ":/converter_editor/button-rescan.png" ), "" );

		m_pSearchLayout = new QHBoxLayout();
		m_pSearchLayout ->setSpacing( 1 );
		m_pSearchLayout->addWidget( m_pSearchText );
		m_pSearchLayout->addWidget( m_pRescanButton );

		m_pAssetList = new QListView();
		m_pBuildButton = new QPushButton( QIcon( ":/converter_editor/button-build.png" ), "Build" );

		m_pMainLayout = new QVBoxLayout();
		m_pMainLayout->setSpacing( 1 );
		m_pMainLayout->addLayout( m_pSearchLayout );
		m_pMainLayout->addWidget( m_pAssetList );
		m_pMainLayout->addWidget( m_pBuildButton );
		m_pMainLayout->setMargin( 0 );

		m_pMainWidget = new QWidget();
		//m_pMainWidget->setContentsMargins( 0, 0, 0, 0 );
		m_pMainWidget->setLayout( m_pMainLayout );

		setWidget( m_pMainWidget );
	}

	QtAssetConverterBuildWidget::~QtAssetConverterBuildWidget()
	{
		delete m_pMainLayout;
	}
}
