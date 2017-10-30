#include "editor_window.hpp"

#include "tiki/qtapplication/qtribbontab.hpp"

#include <QSettings>

namespace tiki
{
	EditorWindow::EditorWindow()
	{
		setMinimumSize( 300, 200 );

		m_pFileTabs = new QTabWidget();
		m_pFileTabs->setTabsClosable( true );

		connect( m_pFileTabs, &QTabWidget::tabCloseRequested, this, &EditorWindow::fileCloseRequested );

		setCentralWidget( m_pFileTabs );

		QSettings settings( "TikiTek", "editor" );
		restoreGeometry( settings.value( "window/geometry" ).toByteArray() );
		restoreState( settings.value( "window/windowState" ).toByteArray() );
	}

	EditorWindow::~EditorWindow()
	{
		QSettings settings( "TikiTek", "editor" );
		settings.setValue( "window/geometry", saveGeometry() );
		settings.setValue( "window/windowState", saveState() );

		delete m_pFileTabs;
	}

	void EditorWindow::openFileTab( QWidget* pWidget, const QString& title )
	{
		m_pFileTabs->setCurrentIndex( m_pFileTabs->addTab( pWidget, title ) );
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

	void EditorWindow::selectFileTab( QWidget* pWidget )
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
			m_pFileTabs->setCurrentIndex( index );
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