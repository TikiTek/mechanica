
#include "tiki/renderer/postascii.hpp"

namespace tiki
{
	PostProcessAscii::PostProcessAscii()
	{
		m_pAsciiCharTexture	= nullptr;
		m_pShader			= nullptr;

		m_pBlendState		= nullptr;
		m_pDepthState		= nullptr;
		m_pRasterizerState	= nullptr;
		m_pSampler			= nullptr;
	}

	PostProcessAscii::~PostProcessAscii()
	{
	}

	bool PostProcessAscii::create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const PostProcessAsciiParameters& parameters )
	{
		m_pAsciiCharTexture	= resourceManager.loadResource< Texture >( "ascii.texture" );
		m_pShader			= resourceManager.loadResource< ShaderSet >( "ascii.shader" );

		bool success = ( m_pAsciiCharTexture != nullptr ) && ( m_pShader != nullptr );

		TextureDescription description;
		description.width	= parameters.resultDataWidth;
		description.height	= parameters.resultDataHeight;
		description.flags	= TextureFlags_RenderTarget | TextureFlags_ShaderInput;
		description.type	= TextureType_2d;
		description.format	= PixelFormat_Color;
		success &= m_finalResultData.create( graphicsSystem, description );

		description.width	= parameters.maxCharsWidth;
		description.height	= parameters.maxCharsHeight;
		success &= m_downSampleData.create( graphicsSystem, description );

		RenderTargetBuffer renderTargetBuffer = RenderTargetBuffer( m_finalResultData );
		success &= m_finalResultTarget.create( graphicsSystem, 0u, 0u, &renderTargetBuffer, 1u, nullptr );

		renderTargetBuffer = RenderTargetBuffer( m_downSampleData );
		success &= m_downSampleTarget.create( graphicsSystem, 0u, 0u, &renderTargetBuffer, 1u, nullptr );

		m_pBlendState		= graphicsSystem.createBlendState( BlendStateParamters() );
		m_pDepthState		= graphicsSystem.createDepthStencilState( false, false );
		m_pRasterizerState	= graphicsSystem.createRasterizerState( RasterizerStateParamters() );
		m_pSampler			= graphicsSystem.createSamplerState( SamplerStateParamters() );
		success &= ( m_pBlendState != nullptr ) && ( m_pDepthState != nullptr ) && ( m_pRasterizerState != nullptr ) && ( m_pSampler != nullptr );

		if ( !success )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		return true;
	}

	void PostProcessAscii::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		graphicsSystem.disposeSamplerState( m_pSampler );
		graphicsSystem.disposeRasterizerState( m_pRasterizerState );
		graphicsSystem.disposeDepthStencilState( m_pDepthState );
		graphicsSystem.disposeBlendState( m_pBlendState );

		m_downSampleTarget.dispose( graphicsSystem );
		m_finalResultTarget.dispose( graphicsSystem );

		m_downSampleData.dispose( graphicsSystem );
		m_finalResultData.dispose( graphicsSystem );

		resourceManager.unloadResource( m_pAsciiCharTexture );
		m_pAsciiCharTexture = nullptr;

		resourceManager.unloadResource( m_pShader );
		m_pShader = nullptr;
	}

	void PostProcessAscii::render( GraphicsContext& graphicsContext, const TextureData& inputData )
	{
		graphicsContext.beginRenderPass( m_downSampleTarget );

		graphicsContext.setBlendState( m_pBlendState );
		graphicsContext.setRasterizerState( m_pRasterizerState );
		graphicsContext.setDepthStencilState( m_pDepthState );

		graphicsContext.setPixelShaderTexture( 0u, &inputData );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSampler );

		graphicsContext.endRenderPass();
	}
}