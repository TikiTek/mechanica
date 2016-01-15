#include "tiki/ui/uirenderer.hpp"

#include "tiki/container/fixedarray.hpp"
#include "tiki/graphics/color.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/resource/resourcemanager.hpp"

#include "uitypes_private.hpp"

namespace tiki
{
	UiRenderer::UiRenderer()
	{
		m_pShader				= nullptr;
		m_pVertexFormat			= nullptr;
		m_pVertexInputBinding	= nullptr;
		m_pBlendState			= nullptr;
		m_pDepthStencilState	= nullptr;
		m_pRasterizerState		= nullptr;
		m_pSamplerState			= nullptr;
	}

	UiRenderer::~UiRenderer()
	{
		TIKI_ASSERT( m_pShader				== nullptr );
		TIKI_ASSERT( m_pVertexFormat		== nullptr );
		TIKI_ASSERT( m_pVertexInputBinding	== nullptr );
		TIKI_ASSERT( m_pBlendState			== nullptr );
		TIKI_ASSERT( m_pDepthStencilState	== nullptr );
		TIKI_ASSERT( m_pRasterizerState		== nullptr );
		TIKI_ASSERT( m_pSamplerState		== nullptr );
	}

	bool UiRenderer::create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const UiRendererParameters& parameters )
	{
		m_pShader = resourceManager.loadResource< ShaderSet >( "ui.shader" );
		if( m_pShader == nullptr )
		{
			return false;
		}

		if( !m_renderElements.create( parameters.maxRenderElements ) )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		{
			const VertexAttribute attributes[] =
			{
				{ VertexSementic_Position,	0u,	VertexAttributeFormat_x32y32_float,		0u, VertexInputType_PerVertex },
				{ VertexSementic_TexCoord,	0u,	VertexAttributeFormat_x32y32_float,		0u, VertexInputType_PerVertex },
				{ VertexSementic_Color,		0u,	VertexAttributeFormat_x8y8z8w8_unorm,	0u, VertexInputType_PerVertex }
			};

			m_pVertexFormat			= graphicsSystem.createVertexFormat( attributes, TIKI_COUNT( attributes ) );
			m_pVertexInputBinding	= graphicsSystem.createVertexInputBinding( m_pShader->getShader( ShaderType_VertexShader, 0u ), m_pVertexFormat );
		}

		m_pBlendState				= graphicsSystem.createBlendState( true, Blend_SourceAlpha, Blend_InverseSourceAlpha, BlendOperation_Add, ColorWriteMask_All );
		m_pDepthStencilState		= graphicsSystem.createDepthStencilState( false, false );
		m_pRasterizerState			= graphicsSystem.createRasterizerState( FillMode_Solid, CullMode_Back, WindingOrder_Clockwise );
		m_pSamplerState				= graphicsSystem.createSamplerState();

		if( m_pVertexFormat == nullptr || m_pVertexInputBinding == nullptr || m_pBlendState == nullptr || m_pDepthStencilState == nullptr || m_pRasterizerState == nullptr || m_pSamplerState == nullptr )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		return true;
	}

	void UiRenderer::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		graphicsSystem.disposeVertexFormat( m_pVertexFormat );
		graphicsSystem.disposeVertexInputBinding( m_pVertexInputBinding );

		graphicsSystem.disposeBlendState( m_pBlendState );
		graphicsSystem.disposeDepthStencilState( m_pDepthStencilState );
		graphicsSystem.disposeRasterizerState( m_pRasterizerState );
		graphicsSystem.disposeSamplerState( m_pSamplerState );

		m_renderElements.dispose();

		resourceManager.unloadResource( m_pShader );
	}

	void UiRenderer::update( const UiRenderData& renderData )
	{
		m_renderElements.clear();

		for( const UiElement& element : renderData.elements )
		{
			updateRecursiveRenderTree( element );
		}
	}

	void UiRenderer::render( GraphicsContext& context, const RenderTarget& renderTarget ) const
	{
		context.beginRenderPass( renderTarget );

		context.setBlendState( m_pBlendState );
		context.setDepthStencilState( m_pDepthStencilState );
		context.setRasterizerState( m_pRasterizerState );

		context.setPixelShader( m_pShader->getShader( ShaderType_PixelShader, 0u ) );
		context.setVertexShader( m_pShader->getShader( ShaderType_VertexShader, 0u ) );

		context.setVertexInputBinding( m_pVertexInputBinding );

		for( uint i = 0u; i < m_renderElements.getCount(); ++i )
		{
			StaticArray< UiVertex > vertices;
			context.beginImmediateGeometry( vertices, 4u );

			vertices[ 0u ].position		= createFloat2(  )

			context.endImmediateGeometry();
		}

		context.endRenderPass();
	}

	void UiRenderer::updateRecursiveRenderTree( const UiElement& element )
	{
		UiRenderElement& renderElement = m_renderElements.push();

		renderElement.position	= element.m_position;
		renderElement.rotation	= 0.0f;
		renderElement.size		= element.m_layoutSize;

		if( element.m_pFont != nullptr )
		{
			renderElement.type				= UiRenderElementType_Text;
			renderElement.data.text.pFont	= element.m_pFont;
			renderElement.data.text.pText	= element.m_pText;
		}
		else if( element.m_pTextureData != nullptr )
		{
			renderElement.type									= UiRenderElementType_TextureRectangle;
			renderElement.data.textureRectangle.pTextureData	= element.m_pTextureData;
			renderElement.data.textureRectangle.texCoords.create( element.m_texCoords.getBegin(), element.m_texCoords.getCount() );
			renderElement.data.textureRectangle.colors.create( element.m_colors.getBegin(), element.m_colors.getCount() );
		}
		else
		{
			renderElement.type	= UiRenderElementType_ColorRectangle;
			renderElement.data.coloredRectangle.colors.create( element.m_colors.getBegin(), element.m_colors.getCount() );
		}

		for( const UiElement& child : element.m_children )
		{
			updateRecursiveRenderTree( child );
		}
	}
}
