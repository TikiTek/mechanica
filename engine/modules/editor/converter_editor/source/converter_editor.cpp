#include "tiki/converter_editor/converter_editor.hpp"

#include "tiki/asset_converter_interface//asset_converter_interface.hpp"
#include "tiki/editor_interface/editor_interface.hpp"

#include "res_converter_editor.hpp"

namespace tiki
{
	ConverterEditor::ConverterEditor( EditorInterface& editor, AssetConverterInterface* pAssetConverter )
		: BaseEditor( editor )
		, m_pConverter( pAssetConverter )
	{
		m_editor.addGlobalRibbon( &m_ribbon );
	}

	ConverterEditor::~ConverterEditor()
	{
		m_editor.removeGlobalRibbon( &m_ribbon );
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
		//m_pInterface->openEditable( "Asset Build", this );
	}
}