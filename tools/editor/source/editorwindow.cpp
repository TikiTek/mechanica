#include "editorwindow.hpp"

#include "tiki/qtapplication/qtribbontab.hpp"

namespace tiki
{
	EditorWindow::EditorWindow()
	{
		m_pFileRibbon = new QtRibbonTab( "Package" );

		m_pFileNewButton = m_pFileRibbon->addButton( "New Package" );
		m_pFileNewButton = m_pFileRibbon->addButton( "Open Package" );
		m_pFileNewButton = m_pFileRibbon->addButton( "Save Package" );
		m_pFileNewButton = m_pFileRibbon->addButton( "New Package" );

		addRibbonTab( m_pFileRibbon );

		//saveGeometry();
		//saveState();
	}

	EditorWindow::~EditorWindow()
	{
		removeRibbonTab( m_pFileRibbon );

		delete m_pFileRibbon;
	}
}