#include "tiki/tool_application/tool_application.hpp"

#include "tiki/base/timer.hpp"
#include "tiki/graphics/color_xkcd.hpp"
#include "tiki/graphics/graphics_system.hpp"
#include "tiki/graphics/immediate_renderer.hpp"
#include "tiki/graphics/shader_set.hpp"
#include "tiki/input/input_event.hpp"
#include "tiki/math/axis_aligned_rectangle.hpp"
#include "tiki/runtime_shared/window_event.hpp"

#include <imgui.h>

namespace tiki
{
	static const int s_aMouseMapping[] =
	{
		0,
		2,
		1,
		3,
		4
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aMouseMapping ) == MouseButton_Count );

	static const int s_aKeyboardMapping[] =
	{
		KeyboardKey_Tab,			// ImGuiKey_Tab
		KeyboardKey_Left,			// ImGuiKey_LeftArrow
		KeyboardKey_Right,			// ImGuiKey_RightArrow
		KeyboardKey_Up,				// ImGuiKey_UpArrow
		KeyboardKey_Down,			// ImGuiKey_DownArrow
		KeyboardKey_PageUp,			// ImGuiKey_PageUp
		KeyboardKey_PageDown,		// ImGuiKey_PageDown
		KeyboardKey_Home,			// ImGuiKey_Home
		KeyboardKey_End,			// ImGuiKey_End
		KeyboardKey_Insert,			// ImGuiKey_Insert
		KeyboardKey_Delete,			// ImGuiKey_Delete
		KeyboardKey_BackSpace,		// ImGuiKey_Backspace
		KeyboardKey_Space,			// ImGuiKey_Space
		KeyboardKey_Return,			// ImGuiKey_Enter
		KeyboardKey_Escape,			// ImGuiKey_Escape
		KeyboardKey_NumpadEnter,	// ImGuiKey_KeyPadEnter
		KeyboardKey_A,				// ImGuiKey_A
		KeyboardKey_C,				// ImGuiKey_C
		KeyboardKey_V,				// ImGuiKey_V
		KeyboardKey_X,				// ImGuiKey_X
		KeyboardKey_Y,				// ImGuiKey_Y
		KeyboardKey_Z				// ImGuiKey_Z
	};
	TIKI_COMPILETIME_ASSERT( TIKI_COUNT( s_aKeyboardMapping ) == ImGuiKey_COUNT );

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

			m_pVertexInputBinding = graphicsSystem.createVertexInputBinding( getImmediateRenderer().getShaderSet()->getShader( ShaderType_VertexShader, 0u ), m_pVertexFormat );
			if( m_pVertexInputBinding == nullptr )
			{
				shutdownTool();
				return false;
			}
		}

		ImGui::CreateContext();

		// Build atlas
		ImGuiIO& io = ImGui::GetIO();

		for( uint i = 0u; i < TIKI_COUNT( s_aKeyboardMapping ); ++i )
		{
			io.KeyMap[ i ] = s_aKeyboardMapping[ i ];
		}

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

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding		= 3.0f;
		style.WindowPadding			= ImVec2( 5.0f, 5.0f );
		style.FrameRounding			= 2.0f;
		style.PopupRounding			= 3.0f;
		style.FramePadding			= ImVec2( 7.0f, 5.0f );
		style.ItemSpacing			= ImVec2( 4.0f, 4.0f );
		style.ScrollbarRounding		= 2.0f;

		style.Colors[ ImGuiCol_WindowBg ]				= ImColor( color::setChannelA( TIKI_COLOR_XKCD_LIGHT_NAVY, 0x77u ) );
		style.Colors[ ImGuiCol_CheckMark ]				= ImColor( TIKI_COLOR_XKCD_LIGHTISH_BLUE );
		style.Colors[ ImGuiCol_SliderGrab ]				= ImColor( TIKI_COLOR_XKCD_LIGHTISH_BLUE );
		style.Colors[ ImGuiCol_SliderGrabActive ]		= ImColor( TIKI_COLOR_XKCD_LIGHTISH_BLUE );

		style.Colors[ ImGuiCol_FrameBg ]				= ImColor( color::setChannelA( TIKI_COLOR_XKCD_LIGHT_NAVY, 0x77u ) );
		style.Colors[ ImGuiCol_FrameBgHovered ]			= ImColor( TIKI_COLOR_XKCD_LIGHT_NAVY );
		style.Colors[ ImGuiCol_FrameBgActive ]			= ImColor( TIKI_COLOR_XKCD_LIGHT_NAVY_BLUE );

		style.Colors[ ImGuiCol_ScrollbarBg ]			= ImColor( color::setChannelA( TIKI_COLOR_XKCD_LIGHT_NAVY, 0x20u ) );
		style.Colors[ ImGuiCol_ScrollbarGrab ]			= ImColor( color::setChannelA( TIKI_COLOR_XKCD_LIGHT_NAVY, 0x77u ) );
		style.Colors[ ImGuiCol_ScrollbarGrabHovered ]	= ImColor( TIKI_COLOR_XKCD_LIGHT_NAVY );
		style.Colors[ ImGuiCol_ScrollbarGrabActive ]	= ImColor( TIKI_COLOR_XKCD_LIGHTISH_BLUE );

		style.Colors[ ImGuiCol_Button ]					= ImColor( color::setChannelA( TIKI_COLOR_XKCD_LIGHT_NAVY, 0x77u ) );
		style.Colors[ ImGuiCol_ButtonHovered ]			= ImColor( TIKI_COLOR_XKCD_LIGHT_NAVY );
		style.Colors[ ImGuiCol_ButtonActive ]			= ImColor( TIKI_COLOR_XKCD_LIGHTISH_BLUE );

		style.Colors[ ImGuiCol_TitleBg ]				= ImColor( color::setChannelA( TIKI_COLOR_XKCD_LIGHT_NAVY, 0x77u ) );
		style.Colors[ ImGuiCol_TitleBgActive ]			= ImColor( TIKI_COLOR_XKCD_LIGHT_NAVY );
		style.Colors[ ImGuiCol_TitleBgCollapsed ]		= ImColor( color::setChannelA( TIKI_COLOR_XKCD_LIGHT_NAVY, 0x77u ) );

		style.Colors[ ImGuiCol_ResizeGrip ]				= ImColor( color::setChannelA( TIKI_COLOR_XKCD_LIGHT_NAVY, 0x77u ) );
		style.Colors[ ImGuiCol_ResizeGripHovered ]		= ImColor( TIKI_COLOR_XKCD_LIGHT_NAVY );
		style.Colors[ ImGuiCol_ResizeGripActive ]		= ImColor( TIKI_COLOR_XKCD_LIGHTISH_BLUE );

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
		if( m_indexBuffer.getCount() < uint( pDrawData->TotalIdxCount ) )
		{
			m_indexBuffer.dispose( graphicsSystem );
			m_indexBuffer.create( graphicsSystem, getNextPowerOfTwo( pDrawData->TotalIdxCount ), (IndexType)sizeof( ImDrawIdx ), true, nullptr, "ImGuiIndex" );
		}

		if( m_vertexBuffer.getCount() < uint( pDrawData->TotalVtxCount ) )
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
		for( uint drawListIndex = 0u; drawListIndex < uint( pDrawData->CmdListsCount ); ++drawListIndex )
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

		graphicsContext.clear( graphicsContext.getBackBuffer(), m_parameters.backgroundColor );
		graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleList );
		graphicsContext.setVertexInputBinding( m_pVertexInputBinding );
		graphicsContext.setIndexBuffer( m_indexBuffer );
		graphicsContext.setVertexBuffer( 0u, m_vertexBuffer );

		uint indexOffset = 0u;
		uint vertexOffset = 0u;
		for( uint drawListIndex = 0u; drawListIndex < uint( pDrawData->CmdListsCount ); ++drawListIndex )
		{
			const ImDrawList* pDrawList = pDrawData->CmdLists[ drawListIndex ];
			for( uint drawCommandIndex = 0u; drawCommandIndex < uint( pDrawList->CmdBuffer.Size ); ++drawCommandIndex )
			{
				const ImDrawCmd* pDrawCommand = &pDrawList->CmdBuffer[ (int)drawCommandIndex ];

				renderer.setShaderMode( pDrawCommand->TextureId != nullptr ? ImmediateShaderMode_Texture : ImmediateShaderMode_Color );
				graphicsContext.setPixelShaderTexture( 0u, (const TextureData*)pDrawCommand->TextureId );
				graphicsContext.setScissorRectangle( AxisAlignedRectangle::createMinMax( pDrawCommand->ClipRect.x, pDrawCommand->ClipRect.y, pDrawCommand->ClipRect.z, pDrawCommand->ClipRect.w ) );
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
		case InputEventType_Mouse_ButtonUp:
		case InputEventType_Mouse_ButtonDown:
			{
				const bool down = (inputEvent.eventType == InputEventType_Mouse_ButtonDown);
				const int buttonIndex = s_aMouseMapping[ inputEvent.data.mouseButton.button ];
				io.MouseDown[ buttonIndex ] = down;
			}
			return true;
			return true;

		case InputEventType_Mouse_Wheel:
			io.MouseWheel = float( inputEvent.data.mouseWheel.offset ) / InputMouseWheelDelta;
			return true;

		case InputEventType_Mouse_Moved:
			io.MousePos.x = inputEvent.data.mouseMoved.xState;
			io.MousePos.y = inputEvent.data.mouseMoved.yState;
			return true;

		case InputEventType_Keyboard_Up:
		case InputEventType_Keyboard_Down:
			{
				const bool down = (inputEvent.eventType == InputEventType_Keyboard_Down);
				if( inputEvent.data.keyboardKey.key == KeyboardKey_LeftControl ||
					inputEvent.data.keyboardKey.key == KeyboardKey_RightControl )
				{
					io.KeyCtrl = down;
				}
				else if( inputEvent.data.keyboardKey.key == KeyboardKey_LeftShift ||
						 inputEvent.data.keyboardKey.key == KeyboardKey_RightShift )
				{
					io.KeyShift = down;
				}
				else if( inputEvent.data.keyboardKey.key == KeyboardKey_LeftAlt ||
						 inputEvent.data.keyboardKey.key == KeyboardKey_RightAlt )
				{
					io.KeyAlt = down;
				}

				io.KeysDown[ inputEvent.data.keyboardKey.key ] = down;
			}
			return true;

		case InputEventType_Keyboard_Character:
			io.AddInputCharacter( inputEvent.data.keyboardCharacter.rune );
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
