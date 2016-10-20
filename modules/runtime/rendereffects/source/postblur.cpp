#include "tiki/rendereffects/postblur.hpp"

#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/graphics/viewport.hpp"
#include "tiki/resource/resourcemanager.hpp"

#include "shader/blur_shader.hpp"

namespace tiki
{
	PostProcessBlur::PostProcessBlur()
	{
		m_pShader			= nullptr;

		m_pBlendState		= nullptr;
		m_pDepthState		= nullptr;
		m_pRasterizerState	= nullptr;
		m_pSamplerState		= nullptr;

		m_pInputBinding		= nullptr;
	}

	PostProcessBlur::~PostProcessBlur()
	{
		TIKI_ASSERT( m_pShader			== nullptr );

		TIKI_ASSERT( m_pBlendState		== nullptr );
		TIKI_ASSERT( m_pDepthState		== nullptr );
		TIKI_ASSERT( m_pRasterizerState	== nullptr );
		TIKI_ASSERT( m_pSamplerState	== nullptr );

		TIKI_ASSERT( m_pInputBinding	== nullptr );
	}

	bool PostProcessBlur::create( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool, uint maxWidth, uint maxHeight, PixelFormat format /* = PixelFormat_Color */ )
	{
		bool success = true;

		m_format = format;
		
		resourcePool.beginLoadResource( &m_pShader, "blur.shader" );

		m_pBlendState		= graphicsSystem.createBlendState( false, Blend_One, Blend_Zero, BlendOperation_Add, ColorWriteMask_All );
		m_pDepthState		= graphicsSystem.createDepthStencilState( false, false );
		m_pRasterizerState	= graphicsSystem.createRasterizerState( FillMode_Solid, CullMode_Back, WindingOrder_Clockwise );
		m_pSamplerState		= graphicsSystem.createSamplerState( AddressMode_Clamp, AddressMode_Clamp, AddressMode_Clamp, FilterMode_Linear, FilterMode_Linear );
		success &= ( m_pBlendState != nullptr ) && ( m_pDepthState != nullptr ) && ( m_pRasterizerState != nullptr ) && ( m_pSamplerState != nullptr );

		success &= m_pixelConstants.create( graphicsSystem, sizeof( BlurPixelConstantData ), "BlurPixelConstants" );
		success &= createRenderTargets( graphicsSystem, maxWidth, maxHeight );

		if ( !success )
		{
			dispose( graphicsSystem, resourcePool );
			return false;
		}

		return true;
	}

	bool PostProcessBlur::createResources( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool )
	{
		if( m_pShader == nullptr )
		{
			dispose( graphicsSystem, resourcePool );
			return false;
		}

		m_pInputBinding = graphicsSystem.createVertexInputBinding( m_pShader->getShader( ShaderType_VertexShader, 0u ), graphicsSystem.getStockVertexFormat( StockVertexFormat_Pos2Tex2 ) );
		if( m_pInputBinding == nullptr )
		{
			dispose( graphicsSystem, resourcePool );
			return false;
		}

		return true;
	}

	void PostProcessBlur::dispose( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool )
	{
		disposeRenderTargets( graphicsSystem );

		m_pixelConstants.dispose( graphicsSystem );

		graphicsSystem.disposeSamplerState( m_pSamplerState );
		graphicsSystem.disposeRasterizerState( m_pRasterizerState );
		graphicsSystem.disposeDepthStencilState( m_pDepthState );
		graphicsSystem.disposeBlendState( m_pBlendState );
		m_pBlendState		= nullptr;
		m_pDepthState		= nullptr;
		m_pRasterizerState	= nullptr;
		m_pSamplerState		= nullptr;

		graphicsSystem.disposeVertexInputBinding( m_pInputBinding );
		m_pInputBinding = nullptr;

		resourcePool.unloadResource( m_pShader );
		m_pShader = nullptr;
	}

	bool PostProcessBlur::resize( GraphicsSystem& graphicsSystem, uint width, uint height )
	{
		disposeRenderTargets( graphicsSystem );
		return createRenderTargets( graphicsSystem, width, height );
	}

	void PostProcessBlur::render( GraphicsContext& graphicsContext, const TextureData& sourceData, const RenderTarget& target ) const
	{
		TIKI_ASSERT( target.getWidth() <= m_renderTarget.getWidth() );
		TIKI_ASSERT( target.getHeight() <= m_renderTarget.getHeight() );

		// 2 passes: horizontal and vertical
		const float2 aMaxTexCoord[ 2u ] =
		{
			{ 1.0f, 1.0f },
			{ (float)target.getWidth() / (float)m_textureData.getWidth(), (float)target.getHeight() / (float)m_textureData.getHeight() }
		};

		const float4 aPixelData[ 2u ] =
		{
			{ 1.0f / (float)target.getWidth(), 0.0f, 0.0f, 0.0f },
			{ 0.0f, aMaxTexCoord[ 1u ].y / (float)target.getHeight(), 0.0f, 0.0f }
		};

		const RenderTarget* apTarget[ 2u ] =
		{
			&m_renderTarget,
			&target
		};

		const TextureData* apInputData[ 2u ] =
		{
			&sourceData,
			&m_textureData
		};

		Viewport viewport;
		viewport.x			= 0.0f;
		viewport.y			= 0.0f;
		viewport.width		= (float)target.getWidth();
		viewport.height		= (float)target.getHeight();
		viewport.minDepth	= 0.0f;
		viewport.maxDepth	= 1.0f;

		graphicsContext.clear( m_renderTarget, TIKI_COLOR_TRANSPARENT );

		for (uint passIndex = 0u; passIndex < 2u; ++passIndex)
		{
			graphicsContext.beginRenderPass( *apTarget[ passIndex ], &viewport );

			BlurPixelConstantData* pPixelConstants = static_cast< BlurPixelConstantData* >( graphicsContext.mapBuffer( m_pixelConstants ) );
			TIKI_ASSERT( pPixelConstants != nullptr );
			pPixelConstants->param0 = aPixelData[ passIndex ];
			graphicsContext.unmapBuffer( m_pixelConstants );

			graphicsContext.setVertexShader( m_pShader->getShader( ShaderType_VertexShader, 0u ) );
			graphicsContext.setPixelShader( m_pShader->getShader( ShaderType_PixelShader, 0u ) );

			graphicsContext.setBlendState( m_pBlendState );
			graphicsContext.setDepthStencilState( m_pDepthState );
			graphicsContext.setRasterizerState( m_pRasterizerState );
			graphicsContext.setVertexInputBinding( m_pInputBinding );

			graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerState );
			graphicsContext.setPixelShaderTexture( 0u, apInputData[ passIndex ] );
			graphicsContext.setPixelShaderConstant( 0u, m_pixelConstants );

			graphicsContext.drawFullScreenQuadPos2Tex2( 0.0f, 0.0f, aMaxTexCoord[ passIndex ].x, aMaxTexCoord[ passIndex ].y );

			graphicsContext.endRenderPass();
		}
	}

	bool PostProcessBlur::createRenderTargets( GraphicsSystem& graphicsSystem, uint width, uint height )
	{
		TextureDescription description;
		description.type		= TextureType_2d;
		description.arrayCount	= 1u;
		description.depth		= 1u;
		description.format		= m_format;
		description.mipCount	= 1u;
		description.flags		= TextureFlags_ShaderInput | TextureFlags_RenderTarget;
		description.width		= uint16( width );
		description.height		= uint16( height );

		if ( !m_textureData.create( graphicsSystem, description ) )
		{
			return false;
		}

		RenderTargetBuffer colorBuffer( m_textureData );
		if ( !m_renderTarget.create( graphicsSystem, 0u, 0u, &colorBuffer, 1u, nullptr ) )
		{
			return false;
		}

		return true;
	}

	void PostProcessBlur::disposeRenderTargets( GraphicsSystem& graphicsSystem )
	{
		m_renderTarget.dispose( graphicsSystem );
		m_textureData.dispose( graphicsSystem );
	}
}