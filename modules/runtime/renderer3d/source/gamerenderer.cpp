
#include "tiki/renderer/gamerenderer.hpp"

#include "tiki/base/bits.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/material.hpp"
#include "tiki/graphics/model.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/renderer/rendereffect.hpp"
#include "tiki/renderer/renderscene.hpp"
#include "tiki/renderer/renderview.hpp"
#include "tiki/resource/resourcemanager.hpp"
#include "tiki/resource/resourcerequestpool.hpp"

#include "shader/lighting_shader.hpp"
#include "tiki/graphics/shader/cameraparameter.hpp"

#include "renderer.hpp"

namespace tiki
{
	static uint32 getShaderVariantMask( uint directionalLightCount, uint pointLightCount, uint spotLightCount )
	{
		uint32 mask = 0u;
		mask = setBitValue( mask, 0u, 1u, uint32( directionalLightCount ) );
		mask = setBitValue( mask, 1u, 3u, uint32( pointLightCount ) );
		mask = setBitValue( mask, 4u, 1u, uint32( spotLightCount ) );
		return mask;
	}

	GameRenderer::GameRenderer()
	{
		m_pBlendStateAdd				= nullptr;
		m_pBlendStateSet				= nullptr;
		m_pDepthStencilState			= nullptr;
		m_pRasterizerState				= nullptr;
		m_pSamplerLinear				= nullptr;
		m_pSamplerNearst				= nullptr;

		m_pLightingShader				= nullptr;
		m_pLightingInputBinding			= nullptr;

		m_pReflectionTexture			= nullptr;

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_pVisualizationShader			= nullptr;
		m_pVisualizationInputBinding	= nullptr;
#endif
	}

	GameRenderer::~GameRenderer()
	{
	}

	bool GameRenderer::create( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool, const GameRendererParamaters& parameters )
	{
		m_context.pGraphicsSystem		= &graphicsSystem;
		m_context.rendererWidth			= parameters.rendererWidth;
		m_context.rendererHeight		= parameters.rendererHeight;
		m_context.pGBuffer0				= &m_geometryBufferData[ 0u ];
		m_context.pGBuffer1				= &m_geometryBufferData[ 1u ];
		m_context.pGBuffer2				= &m_geometryBufferData[ 2u ];
		m_context.pAccumulationBuffer	= &m_accumulationData;
		m_context.pDepthBuffer			= &m_readOnlyDepthBuffer;

		if( !m_effectSystem.create() ||
			!createTextureData() ||
			!createRenderTargets() )
		{
			dispose( resourceRequestPool );
			return false;
		}

		m_pBlendStateAdd		= graphicsSystem.createBlendState( true, Blend_SourceAlpha, Blend_InverseSourceAlpha, BlendOperation_Add, ColorWriteMask_All );
		m_pBlendStateSet		= graphicsSystem.createBlendState( false, Blend_One, Blend_Zero, BlendOperation_Add, ColorWriteMask_All );
		m_pDepthStencilState	= graphicsSystem.createDepthStencilState( true, true );
		m_pRasterizerState		= graphicsSystem.createRasterizerState( FillMode_Solid, CullMode_Back, WindingOrder_Clockwise );
		m_pSamplerLinear		= graphicsSystem.createSamplerState( AddressMode_Clamp, AddressMode_Clamp, AddressMode_Clamp, FilterMode_Linear, FilterMode_Linear );
		m_pSamplerNearst		= graphicsSystem.createSamplerState( AddressMode_Clamp, AddressMode_Clamp, AddressMode_Clamp, FilterMode_Nearest, FilterMode_Nearest );
		
		if ( m_pBlendStateAdd == nullptr || m_pBlendStateSet == nullptr || m_pDepthStencilState == nullptr || m_pRasterizerState == nullptr || m_pSamplerLinear == nullptr || m_pSamplerNearst == nullptr )
		{
			dispose( resourceRequestPool );
			return false;
		}

		if( !m_lightingPixelConstants.create( graphicsSystem, sizeof( LightingPixelConstantData ), "LightingPixelConstants" ) ||
			!m_cameraParameterConstants.create( graphicsSystem, sizeof( CameraParameter ), "LightingCameraConstants" ) )
		{
			dispose( resourceRequestPool );
			return false;
		}
		
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_visualizationMode = VisualizationMode_Invalid;

		resourceRequestPool.beginLoadResource< ShaderSet >( &m_pVisualizationShader, "visualization.shader" );
#endif

		resourceRequestPool.beginLoadResource< ShaderSet >( &m_pLightingShader, "lighting.shader" );
		resourceRequestPool.beginLoadResource< Texture >( &m_pReflectionTexture, "skybox_sea.texture" );

		return true;
	}

	bool GameRenderer::createShaderResources( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool )
	{
		m_pLightingInputBinding = graphicsSystem.createVertexInputBinding( m_pLightingShader->getShader( ShaderType_VertexShader, 0u ), graphicsSystem.getStockVertexFormat( StockVertexFormat_Pos2Tex2 ) );
		if( m_pLightingInputBinding == nullptr )
		{
			dispose( resourceRequestPool );
			return false;
		}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_pVisualizationInputBinding = graphicsSystem.createVertexInputBinding( m_pVisualizationShader->getShader( ShaderType_VertexShader, 0u ), graphicsSystem.getStockVertexFormat( StockVertexFormat_Pos2Tex2 ) );
		if ( m_pVisualizationInputBinding == nullptr )
		{
			dispose( resourceRequestPool );
			return false;
		}
#endif

		m_effectSystem.createShaderResourcees( graphicsSystem, resourceRequestPool );

		return true;
	}

	void GameRenderer::dispose( ResourceRequestPool& resourceRequestPool )
	{
		TIKI_ASSERT( m_context.pGraphicsSystem != nullptr );
		GraphicsSystem& graphicsSystem = *m_context.pGraphicsSystem;

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		graphicsSystem.disposeVertexInputBinding( m_pVisualizationInputBinding );
		m_pVisualizationInputBinding = nullptr;

		resourceRequestPool.unloadResource( m_pVisualizationShader );
#endif

		m_cameraParameterConstants.dispose( graphicsSystem );
		m_lightingPixelConstants.dispose( graphicsSystem );
		
		graphicsSystem.disposeVertexInputBinding( m_pLightingInputBinding );
		m_pLightingInputBinding = nullptr;
		
		resourceRequestPool.unloadResource( m_pReflectionTexture );
		resourceRequestPool.unloadResource( m_pLightingShader );
		
		graphicsSystem.disposeBlendState( m_pBlendStateAdd );
		graphicsSystem.disposeBlendState( m_pBlendStateSet );
		m_pBlendStateAdd = nullptr;
		m_pBlendStateSet = nullptr;
		
		graphicsSystem.disposeDepthStencilState( m_pDepthStencilState );
		m_pDepthStencilState = nullptr;
		
		graphicsSystem.disposeRasterizerState( m_pRasterizerState );
		m_pRasterizerState = nullptr;
		
		graphicsSystem.disposeSamplerState( m_pSamplerLinear );
		graphicsSystem.disposeSamplerState( m_pSamplerNearst );
		m_pSamplerLinear = nullptr;
		m_pSamplerNearst = nullptr;
		
		disposeRenderTargets();
		disposeTextureData();

		m_effectSystem.dispose();
	}

	bool GameRenderer::resize( uint width, uint height )
	{
		disposeRenderTargets();
		disposeTextureData();

		m_context.rendererWidth		= TIKI_MAX( 10u, width );
		m_context.rendererHeight	= TIKI_MAX( 10u, height );

		if( createTextureData() == false || createRenderTargets() == false )
		{
			return false;
		}

		return true;
	}

	bool GameRenderer::registerRenderEffect( RenderEffect& renderEffect, ResourceRequestPool& resourceRequestPool )
	{
		if( !renderEffect.create( *m_context.pGraphicsSystem, resourceRequestPool ) )
		{
			return false;
		}

		m_effectSystem.registerRenderEffect( &renderEffect );

		return true;
	}

	void GameRenderer::unregisterRenderEffect( RenderEffect& renderEffect, ResourceRequestPool& resourceRequestPool )
	{
		m_effectSystem.unregisterRenderEffect( &renderEffect );

		renderEffect.dispose( *m_context.pGraphicsSystem, resourceRequestPool );
	}

	void GameRenderer::renderView( GraphicsContext& graphicsContext, const RenderTarget& renderTarget, RenderView& view ) const
	{
		view.prepareForRenderTarget( renderTarget );

		if( renderTarget.getWidth() != m_geometryTarget.getWidth() ||
			renderTarget.getHeight() != m_geometryTarget.getHeight() )
		{
			// TODO: remove deferred rendering
			TIKI_TRACE_ERROR( "[renderer] Currently the view have the same size as the G-Buffer.\n");
			return;
		}

		renderGeometry( graphicsContext, view );
		renderLighting( graphicsContext, renderTarget, view );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		renderVisualization( graphicsContext, renderTarget, view );
#endif
	}

	bool GameRenderer::createTextureData()
	{
		GraphicsSystem& graphicsSystem = *m_context.pGraphicsSystem;

		TextureDescription desc;
		desc.width		= (uint16)m_context.rendererWidth;
		desc.height		= (uint16)m_context.rendererHeight;
		desc.depth		= 1u;
		desc.type		= TextureType_2d;
		desc.mipCount	= 1u;
		desc.arrayCount	= 1u;

		desc.format		= PixelFormat_Depth24Stencil8;
		desc.flags		= TextureFlags_DepthStencil;
		if ( m_depthBuffer.create( graphicsSystem, desc ) == false ) return false;

		desc.flags		= TextureFlags_ShaderInput;
		if ( m_readOnlyDepthBuffer.create( graphicsSystem, desc ) == false ) return false;

		desc.format		= PixelFormat_R8G8B8A8_Gamma;
		desc.flags		= TextureFlags_ShaderInput | TextureFlags_RenderTarget;
		if ( m_geometryBufferData[ 0u ].create( graphicsSystem, desc ) == false ) return false;
		if ( m_geometryBufferData[ 1u ].create( graphicsSystem, desc ) == false ) return false;

		desc.format		= PixelFormat_R16G16B16A16_Float;
		if ( m_geometryBufferData[ 2u ].create( graphicsSystem, desc ) == false ) return false;

		desc.format		= PixelFormat_R8G8B8A8_Gamma;
		if ( m_accumulationData.create( graphicsSystem, desc ) == false ) return false;

		return true;
	}

	bool GameRenderer::createRenderTargets()
	{
		GraphicsSystem& graphicsSystem = *m_context.pGraphicsSystem;
		const uint width	= m_context.rendererWidth;
		const uint height	= m_context.rendererHeight;

		// geometry buffer
		{
			RenderTargetBuffer depthBuffer( m_depthBuffer );
			RenderTargetBuffer colorBuffers[] =
			{
				RenderTargetBuffer( m_geometryBufferData[ 0u ] ),
				RenderTargetBuffer( m_geometryBufferData[ 1u ] ),
				RenderTargetBuffer( m_geometryBufferData[ 2u ] )
			};

			if ( m_geometryTarget.create( graphicsSystem, width, height, colorBuffers, TIKI_COUNT( colorBuffers ), &depthBuffer ) == false )
			{
				return false;
			}
		}

		// accumulation buffer
		{
			RenderTargetBuffer depthBuffer( m_depthBuffer );
			RenderTargetBuffer colorBuffers[] =
			{
				RenderTargetBuffer( m_accumulationData )
			};

			if ( m_accumulationTarget.create( graphicsSystem, width, height, colorBuffers, TIKI_COUNT( colorBuffers ), &depthBuffer ) == false )
			{
				return false;
			}
		}

		return true;
	}

	void GameRenderer::disposeTextureData()
	{
		GraphicsSystem& graphicsSystem = *m_context.pGraphicsSystem;
		
		m_depthBuffer.dispose( graphicsSystem );
		m_readOnlyDepthBuffer.dispose( graphicsSystem );

		for (uint i = 0u; i < TIKI_COUNT( m_geometryBufferData ); ++i)
		{
			m_geometryBufferData[ i ].dispose( graphicsSystem );
		}		

		m_accumulationData.dispose( graphicsSystem );
	}

	void GameRenderer::disposeRenderTargets()
	{
		GraphicsSystem& graphicsSystem = *m_context.pGraphicsSystem;

		m_geometryTarget.dispose( graphicsSystem );
		m_accumulationTarget.dispose( graphicsSystem );
	}

	void GameRenderer::renderGeometry( GraphicsContext& graphicsContext, const RenderView& view ) const
	{
		graphicsContext.beginRenderPass( m_geometryTarget );
		graphicsContext.clear( m_geometryTarget, TIKI_COLOR_BLACK );

		m_effectSystem.renderView( graphicsContext, m_context, view, RenderPass_Geometry );

		graphicsContext.endRenderPass();

		graphicsContext.copyTextureData( m_depthBuffer, m_readOnlyDepthBuffer );
	}

	void GameRenderer::renderLighting( GraphicsContext& graphicsContext, const RenderTarget& renderTarget, const RenderView& view ) const
	{
		const RenderScene& scene = view.getScene();
		const Camera& camera = view.getCamera();

		graphicsContext.beginRenderPass( m_accumulationTarget );
		graphicsContext.clear( m_accumulationTarget, TIKI_COLOR_BLACK, 1.0f, 0u, ClearMask_Color );

		graphicsContext.setBlendState( m_pBlendStateAdd );
		graphicsContext.setDepthStencilState( m_pDepthStencilState );
		graphicsContext.setRasterizerState( m_pRasterizerState );

		graphicsContext.setPixelShaderTexture( 0u, &m_geometryBufferData[ 0u ] );
		graphicsContext.setPixelShaderTexture( 1u, &m_geometryBufferData[ 1u ] );
		graphicsContext.setPixelShaderTexture( 2u, &m_geometryBufferData[ 2u ] );
		graphicsContext.setPixelShaderTexture( 3u, &m_readOnlyDepthBuffer );
		graphicsContext.setPixelShaderTexture( 4u, &m_pReflectionTexture->getTextureData() );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerNearst );
		graphicsContext.setPixelShaderSamplerState( 1u, m_pSamplerLinear );
		
		Matrix44 inverseProjection;
		matrix::invert( inverseProjection, camera.getProjection().getMatrix() );

		//Matrix44 inverseViewProjection;
		//matrix::invert( inverseViewProjection, m_frameData.mainCamera.getViewProjectionMatrix() );

		CameraParameter* pCameraConstants = static_cast< CameraParameter* >( graphicsContext.mapBuffer( m_cameraParameterConstants ) );
		TIKI_ASSERT( pCameraConstants != nullptr );
		*pCameraConstants = fillCameraParameter( camera );
		graphicsContext.unmapBuffer( m_cameraParameterConstants );

		LightingPixelConstantData* pPixelConstants = static_cast< LightingPixelConstantData* >( graphicsContext.mapBuffer( m_lightingPixelConstants ) );
		TIKI_ASSERT( pPixelConstants != nullptr );

		createGraphicsMatrix44( pPixelConstants->inverseProjection, inverseProjection );

		const Vector3& cameraPosition = camera.getPosition();
		createFloat4( pPixelConstants->param0, cameraPosition.x, cameraPosition.y, cameraPosition.z, 0.0f );

		const RenderScene::DirectionalLightDataArray& directionalLights	= scene.getDirectionalLights();
		const RenderScene::PointLightDataArray& pointLights				= scene.getPointLights();
		const RenderScene::SpotLightDataArray& spotLights				= scene.getSpotLights();
		for (uint i = 0u; i < directionalLights.getCount(); ++i)
		{
			const DirectionalLightData& lightData = directionalLights[ i ];
			pPixelConstants->directionalLights[ i ] = fillDirectionalLightData( lightData.direction, lightData.color );
		}

		for (uint i = 0u; i < pointLights.getCount(); ++i)
		{
			const PointLightData& lightData = pointLights[ i ];
			pPixelConstants->pointLights[ i ] = fillPointLightData( lightData.position, camera.getWorldMatrix(), camera.getPosition(), lightData.color, lightData.range );
		}
		
		for (uint i = 0u; i < spotLights.getCount(); ++i)
		{
			const SpotLightData& lightData = spotLights[ i ];
			pPixelConstants->spotLights[ i ] = fillSpotLightData( lightData.position, camera.getViewMatrix(), lightData.direction, lightData.color, lightData.range, lightData.theta, lightData.phi );
		} 

		graphicsContext.unmapBuffer( m_lightingPixelConstants );

		const uint32 shaderMask = getShaderVariantMask( directionalLights.getCount(), pointLights.getCount(), spotLights.getCount() );
		graphicsContext.setVertexShader( m_pLightingShader->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setPixelShader( m_pLightingShader->getShader( ShaderType_PixelShader, shaderMask ) );
		graphicsContext.setVertexInputBinding( m_pLightingInputBinding );
		graphicsContext.setPixelShaderConstant( 0u, m_lightingPixelConstants );
		graphicsContext.setPixelShaderConstant( 1u, m_cameraParameterConstants );

		graphicsContext.drawFullScreenQuadPos2Tex2();

		graphicsContext.endRenderPass();
	}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	void GameRenderer::renderVisualization( GraphicsContext& graphicsContext, const RenderTarget& renderTarget, const RenderView& view ) const
	{
		if ( m_visualizationMode == VisualizationMode_Invalid )
		{
			return;
		}

		graphicsContext.beginRenderPass( renderTarget );

		graphicsContext.setBlendState( m_pBlendStateSet );
		graphicsContext.setDepthStencilState( m_pDepthStencilState );
		graphicsContext.setRasterizerState( m_pRasterizerState );

		graphicsContext.setPixelShaderTexture( 0u, &m_geometryBufferData[ 0u ] );
		graphicsContext.setPixelShaderTexture( 1u, &m_geometryBufferData[ 1u ] );
		graphicsContext.setPixelShaderTexture( 2u, &m_geometryBufferData[ 2u ] );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerLinear );
		
		graphicsContext.setVertexShader( m_pVisualizationShader->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setVertexInputBinding( m_pVisualizationInputBinding );
		graphicsContext.setPixelShader( m_pVisualizationShader->getShader( ShaderType_PixelShader, m_visualizationMode ) );

		graphicsContext.drawFullScreenQuadPos2Tex2();

		graphicsContext.endRenderPass();
	}
#endif
}