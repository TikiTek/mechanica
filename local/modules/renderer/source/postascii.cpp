
#include "tiki/renderer/postascii.hpp"

#include "tiki/graphics/stockvertex.hpp"

namespace tiki
{
	PostProcessAscii::PostProcessAscii()
	{
		m_pAsciiCharTexture		= nullptr;
		m_pShader				= nullptr;

		m_pBlendState			= nullptr;
		m_pDepthState			= nullptr;
		m_pRasterizerState		= nullptr;
		m_pSamplerLinear		= nullptr;
		m_pSamplerNearest		= nullptr;

		m_pVertexInputBinding	= nullptr;
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
		m_pSamplerLinear	= graphicsSystem.createSamplerState( AddressMode_Wrap, AddressMode_Wrap, AddressMode_Clamp, FilterMode_Linear, FilterMode_Linear );
		m_pSamplerNearest	= graphicsSystem.createSamplerState( AddressMode_Clamp, AddressMode_Clamp, AddressMode_Clamp, FilterMode_Nearest, FilterMode_Nearest );
		success &= ( m_pBlendState != nullptr ) && ( m_pDepthState != nullptr ) && ( m_pRasterizerState != nullptr ) && ( m_pSamplerLinear != nullptr ) && ( m_pSamplerNearest != nullptr );

		if ( m_pShader != nullptr )
		{
			m_pVertexInputBinding = graphicsSystem.createVertexInputBinding(
				m_pShader->getShader( ShaderType_VertexShader, 0u ),
				graphicsSystem.getStockVertexFormat( StockVertexFormat_Pos2Tex2 )
			);
			success &= ( m_pVertexInputBinding != nullptr );
		}

		if ( !success )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		return true;
	}

	void PostProcessAscii::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		graphicsSystem.disposeVertexInputBinding( m_pVertexInputBinding );
		m_pVertexInputBinding	= nullptr;

		graphicsSystem.disposeSamplerState( m_pSamplerLinear );
		graphicsSystem.disposeSamplerState( m_pSamplerNearest );
		graphicsSystem.disposeRasterizerState( m_pRasterizerState );
		graphicsSystem.disposeDepthStencilState( m_pDepthState );
		graphicsSystem.disposeBlendState( m_pBlendState );
		m_pBlendState			= nullptr;
		m_pDepthState			= nullptr;
		m_pRasterizerState		= nullptr;
		m_pSamplerLinear		= nullptr;
		m_pSamplerNearest		= nullptr;

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
		// downsample
		graphicsContext.beginRenderPass( m_downSampleTarget );

		graphicsContext.setBlendState( m_pBlendState );
		graphicsContext.setRasterizerState( m_pRasterizerState );
		graphicsContext.setDepthStencilState( m_pDepthState );

		graphicsContext.setPixelShaderTexture( 0u, &inputData );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerLinear );

		graphicsContext.setVertexInputBinding( m_pVertexInputBinding );
		graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		graphicsContext.setVertexShader( m_pShader->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setPixelShader( m_pShader->getShader( ShaderType_PixelShader, 1u ) );

		graphicsContext.drawFullScreenQuadPos2Tex2();

		graphicsContext.endRenderPass();

		// ascii
		graphicsContext.beginRenderPass( m_finalResultTarget );

		graphicsContext.setBlendState( m_pBlendState );
		graphicsContext.setRasterizerState( m_pRasterizerState );
		graphicsContext.setDepthStencilState( m_pDepthState );

		graphicsContext.setPixelShaderTexture( 0u, &m_downSampleData );
		graphicsContext.setPixelShaderTexture( 1u, &m_pAsciiCharTexture->getTextureData() );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerLinear );
		graphicsContext.setPixelShaderSamplerState( 1u, m_pSamplerNearest );

		graphicsContext.setVertexInputBinding( m_pVertexInputBinding );
		graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		graphicsContext.setVertexShader( m_pShader->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setPixelShader( m_pShader->getShader( ShaderType_PixelShader, 0u ) );

		graphicsContext.drawFullScreenQuadPos2Tex2();

		graphicsContext.endRenderPass();
	}
}