#include "editor_application.hpp"

namespace tiki
{
	EditorApplication::EditorApplication()
	{
		//setMinimumSize( 300, 200 );

		//m_pFileTabs = new QTabWidget();
		//m_pFileTabs->setTabsClosable( true );

		//connect( m_pFileTabs, &QTabWidget::tabCloseRequested, this, &EditorApplication::fileCloseRequested );

		//setCentralWidget( m_pFileTabs );

		//QSettings settings( "TikiTek", "editor" );
		//restoreGeometry( settings.value( "window/geometry" ).toByteArray() );
		//restoreState( settings.value( "window/windowState" ).toByteArray() );
	}

	EditorApplication::~EditorApplication()
	{
		//QSettings settings( "TikiTek", "editor" );
		//settings.setValue( "window/geometry", saveGeometry() );
		//settings.setValue( "window/windowState", saveState() );

		//delete m_pFileTabs;
	}

	void EditorApplication::fillToolParameters( ToolApplicationParamters& parameters )
	{
		parameters.pWindowTitle = "TikiEditor";
	}

	bool EditorApplication::initializeTool()
	{
		return m_editor.create();
	}

	void EditorApplication::shutdownTool()
	{
		m_editor.dispose();
	}

	void EditorApplication::updateTool( bool wantToShutdown )
	{

	}

	void EditorApplication::renderTool( GraphicsContext& graphicsContext ) const
	{

	}

	bool EditorApplication::processToolInputEvent( const InputEvent& inputEvent )
	{
		return false;
	}

	void EditorApplication::processToolWindowEvent( const WindowEvent& windowEvent )
	{

	}

	//void EditorApplication::openFileTab( QWidget* pWidget, const QString& title )
	//{
	//	m_pFileTabs->setCurrentIndex( m_pFileTabs->addTab( pWidget, title ) );
	//}

	//void EditorApplication::changeFileTab( QWidget* pWidget, const QString& title )
	//{
	//	int index = -1;
	//	for( int i = 0u; i < m_pFileTabs->count(); ++i )
	//	{
	//		if( m_pFileTabs->widget( i ) == pWidget )
	//		{
	//			index = i;
	//			break;
	//		}
	//	}

	//	if( index >= 0 )
	//	{
	//		m_pFileTabs->setTabText( index, title );
	//	}
	//}

	//void EditorApplication::selectFileTab( QWidget* pWidget )
	//{
	//	int index = -1;
	//	for( int i = 0u; i < m_pFileTabs->count(); ++i )
	//	{
	//		if( m_pFileTabs->widget( i ) == pWidget )
	//		{
	//			index = i;
	//			break;
	//		}
	//	}

	//	if( index >= 0 )
	//	{
	//		m_pFileTabs->setCurrentIndex( index );
	//	}
	//}

	//void EditorApplication::closeFileTab( QWidget* pWidget )
	//{
	//	int index = -1;
	//	for( int i = 0u; i < m_pFileTabs->count(); ++i )
	//	{
	//		if( m_pFileTabs->widget( i )  == pWidget )
	//		{
	//			index = i;
	//			break;
	//		}
	//	}

	//	if( index >= 0 )
	//	{
	//		m_pFileTabs->removeTab( index );
	//	}
	//}

	//void EditorApplication::fileCloseRequested( int index )
	//{
	//	emit fileCloseRequest( m_pFileTabs->widget( index ) );
	//}

	ToolApplication& tool::getTool()
	{
		static EditorApplication s_application;
		return s_application;
	}
}