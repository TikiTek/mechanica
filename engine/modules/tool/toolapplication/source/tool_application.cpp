#include "tiki/toolapplication/tool_application.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/graphics/color_xkcd.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/immediaterenderer.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/input/inputevent.hpp"
#include "tiki/math/axisalignedrectangle.hpp"
#include "tiki/runtimeshared/windowevent.hpp"

#include <imgui.h>

namespace tiki
{
	ToolApplication::ToolApplication()
	{
		m_pVertexFormat = nullptr;
		m_pVertexInputBinding = nullptr;
	}

	ToolApplication::~ToolApplication()
	{
	}

	void ToolApplication::fillBaseParameters( BaseApplicationParamters& parameters )
	{
		fillToolParameters( m_parameters );
		parameters = m_parameters;
	}

	bool ToolApplication::initializeApplication()
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

		return initializeTool();
	}

	void ToolApplication::shutdownApplication()
	{
		shutdownTool();

		GraphicsSystem& graphicsSystem = getGraphicsSystem();
		m_fontTexture.dispose( graphicsSystem );
		m_indexBuffer.dispose( graphicsSystem );
		m_vertexBuffer.dispose( graphicsSystem );
		graphicsSystem.disposeVertexInputBinding( m_pVertexInputBinding );
		graphicsSystem.disposeVertexFormat( m_pVertexFormat );

		ImGui::DestroyContext();
	}

	void ToolApplication::updateApplication( bool wantToShutdown )
	{
		updateTool( wantToShutdown );

		ImGuiIO& io = ImGui::GetIO();

		GraphicsSystem& graphicsSystem = getGraphicsSystem();
		const RenderTarget& backBuffer = graphicsSystem.getBackBuffer();
		io.DisplaySize	= ImVec2( float( backBuffer.getWidth() ), float( backBuffer.getHeight() ) );
		io.DeltaTime	= float( getFrameTimer().getElapsedTime() );

		ImGui::NewFrame();
		doUi();
		ImGui::Render();

		ImDrawData* pDrawData = ImGui::GetDrawData();
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

		if( wantToShutdown )
		{
			m_running = false;
		}
	}

	void ToolApplication::renderApplication( GraphicsContext& graphicsContext ) const
	{
		renderTool( graphicsContext );

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

		graphicsContext.clear( graphicsContext.getBackBuffer(), TIKI_COLOR_XKCD_AZUL );
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

	bool ToolApplication::processBaseInputEvent( const InputEvent& inputEvent )
	{
		if( processToolInputEvent( inputEvent ) )
		{
			return true;
		}

		ImGuiIO& io = ImGui::GetIO();
		switch( inputEvent.eventType )
		{
		case InputEventType_Mouse_ButtonDown:
			io.MouseDown[ inputEvent.data.mouseButton.button ] = true;
			return true;

		case InputEventType_Mouse_ButtonUp:
			io.MouseDown[ inputEvent.data.mouseButton.button ] = false;
			return true;

		case InputEventType_Mouse_Wheel:
			io.MouseWheel = inputEvent.data.mouseWheel.offset;
			return true;

		case InputEventType_Mouse_Moved:
			io.MousePos.x = inputEvent.data.mouseMoved.xState;
			io.MousePos.y = inputEvent.data.mouseMoved.yState;
			return true;

		default:
			break;
		}

		return false;
	}

	void ToolApplication::processBaseWindowEvent( const WindowEvent& windowEvent )
	{
		switch( windowEvent.type )
		{
		case WindowEventType_SizeChanged:
			getGraphicsSystem().resize( windowEvent.data.sizeChanged.size.x, windowEvent.data.sizeChanged.size.y );
			break;

		default:
			break;
		}

		processToolWindowEvent( windowEvent );
	}
}
