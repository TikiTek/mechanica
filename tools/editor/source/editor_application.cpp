#include "editor_application.hpp"

#include "tiki/toolapplication/tool_ui.hpp"

#include "res_package_editor.hpp"

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
		parameters.screenWidth		= 1600u;
		parameters.screenHeight		= 900u;
	}

	bool EditorApplication::initializeTool()
	{
		m_browserDefaultIcon.createFromMemory( getPackageEditorResource( PackageEditorResources_BrowserFileUnknown ) );

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
		doRibbonUi();
		doBrowserUi();



		//static float f = 0.0f;
		//ImGui::Text( "Hello, world!" );
		//ImGui::SliderFloat( "float", &f, 0.0f, 1.0f );
		//ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate );

		ImGui::ShowStyleEditor();
	}

	void EditorApplication::doRibbonUi()
	{
		const ImGuiIO& io = ImGui::GetIO();

		ImGui::SetNextWindowPos( ImVec2( 5.0f, 5.0f ), ImGuiCond_Always, ImVec2() );
		ImGui::SetNextWindowSize( ImVec2( io.DisplaySize.x - 10.0f, 96.0f ), ImGuiCond_Always );
		if( ImGui::Begin( "Ribbon", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav ) )
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

			if( m_pCurrentRibbon == nullptr && !m_editor.getGlobalRibbons().isEmpty() )
			{
				m_pCurrentRibbon = m_editor.getGlobalRibbons().getFirst();
			}

			if( m_pCurrentRibbon != nullptr )
			{
				m_pCurrentRibbon->doUi();
			}

			ImGui::End();
		}
	}

	void EditorApplication::doBrowserUi()
	{
		const ImGuiIO& io = ImGui::GetIO();

		ImGui::SetNextWindowPos( ImVec2( 5.0f, 106.0f ), ImGuiCond_Always, ImVec2() );
		ImGui::SetNextWindowSize( ImVec2( 250.0f, io.DisplaySize.y - 111.0f ), ImGuiCond_Always );
		if( ImGui::Begin( "Browser", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav ) )
		{
			ImGui::Button( "test" );

			ImGui::PushID( 1 );
			ImGui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
			ImGui::TreeNodeEx( "##hidden1", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet );
			ImGui::SameLine();
			ImGui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
			ImGui::Image( ImGui::Tex( m_browserDefaultIcon ), ImVec2( m_browserDefaultIcon.getData().getWidth(), m_browserDefaultIcon.getData().getHeight() ) );
			ImGui::SameLine();
			ImGui::AlignTextToFramePadding();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
			ImGui::Text( "Objects1" );
			ImGui::PopID();

			ImGui::PushID( 2 );
			ImGui::AlignTextToFramePadding();
			if( ImGui::TreeNode( "Object2" ) )
			{
				ImGui::PushID( 21 );
				ImGui::AlignTextToFramePadding();
				ImGui::TreeNodeEx( "Object21", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet );
				ImGui::PopID();

				ImGui::PushID( 22 );
				ImGui::AlignTextToFramePadding();
				ImGui::TreeNodeEx( "Object22", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet );
				ImGui::PopID();

				ImGui::TreePop();
			}

			ImGui::PopID();

			ImGui::End();
		}
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