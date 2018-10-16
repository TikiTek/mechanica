#include "editor_application.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/math/axisalignedrectangle.hpp"

#include <imgui.h>

namespace tiki
{
	EditorApplication::EditorApplication()
	{
		m_pVertexFormat = nullptr;
		m_pVertexInputBinding = nullptr;

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
		GraphicsSystem& graphicsSystem = getGraphicsSystem();
		{
			const VertexAttribute attributes[] =
			{
				{ VertexSementic_Position,	0u,	VertexAttributeFormat_x32y32_float,		0u, VertexInputType_PerVertex },
				{ VertexSementic_TexCoord,	0u,	VertexAttributeFormat_x32y32_float,		0u, VertexInputType_PerVertex },
				{ VertexSementic_Color,		0u,	VertexAttributeFormat_x8y8z8w8_unorm,	0u, VertexInputType_PerVertex }
			};

			m_pVertexFormat = graphicsSystem.createVertexFormat( attributes, TIKI_COUNT( attributes ) );
			if( m_pVertexFormat == nullptr )
			{
				shutdownTool();
				return false;
			}

			m_pVertexInputBinding = graphicsSystem.createVertexInputBinding( getImmediateRenderer().getShader()->getShader( ShaderType_VertexShader, 0u ), m_pVertexFormat );
			if( m_pVertexInputBinding == nullptr )
			{
				shutdownTool();
				return false;
			}
		}

		ImGui::CreateContext();

		// Build atlas
		ImGuiIO& io = ImGui::GetIO();

		{
			int fontTextureWidth;
			int fontTextureHeight;
			uint8* pFontTextureData = nullptr;
			io.Fonts->GetTexDataAsRGBA32( &pFontTextureData, &fontTextureWidth, &fontTextureHeight );

			TextureDescription fontTextureDescription;
			fontTextureDescription.width	= fontTextureWidth;
			fontTextureDescription.height	= fontTextureHeight;
			fontTextureDescription.type		= TextureType_2d;
			fontTextureDescription.format	= PixelFormat_R8G8B8A8;
			fontTextureDescription.flags	= TextureFlags_ShaderInput;

			if( !m_fontTexture.create( graphicsSystem, fontTextureDescription, pFontTextureData, "ImGuiFont" ) )
			{
				shutdownTool();
				return false;
			}

			io.Fonts->TexID = (ImTextureID)&m_fontTexture;
		}

		return m_editor.create();
	}

	void EditorApplication::shutdownTool()
	{
		GraphicsSystem& graphicsSystem = getGraphicsSystem();

		m_editor.dispose();

		m_fontTexture.dispose( graphicsSystem );
		m_indexBuffer.dispose( graphicsSystem );
		m_vertexBuffer.dispose( graphicsSystem );
		graphicsSystem.disposeVertexInputBinding( m_pVertexInputBinding );
		graphicsSystem.disposeVertexFormat( m_pVertexFormat );

		ImGui::DestroyContext();
	}

	void EditorApplication::updateTool( bool wantToShutdown )
	{
		const RenderTarget& backBuffer = getGraphicsSystem().getBackBuffer();

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize	= ImVec2( float( backBuffer.getWidth() ), float( backBuffer.getHeight() ) );
		io.DeltaTime	= float( getFrameTimer().getElapsedTime() );

		ImGui::NewFrame();
		doUi();

		ImGui::Render();
		ImDrawData* pDrawData = ImGui::GetDrawData();

		GraphicsSystem& graphicsSystem = getGraphicsSystem();
		if( m_indexBuffer.getCount() < pDrawData->TotalIdxCount )
		{
			m_indexBuffer.dispose( graphicsSystem );
			m_indexBuffer.create( graphicsSystem, getNextPowerOfTwo( pDrawData->TotalIdxCount ), (IndexType)sizeof( ImDrawIdx ), true, nullptr, "ImGuiIndex" );
		}

		if( m_vertexBuffer.getCount() < pDrawData->TotalVtxCount )
		{
			m_vertexBuffer.dispose( graphicsSystem );
			m_vertexBuffer.create( graphicsSystem, getNextPowerOfTwo( pDrawData->TotalVtxCount ), sizeof( ImDrawVert ), true, nullptr, "ImGuiVertex" );
		}
	}

	void EditorApplication::renderTool( GraphicsContext& graphicsContext ) const
	{
		ImDrawData* pDrawData = ImGui::GetDrawData();
		if( pDrawData->TotalIdxCount == 0u ||
			pDrawData->TotalVtxCount == 0u )
		{
			return;
		}

		ImDrawIdx* pIndices = graphicsContext.mapBuffer< ImDrawIdx >( m_indexBuffer );
		ImDrawVert* pVertices = graphicsContext.mapBuffer< ImDrawVert >( m_vertexBuffer );
		for( uint drawListIndex = 0u; drawListIndex < pDrawData->CmdListsCount; ++drawListIndex )
		{
			const ImDrawList* pDrawList = pDrawData->CmdLists[ drawListIndex ];
			memory::copy( pIndices, pDrawList->IdxBuffer.Data, sizeof( ImDrawIdx ) * pDrawList->IdxBuffer.Size );
			memory::copy( pVertices, pDrawList->VtxBuffer.Data, sizeof( ImDrawVert ) * pDrawList->VtxBuffer.Size );
			pIndices += pDrawList->IdxBuffer.Size;
			pVertices += pDrawList->VtxBuffer.Size;
		}
		graphicsContext.unmapBuffer( m_indexBuffer );
		graphicsContext.unmapBuffer( m_vertexBuffer );

		const ImmediateRenderer& renderer = getImmediateRenderer();
		renderer.beginRenderPass();

		renderer.drawTexturedRectangle( m_fontTexture, createAxisAlignedRectangle( 100, 100, 512, 64 ), TIKI_COLOR_GRAY );
		renderer.drawRectangle( createAxisAlignedRectangle( 100, 200, 100, 100 ), TIKI_COLOR_GRAY );

		graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleList );
		graphicsContext.setVertexInputBinding( m_pVertexInputBinding );
		graphicsContext.setIndexBuffer( m_indexBuffer );
		graphicsContext.setVertexBuffer( 0u, m_vertexBuffer );

		uint indexOffset = 0u;
		uint vertexOffset = 0u;
		for( uint drawListIndex = 0u; drawListIndex < pDrawData->CmdListsCount; ++drawListIndex )
		{
			const ImDrawList* pDrawList = pDrawData->CmdLists[ drawListIndex ];
			for( uint drawCommandIndex = 0u; drawCommandIndex < pDrawList->CmdBuffer.Size; ++drawCommandIndex )
			{
				const ImDrawCmd* pDrawCommand = &pDrawList->CmdBuffer[ (int)drawCommandIndex ];

				renderer.setShaderMode( pDrawCommand->TextureId != nullptr ? ImmediateShaderMode_Texture : ImmediateShaderMode_Color );
				graphicsContext.setPixelShaderTexture( 0u, (const TextureData*)pDrawCommand->TextureId );
				graphicsContext.setScissorRectangle( createAxisAlignedRectangleMinMax( pDrawCommand->ClipRect.x, pDrawCommand->ClipRect.y, pDrawCommand->ClipRect.z, pDrawCommand->ClipRect.w ) );
				graphicsContext.drawIndexedGeometry( pDrawCommand->ElemCount, indexOffset, vertexOffset );

				indexOffset += pDrawCommand->ElemCount;
			}

			vertexOffset += pDrawList->VtxBuffer.Size;
		}

		renderer.endRenderPass();
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