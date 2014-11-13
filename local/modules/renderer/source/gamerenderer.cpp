
#include "tiki/renderer/gamerenderer.hpp"

#include "tiki/base/bits.hpp"
#include "tiki/graphics/graphicscontext.hpp"
#include "tiki/graphics/model.hpp"

#include "shader/lighting_shader.hpp"
#include "tiki/graphics/shader/cameraparameter.hpp"

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

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_pVisualizationShader			= nullptr;
		m_pVisualizationInputBinding	= nullptr;
#endif
	}

	GameRenderer::~GameRenderer()
	{
	}

	bool GameRenderer::create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const GameRendererParamaters& parameters )
	{
		m_context.pGraphicsSystem		= &graphicsSystem;
		m_context.rendererWidth			= parameters.rendererWidth;
		m_context.rendererHeight		= parameters.rendererHeight;
		m_context.pGBuffer0				= &m_geometryBufferData[ 0u ];
		m_context.pGBuffer1				= &m_geometryBufferData[ 1u ];
		m_context.pGBuffer2				= &m_geometryBufferData[ 2u ];
		m_context.pAccumulationBuffer	= &m_accumulationData;
		m_context.pDepthBuffer			= &m_readOnlyDepthBuffer;

		m_frameData.nearPlane			= parameters.nearPlane;
		m_frameData.farPlane			= parameters.farPlane;
		m_frameData.aspectRatio			= (float)parameters.rendererWidth / (float)parameters.rendererHeight;

		if ( m_renderBatch.create( parameters.maxSeqeuenceCount, parameters.maxRenderCommandCount ) == false )
		{
			dispose( resourceManager );
			return false;
		}

		if ( m_renderEffectSystem.create( m_context ) == false )
		{
			dispose( resourceManager );
			return false;
		}

		if ( createTextureData() == false )
		{
			dispose( resourceManager );
			return false;
		}

		if ( createRenderTargets() == false )
		{
			dispose( resourceManager );
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
			dispose( resourceManager );
			return false;
		}

		m_pLightingShader = resourceManager.loadResource< ShaderSet >( "lighting.shader" );
		if ( m_pLightingShader == nullptr )
		{
			dispose( resourceManager );
			return false;
		}

		m_pLightingInputBinding = graphicsSystem.createVertexInputBinding( m_pLightingShader->getShader( ShaderType_VertexShader, 0u ), graphicsSystem.getStockVertexFormat( StockVertexFormat_Pos2Tex2 ) );

		if( m_pLightingInputBinding == nullptr
			|| !m_lightingPixelConstants.create( graphicsSystem, sizeof( LightingPixelConstantData ) )
			|| !m_cameraParameterConstants.create( graphicsSystem, sizeof( CameraParameter ) ) )
		{
			dispose( resourceManager );
			return false;
		}
		
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_pVisualizationShader = resourceManager.loadResource< ShaderSet >( "visualization.shader" );
		if ( m_pVisualizationShader == nullptr )
		{
			dispose( resourceManager );
			return false;
		}

		m_pVisualizationInputBinding = graphicsSystem.createVertexInputBinding( m_pVisualizationShader->getShader( ShaderType_VertexShader, 0u ), graphicsSystem.getStockVertexFormat( StockVertexFormat_Pos2Tex2 ) );
		if ( m_pVisualizationInputBinding == nullptr )
		{
			dispose( resourceManager );
			return false;
		}

		m_visualizationMode = VisualizationMode_Invalid;
#endif

		return true;
	}

	void GameRenderer::dispose( ResourceManager& resourceManager )
	{
		TIKI_ASSERT( m_context.pGraphicsSystem != nullptr );
		GraphicsSystem& graphicsSystem = *m_context.pGraphicsSystem;

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		graphicsSystem.disposeVertexInputBinding( m_pVisualizationInputBinding );
		m_pVisualizationInputBinding = nullptr;

		resourceManager.unloadResource( m_pVisualizationShader );
		m_pVisualizationShader = nullptr;
#endif

		m_cameraParameterConstants.dispose( graphicsSystem );
		m_lightingPixelConstants.dispose( graphicsSystem );

		graphicsSystem.disposeVertexInputBinding( m_pLightingInputBinding );
		m_pLightingInputBinding = nullptr;

		resourceManager.unloadResource( m_pLightingShader );
		m_pLightingShader = nullptr;

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

		m_renderEffectSystem.dispose();
		m_renderBatch.dispose();
	}


	bool GameRenderer::resize( uint width, uint height )
	{
		disposeRenderTargets();
		disposeTextureData();

		m_context.rendererWidth		= TIKI_MAX( 10u, width );
		m_context.rendererHeight	= TIKI_MAX( 10u, height );

		m_frameData.aspectRatio		= (float)width / (float)height;

		if ( createTextureData() == false || createRenderTargets() == false )
		{
			return false;
		}

		return true;
	}

	void GameRenderer::registerRenderEffect( RenderEffect* pRenderEffect )
	{
		m_renderEffectSystem.registerRenderEffect( pRenderEffect );
	}

	void GameRenderer::unregisterRenderEffect( RenderEffect* pRenderEffect )
	{
		m_renderEffectSystem.unregisterRenderEffect( pRenderEffect );
	}
	
	void GameRenderer::queueModel( const Model* pModel, const Matrix43* pWorldTransform /*= nullptr*/, const SkinningData** ppSkinningData /*= nullptr*/ )
	{
		m_renderBatch.beginSequence( RenderPassMask_Geometry, (RenderEffectId)pModel->getMaterial()->getData()->renderEffectId, 0u );
		for (uint i = 0u; i < pModel->getGeometryCount(); ++i)
		{
			const SkinningData* pSkinningData = ( ppSkinningData != nullptr ? ppSkinningData[ i ] : nullptr );
			m_renderBatch.queueGeometry( pModel->getGeometryByIndex( i ), pModel->getMaterial(), pWorldTransform, pSkinningData );
		}
		m_renderBatch.endSequence();
	}

	void GameRenderer::update()
	{
		m_frameData.reset();
		m_frameData.mainCamera.getProjection().createPerspective(
			m_frameData.aspectRatio,
			f32::piOver4,
			m_frameData.nearPlane,
			m_frameData.farPlane
		);

		m_renderBatch.reset();
		m_renderEffectSystem.setFrameData( m_frameData );
	}

	void GameRenderer::render( GraphicsContext& graphicsContext ) const
	{
		renderGeometry( graphicsContext );
		renderLighting( graphicsContext );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		renderVisualization( graphicsContext );
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

	void GameRenderer::renderGeometry( GraphicsContext& graphicsContext ) const
	{
		graphicsContext.beginRenderPass( m_geometryTarget );
		graphicsContext.clear( m_geometryTarget, TIKI_COLOR_BLACK );

		m_renderEffectSystem.render( graphicsContext, RenderPass_Geometry, m_renderBatch );

		graphicsContext.endRenderPass();

		graphicsContext.copyTextureData( m_depthBuffer, m_readOnlyDepthBuffer );
	}

	void GameRenderer::renderLighting( GraphicsContext& graphicsContext ) const
	{
		graphicsContext.beginRenderPass( m_accumulationTarget );
		graphicsContext.clear( m_accumulationTarget, TIKI_COLOR_BLACK, 1.0f, 0u, ClearMask_Color );

		graphicsContext.setBlendState( m_pBlendStateAdd );
		graphicsContext.setDepthStencilState( m_pDepthStencilState );
		graphicsContext.setRasterizerState( m_pRasterizerState );

		graphicsContext.setPixelShaderTexture( 0u, &m_geometryBufferData[ 0u ] );
		graphicsContext.setPixelShaderTexture( 1u, &m_geometryBufferData[ 1u ] );
		graphicsContext.setPixelShaderTexture( 2u, &m_geometryBufferData[ 2u ] );
		graphicsContext.setPixelShaderTexture( 3u, &m_readOnlyDepthBuffer );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerNearst );
		
		Matrix44 inverseProjection;
		matrix::invert( inverseProjection, m_frameData.mainCamera.getProjection().getMatrix() );

		//Matrix44 inverseViewProjection;
		//matrix::invert( inverseViewProjection, m_frameData.mainCamera.getViewProjectionMatrix() );

		CameraParameter* pCameraConstants = static_cast< CameraParameter* >( graphicsContext.mapBuffer( m_cameraParameterConstants ) );
		TIKI_ASSERT( pCameraConstants != nullptr );
		*pCameraConstants = fillCameraParameter( m_frameData.mainCamera );
		graphicsContext.unmapBuffer( m_cameraParameterConstants );

		LightingPixelConstantData* pPixelConstants = static_cast< LightingPixelConstantData* >( graphicsContext.mapBuffer( m_lightingPixelConstants ) );
		TIKI_ASSERT( pPixelConstants != nullptr );

		//createGraphicsMatrix44( pPixelConstants->inverseProjection, inverseProjection );
		createGraphicsMatrix44( pPixelConstants->inverseProjection, inverseProjection );

		const Vector3& cameraPosition = m_frameData.mainCamera.getPosition();
		createFloat4( pPixelConstants->param0, cameraPosition.x, cameraPosition.y, cameraPosition.z, 0.0f );

		const uint directionalLightCount	= m_frameData.directionalLights.getCount();
		const uint pointLightCount			= m_frameData.pointLights.getCount();
		const uint spotLightCount			= m_frameData.spotLights.getCount();
		for (uint i = 0u; i < directionalLightCount; ++i)
		{
			const DirectionalLightData& lightData = m_frameData.directionalLights[ i ];
			pPixelConstants->directionalLights[ i ] = fillDirectionalLightData( lightData.direction, lightData.color );
		}

		for (uint i = 0u; i < pointLightCount; ++i)
		{
			const PointLightData& lightData = m_frameData.pointLights[ i ];
			pPixelConstants->pointLights[ i ] = fillPointLightData( lightData.position, m_frameData.mainCamera.getWorldMatrix(), m_frameData.mainCamera.getPosition(), lightData.color, lightData.range );
		}
		
		for (uint i = 0u; i < spotLightCount; ++i)
		{
			const SpotLightData& lightData = m_frameData.spotLights[ i ];
			pPixelConstants->spotLights[ i ] = fillSpotLightData( lightData.position, m_frameData.mainCamera.getViewMatrix(), lightData.direction, lightData.color, lightData.range, lightData.theta, lightData.phi );
		} 

		graphicsContext.unmapBuffer( m_lightingPixelConstants );

		const uint32 shaderMask = getShaderVariantMask( directionalLightCount, pointLightCount, spotLightCount );
		graphicsContext.setVertexShader( m_pLightingShader->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setPixelShader( m_pLightingShader->getShader( ShaderType_PixelShader, shaderMask ) );
		graphicsContext.setVertexInputBinding( m_pLightingInputBinding );
		graphicsContext.setPixelShaderConstant( 0u, m_lightingPixelConstants );
		graphicsContext.setPixelShaderConstant( 1u, m_cameraParameterConstants );

		graphicsContext.drawFullScreenQuadPos2Tex2();

		graphicsContext.endRenderPass();
	}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	void GameRenderer::renderVisualization( GraphicsContext& graphicsContext ) const
	{
		if ( m_visualizationMode == VisualizationMode_Invalid )
		{
			return;
		}

		graphicsContext.beginRenderPass( m_accumulationTarget );

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