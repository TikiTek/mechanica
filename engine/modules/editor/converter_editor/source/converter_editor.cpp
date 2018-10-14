#include "tiki/converter_editor/converter_editor.hpp"

#include "tiki/asset_converter_interface//asset_converter_interface.hpp"
#include "tiki/editor_interface/editor_interface.hpp"

namespace tiki
{
	ConverterEditor::ConverterEditor( EditorInterface* pInterface, AssetConverterInterface* pAssetConverter )
		: BaseEditor( pInterface, "converter_editor/editable-icon.png" )
		, m_pConverter( pAssetConverter )
	{
		//m_pRibbon = new QtRibbonTab( "Converter" );
		//m_pConvertAllButton		= m_pRibbon->addButton( "Convert all", QIcon( ":/converter_editor/ribbon-convert-all.png" ) );
		//m_pShowConverterButton	= m_pRibbon->addButton( "Show Converter", QIcon( ":/converter_editor/ribbon-show-converter.png" ) );
		//m_pShowLastResultButton	= m_pRibbon->addButton( "Show last result", QIcon( ":/converter_editor/ribbon-show-last-result.png" ) );

		m_pInterface->addGlobalRibbon( nullptr );
	}

	ConverterEditor::~ConverterEditor()
	{
		m_pInterface->removeGlobalRibbon( nullptr );
	}

	bool ConverterEditor::saveEditable( Editable* pEditable )
	{
		return true;
	}

	void ConverterEditor::closeEditable( Editable* pEditable )
	{
	}

	void ConverterEditor::convertAll()
	{
		//m_pConverter->convertAll();
	}

	void ConverterEditor::showConverter()
	{
		//m_pInterface->openEditable( "Asset Converter", m_pBuildWidget, this );
	}

	void ConverterEditor::showLastResult()
	{
		m_pInterface->openEditable( "Asset Build", this );
	}
}