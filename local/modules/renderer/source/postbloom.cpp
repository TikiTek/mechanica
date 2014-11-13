
#include "tiki/renderer/postbloom.hpp"

#include "shader/bloom_shader.hpp"

namespace tiki
{
	PostProcessBloom::PostProcessBloom()
	{
		m_pShader			= nullptr;

		m_pBlendStateCuroff	= nullptr;
		m_pBlendStateAdd	= nullptr;
		m_pDepthState		= nullptr;
		m_pRasterizerState	= nullptr;
		m_pSamplerState		= nullptr;
	}

	PostProcessBloom::~PostProcessBloom()
	{
		TIKI_ASSERT( m_pShader				== nullptr );
		TIKI_ASSERT( m_pBlendStateCuroff	== nullptr );
		TIKI_ASSERT( m_pBlendStateAdd		== nullptr );
		TIKI_ASSERT( m_pDepthState			== nullptr );
		TIKI_ASSERT( m_pRasterizerState		== nullptr );
		TIKI_ASSERT( m_pSamplerState		== nullptr );
	}

	bool PostProcessBloom::create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const PostProcessBloomParameters& parameters )
	{
		bool success = true;

		m_width		= parameters.width;
		m_height	= parameters.height;
		m_passCount	= parameters.passCount;

		m_pShader = resourceManager.loadResource< ShaderSet >( "bloom.shader" );
		success &= ( m_pShader != nullptr );

		if ( m_pShader != nullptr )
		{
			m_pInputBinding = graphicsSystem.createVertexInputBinding( m_pShader->getShader( ShaderType_VertexShader, 0u ), graphicsSystem.getStockVertexFormat( StockVertexFormat_Pos2Tex2 ) );
			success &= ( m_pInputBinding != nullptr );
		}

		m_pBlendStateCuroff	= graphicsSystem.createBlendState( false, Blend_One, Blend_Zero, BlendOperation_Add, ColorWriteMask_All );
		m_pBlendStateAdd	= graphicsSystem.createBlendState( true, Blend_SourceAlpha, Blend_InverseSourceAlpha, BlendOperation_Add, ColorWriteMask_All );
		m_pDepthState		= graphicsSystem.createDepthStencilState( false, false );
		m_pRasterizerState	= graphicsSystem.createRasterizerState( FillMode_Solid, CullMode_Back, WindingOrder_Clockwise );
		m_pSamplerState		= graphicsSystem.createSamplerState( AddressMode_Clamp, AddressMode_Clamp, AddressMode_Clamp, FilterMode_Linear, FilterMode_Linear );
		success &= ( m_pBlendStateCuroff != nullptr ) && ( m_pBlendStateAdd != nullptr ) && ( m_pDepthState != nullptr ) && ( m_pRasterizerState != nullptr ) && ( m_pSamplerState != nullptr );

		success &= m_blur.create( graphicsSystem, resourceManager, m_width, m_height, PixelFormat_R16G16B16A16_Float );
		success &= createRenderTargets( graphicsSystem );

		if ( !success )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		return true;
	}

	void PostProcessBloom::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		disposeRenderTargets( graphicsSystem );
		m_blur.dispose( graphicsSystem, resourceManager );

		graphicsSystem.disposeSamplerState( m_pSamplerState );
		graphicsSystem.disposeRasterizerState( m_pRasterizerState );
		graphicsSystem.disposeDepthStencilState( m_pDepthState );
		graphicsSystem.disposeBlendState( m_pBlendStateAdd );
		graphicsSystem.disposeBlendState( m_pBlendStateCuroff );
		m_pBlendStateCuroff	= nullptr;
		m_pBlendStateAdd	= nullptr;
		m_pDepthState		= nullptr;
		m_pRasterizerState	= nullptr;
		m_pSamplerState		= nullptr;

		graphicsSystem.disposeVertexInputBinding( m_pInputBinding );
		m_pInputBinding = nullptr;

		resourceManager.unloadResource( m_pShader );
		m_pShader = nullptr;
	}

	bool PostProcessBloom::resize( GraphicsSystem& graphicsSystem, uint width, uint height, uint passCount )
	{
		disposeRenderTargets( graphicsSystem );

		m_width		= width;
		m_height	= height;
		m_passCount	= passCount;

		m_blur.resize( graphicsSystem, m_width, m_height );

		return createRenderTargets( graphicsSystem );
	}

	void PostProcessBloom::render( GraphicsContext& graphicsContext, const TextureData& accumulationData, const TextureData& selfilluData ) const
	{
		for (uint passIndex = 0u; passIndex < m_passCount; ++passIndex)
		{
			graphicsContext.beginRenderPass( m_renderTargets[ passIndex ] );

			const uint32 pixelShaderIndex = ( passIndex == 0u ? 1u : 0u );
			graphicsContext.setVertexShader( m_pShader->getShader( ShaderType_VertexShader, 0u ) );
			graphicsContext.setPixelShader( m_pShader->getShader( ShaderType_PixelShader, pixelShaderIndex ) );

			graphicsContext.setDepthStencilState( m_pDepthState );
			graphicsContext.setRasterizerState( m_pRasterizerState );
			graphicsContext.setVertexInputBinding( m_pInputBinding );
			graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerState );

			if ( passIndex == 0u )
			{
				graphicsContext.setBlendState( m_pBlendStateCuroff );
				graphicsContext.setPixelShaderTexture( 0u, &accumulationData );
				graphicsContext.setPixelShaderTexture( 1u, &selfilluData );
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
			graphicsContext.setPixelShader( m_pShader->getShader( ShaderType_PixelShader, 2u ) );

			if ( passIndex == 0u )
			{
				graphicsContext.setBlendState( m_pBlendStateCuroff );
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
		for (uint passIndex = 0u; passIndex < m_passCount; ++passIndex)
		{
			TIKI_ASSERT( passWidth > 0u );
			TIKI_ASSERT( passHeight > 0u );

			description.width	= passWidth;
			description.height	= passHeight;

			if ( !m_textures[ passIndex ].create( graphicsSystem, description ) )
			{
				return false;
			}

			RenderTargetBuffer colorBuffer( m_textures[ passIndex ] );
			if ( !m_renderTargets[ passIndex ].create( graphicsSystem, 0u, 0u, &colorBuffer, 1u, nullptr ) )
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
		for (uint i = 0u; i < m_renderTargets.getCount(); ++i)
		{
			m_renderTargets[ i ].dispose( graphicsSystem );
		}

		for (uint i = 0u; i < m_textures.getCount(); ++i)
		{
			m_textures[ i ].dispose( graphicsSystem );
		} 

		m_renderTargets.dispose();
		m_textures.dispose();
	}
}