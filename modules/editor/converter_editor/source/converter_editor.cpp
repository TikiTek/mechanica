#include "tiki/converter_editor/converter_editor.hpp"

#include "tiki/editor_interface/ieditorinterface.hpp"
#include "tiki/qtapplication/qtribbonbutton.hpp"
#include "tiki/qtapplication/qtribbontab.hpp"

#include <QResource>

namespace tiki
{
	ConverterEditor::ConverterEditor()
	{
		QResource::registerResource( "converter_editor.rcc" );

		m_pRibbon = new QtRibbonTab( "Converter" );
		m_pConvertAllButton		= m_pRibbon->addButton( "Convert all", QIcon( ":/converter-editor/ribbon-convert-all.png" ) );
		m_pShowConverterButton	= m_pRibbon->addButton( "Show Converter", QIcon( ":/converter-editor/ribbon-show-converter.png" ) );
		m_pShowLastResultButton	= m_pRibbon->addButton( "Show last result", QIcon( ":/converter-editor/ribbon-show-last-result.png" ) );

		connect( m_pConvertAllButton, &QtRibbonButton::clicked, this, &ConverterEditor::convertAllClicked );
		connect( m_pShowConverterButton, &QtRibbonButton::clicked, this, &ConverterEditor::showConverterClicked );
		connect( m_pShowLastResultButton, &QtRibbonButton::clicked, this, &ConverterEditor::showLastResultClicked );

		m_globalTabs.insert( m_pRibbon );
	}

	ConverterEditor::~ConverterEditor()
	{
		delete m_pRibbon;
	}

	void ConverterEditor::convertAllClicked()
	{
	}

	void ConverterEditor::showConverterClicked()
	{
	}

	void ConverterEditor::showLastResultClicked()
	{
	}
}