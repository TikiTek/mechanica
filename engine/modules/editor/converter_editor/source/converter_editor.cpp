#include "tiki/converter_editor/converter_editor.hpp"

#include "tiki/asset_converter_interface//asset_converter_interface.hpp"
#include "tiki/editor_interface/ieditorinterface.hpp"
#include "tiki/qtapplication/qtribbonbutton.hpp"
#include "tiki/qtapplication/qtribbontab.hpp"
#include "tiki/qtassetconverter/qt_asset_converter_build_widget.hpp"
#include "tiki/qtassetconverter/qt_asset_converter_status_widget.hpp"

#include <QResource>

namespace tiki
{
	ConverterEditor::ConverterEditor( IEditorInterface* pInterface, AssetConverterInterface* pAssetConverter )
		: m_pInterface( pInterface )
		, m_pConverter( pAssetConverter )
	{
		QResource::registerResource( "converter_editor.rcc" );

		m_icon = QIcon( ":/converter_editor/editable-icon.png" );

		m_pRibbon = new QtRibbonTab( "Converter" );
		m_pConvertAllButton		= m_pRibbon->addButton( "Convert all", QIcon( ":/converter_editor/ribbon-convert-all.png" ) );
		m_pShowConverterButton	= m_pRibbon->addButton( "Show Converter", QIcon( ":/converter_editor/ribbon-show-converter.png" ) );
		m_pShowLastResultButton	= m_pRibbon->addButton( "Show last result", QIcon( ":/converter_editor/ribbon-show-last-result.png" ) );

		connect( m_pConvertAllButton, &QtRibbonButton::clicked, this, &ConverterEditor::convertAllClicked );
		connect( m_pShowConverterButton, &QtRibbonButton::clicked, this, &ConverterEditor::showConverterClicked );
		connect( m_pShowLastResultButton, &QtRibbonButton::clicked, this, &ConverterEditor::showLastResultClicked );

		m_pBuildWidget = new QtAssetConverterBuildWidget( pAssetConverter );
		m_pStatusWidget = new QtAssetConverterStatusWidget();

		m_pInterface->addGlobalRibbonTab( m_pRibbon );
		m_pInterface->addGlobalDockWidget( m_pBuildWidget, Qt::RightDockWidgetArea );
	}

	ConverterEditor::~ConverterEditor()
	{
		delete m_pBuildWidget;
		delete m_pStatusWidget;

		m_pInterface->removeGlobalRibbonTab( m_pRibbon );
		delete m_pRibbon;
	}

	bool ConverterEditor::saveEditable( IEditable* pEditable )
	{
		return true;
	}

	void ConverterEditor::closeEditable( IEditable* pEditable )
	{
	}

	QIcon ConverterEditor::getEditableIcon() const
	{
		return m_icon;
	}

	void ConverterEditor::convertAllClicked()
	{
		//m_pConverter->convertAll();
	}

	void ConverterEditor::showConverterClicked()
	{
		//m_pInterface->openEditable( "Asset Converter", m_pBuildWidget, this );
	}

	void ConverterEditor::showLastResultClicked()
	{
		m_pInterface->openEditable( "Asset Build", m_pStatusWidget, this );
	}
}