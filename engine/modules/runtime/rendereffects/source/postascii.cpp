#include "tiki/rendereffects/postascii.hpp"

#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/graphics/stockvertex.hpp"
#include "tiki/resource/resourcemanager.hpp"

#include "shader/ascii_shader.hpp"

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
		m_pAsciiCharTexture	= resourceManager.loadResource< Font >( "ascii.font" );
		m_pShader			= resourceManager.loadResource< ShaderSet >( "ascii.shader" );

		bool success = ( m_pAsciiCharTexture != nullptr ) && ( m_pShader != nullptr );

		TextureDescription description;
		description.width	= uint16( parameters.resultDataWidth );
		description.height	= uint16( parameters.resultDataHeight );
		description.flags	= TextureFlags_RenderTarget | TextureFlags_ShaderInput;
		description.type	= TextureType_2d;
		description.format	= PixelFormat_Color;
		success &= m_finalResultData.create( graphicsSystem, description );

		description.width	= uint16( parameters.maxCharsWidth );
		description.height	= uint16( parameters.maxCharsHeight );
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

		success &= m_pixelConstants.create( graphicsSystem, sizeof( AsciiPixelConstantData ), "AsciiPixelConstants" );

		if ( !success )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		return true;
	}

	void PostProcessAscii::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		m_pixelConstants.dispose( graphicsSystem );

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

	void PostProcessAscii::render( GraphicsContext& graphicsContext, const Camera& camera, const TextureData* pOffscreenColorData, const TextureData* pOffscreenDepthData ) const
	{
		// down sample
		{
			Matrix44 inverseProjection;
			matrix::invert( inverseProjection, camera.getProjection().getMatrix() );

			AsciiPixelConstantData* pPixelConstants = static_cast< AsciiPixelConstantData* >( graphicsContext.mapBuffer( m_pixelConstants ) );
			TIKI_ASSERT( pPixelConstants != nullptr );
			createFloat4( pPixelConstants->param0, camera.getProjection().getFarPlane(), (float)pOffscreenColorData->getWidth(), (float)pOffscreenColorData->getHeight(), 0.0f );
			createGraphicsMatrix44( pPixelConstants->inverseProjection, inverseProjection );
			graphicsContext.unmapBuffer( m_pixelConstants );
		}

		graphicsContext.beginRenderPass( m_downSampleTarget );

		graphicsContext.setBlendState( m_pBlendState );
		graphicsContext.setRasterizerState( m_pRasterizerState );
		graphicsContext.setDepthStencilState( m_pDepthState );

		graphicsContext.setPixelShaderTexture( 0u, pOffscreenColorData );
		graphicsContext.setPixelShaderTexture( 1u, pOffscreenDepthData );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerLinear );
		graphicsContext.setPixelShaderSamplerState( 1u, m_pSamplerNearest );

		graphicsContext.setVertexInputBinding( m_pVertexInputBinding );
		graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		graphicsContext.setVertexShader( m_pShader->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setPixelShader( m_pShader->getShader( ShaderType_PixelShader, 1u ) );

		graphicsContext.setPixelShaderConstant( 0u, m_pixelConstants );

		graphicsContext.drawFullScreenQuadPos2Tex2();

		graphicsContext.endRenderPass();

		// ASCII
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