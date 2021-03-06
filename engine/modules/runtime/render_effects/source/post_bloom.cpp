#include "tiki/render_effects/post_bloom.hpp"

#include "tiki/graphics/graphics_system.hpp"
#include "tiki/graphics/shader_set.hpp"
#include "tiki/resource/resource_request_pool.hpp"

#include "shader/bloom_shader.hpp"

namespace tiki
{
	PostProcessBloom::PostProcessBloom()
	{
		m_pShader			= nullptr;

		m_pBlendStateCutoff	= nullptr;
		m_pBlendStateAdd	= nullptr;
		m_pDepthState		= nullptr;
		m_pRasterizerState	= nullptr;
		m_pSamplerState		= nullptr;
		m_pInputBinding		= nullptr;
	}

	PostProcessBloom::~PostProcessBloom()
	{
		TIKI_ASSERT( m_pShader				== nullptr );
		TIKI_ASSERT( m_pBlendStateCutoff	== nullptr );
		TIKI_ASSERT( m_pBlendStateAdd		== nullptr );
		TIKI_ASSERT( m_pDepthState			== nullptr );
		TIKI_ASSERT( m_pRasterizerState		== nullptr );
		TIKI_ASSERT( m_pSamplerState		== nullptr );
		TIKI_ASSERT( m_pInputBinding		== nullptr );
	}

	bool PostProcessBloom::create( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool, const PostProcessBloomParameters& parameters )
	{
		m_width		= parameters.width;
		m_height	= parameters.height;
		m_passCount	= parameters.passCount;

		m_pBlendStateCutoff	= graphicsSystem.createBlendState( false, Blend_One, Blend_Zero, BlendOperation_Add, ColorWriteMask_All );
		m_pBlendStateAdd	= graphicsSystem.createBlendState( true, Blend_SourceAlpha, Blend_InverseSourceAlpha, BlendOperation_Add, ColorWriteMask_All );
		m_pDepthState		= graphicsSystem.createDepthStencilState( false, false );
		m_pRasterizerState	= graphicsSystem.createRasterizerState( FillMode_Solid, CullMode_Back, WindingOrder_Clockwise );
		m_pSamplerState		= graphicsSystem.createSamplerState( AddressMode_Clamp, AddressMode_Clamp, AddressMode_Clamp, FilterMode_Linear, FilterMode_Linear );
		bool success = ( m_pBlendStateCutoff != nullptr ) && ( m_pBlendStateAdd != nullptr ) && ( m_pDepthState != nullptr ) && ( m_pRasterizerState != nullptr ) && ( m_pSamplerState != nullptr );

		success &= m_cutoffPixelConstants.create( graphicsSystem, sizeof( BloomCutoffPixelConstantData ), "BloomPixelConstants" );
		success &= m_blur.create( graphicsSystem, resourcePool, m_width, m_height, PixelFormat_R16G16B16A16_Float );
		success &= createRenderTargets( graphicsSystem );

		if ( !success )
		{
			dispose( graphicsSystem, resourcePool );
			return false;
		}

		resourcePool.beginLoadResource( &m_pShader, "bloom.shader" );

		return true;
	}

	bool PostProcessBloom::createResources( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool )
	{
		if( m_pShader == nullptr )
		{
			dispose( graphicsSystem, resourcePool );
			return false;
		}

		m_pInputBinding = graphicsSystem.createVertexInputBinding( m_pShader->getShader( ShaderType_VertexShader, 0u ), graphicsSystem.getStockVertexFormat( StockVertexFormat_Pos2Tex2 ) );
		if( !m_pInputBinding )
		{
			dispose( graphicsSystem, resourcePool );
			return false;
		}

		if( !m_blur.createResources( graphicsSystem, resourcePool ) )
		{
			dispose( graphicsSystem, resourcePool );
			return false;
		}

		return true;
	}

	void PostProcessBloom::dispose( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool )
	{
		disposeRenderTargets( graphicsSystem );
		m_blur.dispose( graphicsSystem, resourcePool );
		m_cutoffPixelConstants.dispose( graphicsSystem );

		graphicsSystem.disposeSamplerState( m_pSamplerState );
		graphicsSystem.disposeRasterizerState( m_pRasterizerState );
		graphicsSystem.disposeDepthStencilState( m_pDepthState );
		graphicsSystem.disposeBlendState( m_pBlendStateAdd );
		graphicsSystem.disposeBlendState( m_pBlendStateCutoff );

		graphicsSystem.disposeVertexInputBinding( m_pInputBinding );
		m_pInputBinding = nullptr;

		resourcePool.unloadResource( m_pShader );
		m_pShader = nullptr;
	}

	bool PostProcessBloom::resize( GraphicsSystem& graphicsSystem, uintreg width, uintreg height, uintreg passCount /*= TIKI_SIZE_T_MAX*/ )
	{
		if( m_width == width && m_height == height && (m_passCount == passCount ||passCount == TIKI_SIZE_T_MAX) )
		{
			return true;
		}

		disposeRenderTargets( graphicsSystem );

		m_width		= width;
		m_height	= height;
		if ( passCount != TIKI_SIZE_T_MAX )
		{
			m_passCount	= passCount;
		}

		m_blur.resize( graphicsSystem, m_width, m_height );

		return createRenderTargets( graphicsSystem );
	}

	void PostProcessBloom::render( GraphicsContext& graphicsContext, const PostProcessBloomRenderParameters& parameters ) const
	{
		{
			BloomCutoffPixelConstantData* pPixelConstants = graphicsContext.mapBuffer<BloomCutoffPixelConstantData>( m_cutoffPixelConstants );
			vector::toFloat( pPixelConstants->param0, parameters.cutoffThreshold, 0.0f );
			graphicsContext.unmapBuffer( m_cutoffPixelConstants );
		}

		for (uintreg passIndex = 0u; passIndex < m_passCount; ++passIndex)
		{
			graphicsContext.beginRenderPass( m_renderTargets[ passIndex ] );
			graphicsContext.clear( m_renderTargets[ passIndex ], TIKI_COLOR_TRANSPARENT );

			const uint32 pixelShaderIndex = ( passIndex == 0u ? (parameters.pEmissivData != nullptr ? 2u : 1u) : 0u );
			graphicsContext.setVertexShader( m_pShader->getShader( ShaderType_VertexShader, 0u ) );
			graphicsContext.setPixelShader( m_pShader->getShader( ShaderType_PixelShader, pixelShaderIndex ) );

			graphicsContext.setDepthStencilState( m_pDepthState );
			graphicsContext.setRasterizerState( m_pRasterizerState );
			graphicsContext.setVertexInputBinding( m_pInputBinding );
			graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerState );

			if ( passIndex == 0u )
			{
				graphicsContext.setBlendState( m_pBlendStateCutoff );
				graphicsContext.setPixelShaderConstant( 0u, m_cutoffPixelConstants );
				graphicsContext.setPixelShaderTexture( 0u, parameters.pSourceData );
				graphicsContext.setPixelShaderTexture( 1u, parameters.pEmissivData );
			}
			else
			{
				graphicsContext.setBlendState( m_pBlendStateAdd );
				graphicsContext.setPixelShaderTexture( 0u, &m_textures[ passIndex - 1u ] );
			}

			graphicsContext.drawFullScreenQuadPos2Tex2();
			graphicsContext.endRenderPass();

			m_blur.render( graphicsContext, m_textures[ passIndex ], m_renderTargets[ passIndex ] );
		}

		for (sint passIndex = m_passCount - 2u; passIndex >= 0; --passIndex)
		{
			graphicsContext.beginRenderPass( m_renderTargets[ passIndex ] );

			graphicsContext.setVertexShader( m_pShader->getShader( ShaderType_VertexShader, 0u ) );
			graphicsContext.setPixelShader( m_pShader->getShader( ShaderType_PixelShader, 0u ) );

			if ( passIndex == 0u )
			{
				graphicsContext.setBlendState( m_pBlendStateCutoff );
			}
			else
			{
				graphicsContext.setBlendState( m_pBlendStateAdd );
			}
			graphicsContext.setDepthStencilState( m_pDepthState );
			graphicsContext.setRasterizerState( m_pRasterizerState );
			graphicsContext.setVertexInputBinding( m_pInputBinding );

			graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerState );
			graphicsContext.setPixelShaderTexture( 0u, &m_textures[ passIndex + 1u ] );

			graphicsContext.drawFullScreenQuadPos2Tex2();

			graphicsContext.endRenderPass();
		}
	}

	const TextureData& PostProcessBloom::getResultData() const
	{
		return m_textures[ 0u ];
	}

	bool PostProcessBloom::createRenderTargets( GraphicsSystem& graphicsSystem )
	{
		m_textures.create( m_passCount );
		m_renderTargets.create( m_passCount );

		TextureDescription description;
		description.type		= TextureType_2d;
		description.format		= PixelFormat_R16G16B16A16_Float;
		description.flags		= TextureFlags_ShaderInput | TextureFlags_RenderTarget;

		uint16 passWidth	= uint16( m_width );
		uint16 passHeight	= uint16( m_height );
		for (uintreg passIndex = 0u; passIndex < m_passCount; ++passIndex)
		{
			TIKI_ASSERT( passWidth > 0u );
			TIKI_ASSERT( passHeight > 0u );

			description.width	= passWidth;
			description.height	= passHeight;

			if ( !m_textures[ passIndex ].create( graphicsSystem, description ) )
			{
				return false;
			}

			const TextureData* pColorBuffer = &m_textures[ passIndex ];
			if ( !m_renderTargets[ passIndex ].create( graphicsSystem, 0u, 0u, &pColorBuffer, 1u, nullptr ) )
			{
				return false;
			}

			passWidth	/= 2u;
			passHeight	/= 2u;
		}

		return true;
	}

	void PostProcessBloom::disposeRenderTargets( GraphicsSystem& graphicsSystem )
	{
		for (uintreg i = 0u; i < m_renderTargets.getCount(); ++i)
		{
			m_renderTargets[ i ].dispose( graphicsSystem );
		}

		for (uintreg i = 0u; i < m_textures.getCount(); ++i)
		{
			m_textures[ i ].dispose( graphicsSystem );
		}

		m_renderTargets.dispose();
		m_textures.dispose();
	}
}