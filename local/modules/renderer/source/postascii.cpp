
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
		m_pSampler				= nullptr;

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
		m_pSampler			= graphicsSystem.createSamplerState( SamplerStateParamters() );
		success &= ( m_pBlendState != nullptr ) && ( m_pDepthState != nullptr ) && ( m_pRasterizerState != nullptr ) && ( m_pSampler != nullptr );

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

		graphicsContext.setVertexInputBinding( m_pVertexInputBinding );
		graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		graphicsContext.setVertexShader( m_pShader->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setPixelShader( m_pShader->getShader( ShaderType_PixelShader, 0u ) );

		StockVertexPos2Tex2* pVertices = static_cast< StockVertexPos2Tex2* >( graphicsContext.beginImmediateGeometry( sizeof( StockVertexPos2Tex2 ), 4u ) );
		createFloat2( pVertices[ 0u ].position, -1.0f, -1.0f );
		createFloat2( pVertices[ 0u ].texCoord, 0.0f, 1.0f );
		
		createFloat2( pVertices[ 1u ].position, -1.0f, 1.0f );
		createFloat2( pVertices[ 1u ].texCoord, 0.0f, 0.0f );
		
		createFloat2( pVertices[ 2u ].position, 1.0f, -1.0f );
		createFloat2( pVertices[ 2u ].texCoord, 1.0f, 1.0f );

		createFloat2( pVertices[ 3u ].position, 1.0f, 1.0f );
		createFloat2( pVertices[ 3u ].texCoord, 1.0f, 0.0f );
		graphicsContext.endImmediateGeometry();

		graphicsContext.endRenderPass();
	}
}