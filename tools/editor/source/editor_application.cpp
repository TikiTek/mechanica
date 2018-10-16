#include "editor_application.hpp"


#include <imgui.h>

namespace tiki
{
	EditorApplication::EditorApplication()
	{
		m_pCurrentRibbon = nullptr;

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
		parameters.pWindowTitle		= "TikiEditor";
		parameters.assetBuildPath	= m_project.getAssetBuildPath();
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

	void EditorApplication::doUi()
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGui::SetNextWindowPos( ImVec2( 10.f, 10.0f ), ImGuiCond_Always, ImVec2() );
		ImGui::SetNextWindowSize( ImVec2( 500.0f, 100.0f ), ImGuiCond_Always );
		ImGui::SetNextWindowBgAlpha( 0.3f );
		if( ImGui::Begin( "Example: Simple Overlay", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav ) )
		{
			bool found;
			for( EditorRibbon* pRibbon : m_editor.getGlobalRibbons() )
			{
				bool wasSelected = false;
				if( m_pCurrentRibbon == pRibbon )
				{
					const ImGuiStyle& style = ImGui::GetStyle();
					ImGui::PushStyleColor( ImGuiCol_Button, style.Colors[ ImGuiCol_ButtonHovered ] );
					wasSelected = true;
				}

				if( ImGui::Button( pRibbon->getTitle().cStr() ) )
				{
					m_pCurrentRibbon = pRibbon;
				}

				if( wasSelected )
				{
					ImGui::PopStyleColor();
				}

				ImGui::SameLine();

				found |= (m_pCurrentRibbon == pRibbon);
			}
			ImGui::NewLine();

			if( !found )
			{
				m_pCurrentRibbon = nullptr;
			}

			ImGui::Button( "test" );
			ImGui::SameLine();
			ImGui::Button( "test" );
			ImGui::SameLine();
			ImGui::Button( "test" );

			if( m_pCurrentRibbon != nullptr )
			{
				m_pCurrentRibbon->doUi();
			}

			ImGui::End();
		}

		static float f = 0.0f;
		ImGui::Text( "Hello, world!" );
		ImGui::SliderFloat( "float", &f, 0.0f, 1.0f );
		ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate );
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