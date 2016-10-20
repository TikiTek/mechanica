#include "tiki/rendereffects/postskybox.hpp"

#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/resource/resourcemanager.hpp"

#include "shader/skybox_shader.hpp"

namespace tiki
{
	PostProcessSkybox::PostProcessSkybox()
	{
		m_pShader			= nullptr;
		m_pInputBinding		= nullptr;

		m_pBlendState		= nullptr;
		m_pDepthState		= nullptr;
		m_pRasterizerState	= nullptr;
		m_pSamplerState		= nullptr;
	}

	PostProcessSkybox::~PostProcessSkybox()
	{
		TIKI_ASSERT( m_pShader				== nullptr );
		TIKI_ASSERT( m_pInputBinding		== nullptr );

		TIKI_ASSERT( m_pBlendState			== nullptr );
		TIKI_ASSERT( m_pDepthState			== nullptr );
		TIKI_ASSERT( m_pRasterizerState		== nullptr );
		TIKI_ASSERT( m_pSamplerState		== nullptr );
	}

	bool PostProcessSkybox::create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		m_pShader = resourceManager.loadResource< ShaderSet >( "skybox.shader" );
		m_pTexture = resourceManager.loadResource< Texture >( "skybox_sea.texture" );
		if ( m_pShader == nullptr || m_pTexture == nullptr )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}
		
		m_pInputBinding = graphicsSystem.createVertexInputBinding( m_pShader->getShader( ShaderType_VertexShader, 0u ), graphicsSystem.getStockVertexFormat( StockVertexFormat_Pos3 ) );
		if ( m_pInputBinding == nullptr )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		m_pBlendState		= graphicsSystem.createBlendState( false );
		m_pDepthState		= graphicsSystem.createDepthStencilState( false, false );
		m_pRasterizerState	= graphicsSystem.createRasterizerState();
		m_pSamplerState		= graphicsSystem.createSamplerState();
		if ( m_pBlendState == nullptr || m_pDepthState == nullptr || m_pRasterizerState == nullptr || m_pSamplerState == nullptr )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		if( !m_vertexConstants.create( graphicsSystem, sizeof( SkyboxVertexConstantData ), "SkyBoxVertexConstants" ) ||
			!m_pixelConstants.create( graphicsSystem, sizeof( SkyboxPixelConstantData ), "SkyBoxPixelConstants" ) )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}
				
		return true;
	}

	void PostProcessSkybox::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		m_vertexConstants.dispose( graphicsSystem );
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

		resourceManager.unloadResource( m_pShader );
		resourceManager.unloadResource( m_pTexture );
		m_pShader = nullptr;
		m_pTexture = nullptr;
	}

	void PostProcessSkybox::render( GraphicsContext& graphicsContext, const RenderTarget& renderTarget )
	{
		graphicsContext.beginRenderPass( renderTarget );

		//BlurPixelConstantData* pPixelConstants = static_cast< BlurPixelConstantData* >( graphicsContext.mapBuffer( m_pixelConstants ) );
		//TIKI_ASSERT( pPixelConstants != nullptr );
		//pPixelConstants->param0 = aPixelData[ passIndex ];
		//graphicsContext.unmapBuffer( m_pixelConstants );

		graphicsContext.setVertexShader( m_pShader->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setPixelShader( m_pShader->getShader( ShaderType_PixelShader, 0u ) );

		graphicsContext.setBlendState( m_pBlendState );
		graphicsContext.setDepthStencilState( m_pDepthState );
		graphicsContext.setRasterizerState( m_pRasterizerState );
		graphicsContext.setVertexInputBinding( m_pInputBinding );

		graphicsContext.setVertexShaderConstant( 0u, m_vertexConstants );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerState );
		graphicsContext.setPixelShaderTexture( 0u, &m_pTexture->getTextureData() );
		graphicsContext.setPixelShaderConstant( 0u, m_pixelConstants );

		graphicsContext.drawFullScreenQuadPos3( 1.0f );

		graphicsContext.endRenderPass();
	}
}