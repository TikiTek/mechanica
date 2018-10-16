#include "tiki/converter_editor/converter_editor.hpp"

#include "tiki/asset_converter_interface//asset_converter_interface.hpp"
#include "tiki/editor_interface/editor_interface.hpp"

namespace tiki
{
	ConverterEditor::ConverterEditor( EditorInterface* pInterface, AssetConverterInterface* pAssetConverter )
		: BaseEditor( pInterface, "converter_editor/editable-icon.png" )
		, m_pConverter( pAssetConverter )
	{
		m_pInterface->addGlobalRibbon( &m_ribbon );
	}

	ConverterEditor::~ConverterEditor()
	{
		m_pInterface->removeGlobalRibbon( &m_ribbon );
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