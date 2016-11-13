#include "editorwindow.hpp"

#include "tiki/qtapplication/qtribbontab.hpp"

namespace tiki
{
	EditorWindow::EditorWindow()
	{
		m_pFileTabs = new QTabWidget();
		m_pFileTabs->setTabsClosable( true );

		connect( m_pFileTabs, &QTabWidget::tabCloseRequested, this, &EditorWindow::fileCloseRequested );

		addWidget( m_pFileTabs );

		//m_pFileRibbon = new QtRibbonTab( "Package" );

		//m_pFileNewButton = m_pFileRibbon->addButton( "New Package" );
		//m_pFileNewButton = m_pFileRibbon->addButton( "Open Package" );
		//m_pFileNewButton = m_pFileRibbon->addButton( "Save Package" );
		//m_pFileNewButton = m_pFileRibbon->addButton( "New Package" );

		//addRibbonTab( m_pFileRibbon );

		//saveGeometry();
		//saveState();
	}

	EditorWindow::~EditorWindow()
	{
		delete m_pFileTabs;
	}

	void EditorWindow::openFileTab( const QString& title, QWidget* pWidget )
	{
		m_pFileTabs->addTab( pWidget, title );
	}

	void EditorWindow::closeFileTab( QWidget* pWidget )
	{
		int index = -1;
		for( int i = 0u; i < m_pFileTabs->count(); ++i )
		{
			if( m_pFileTabs->widget( i ) )
			{
				index = i;
				break;
			}
		}

		if( index >= 0 )
		{
			m_pFileTabs->removeTab( index );
		}
	}

	void EditorWindow::fileCloseRequested( int index )
	{
		emit fileCloseRequest( m_pFileTabs->widget( index ) );
	}
}