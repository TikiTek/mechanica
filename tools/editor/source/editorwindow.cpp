#include "editorwindow.hpp"

#include "tiki/qtapplication/qtribbontab.hpp"

namespace tiki
{
	EditorWindow::EditorWindow()
	{
		m_pFileTabs = new QTabWidget();
		m_pFileTabs->setTabsClosable( true );

		connect( m_pFileTabs, &QTabWidget::tabCloseRequested, this, &EditorWindow::fileCloseRequested );

		setCentralWidget( m_pFileTabs );

		//saveGeometry();
		//saveState();
	}

	EditorWindow::~EditorWindow()
	{
		delete m_pFileTabs;
	}

	void EditorWindow::openFileTab( QWidget* pWidget, const QString& title )
	{
		m_pFileTabs->addTab( pWidget, title );
	}

	void EditorWindow::changeFileTab( QWidget* pWidget, const QString& title )
	{
		int index = -1;
		for( int i = 0u; i < m_pFileTabs->count(); ++i )
		{
			if( m_pFileTabs->widget( i ) == pWidget )
			{
				index = i;
				break;
			}
		}

		if( index >= 0 )
		{
			m_pFileTabs->setTabText( index, title );
		}
	}

	void EditorWindow::closeFileTab( QWidget* pWidget )
	{
		int index = -1;
		for( int i = 0u; i < m_pFileTabs->count(); ++i )
		{
			if( m_pFileTabs->widget( i )  == pWidget )
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