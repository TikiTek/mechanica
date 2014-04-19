
#include "tiki/renderer/scenerendereffect.hpp"

#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/material.hpp"
#include "tiki/graphics/modelgeometry.hpp"
#include "tiki/graphics/samplerstate.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/graphics/graphicstypes.hpp"
#include "tiki/renderer/scenerendereffectdata.hpp"
#include "tiki/renderer/rendercommand.hpp"
#include "tiki/renderer/renderercontext.hpp"
#include "tiki/resource/resourcemanager.hpp"

#include "shader/scene_shader.hpp"

namespace tiki
{
	SceneRenderEffect::SceneRenderEffect()
	{
		m_pShader				= nullptr;
		
		m_pBlendState			= nullptr;
		m_pDepthStencilState	= nullptr;
		m_pRasterizerState		= nullptr;
		m_pSampler				= nullptr;

		m_pVertexFormat			= nullptr;
		m_pVertexInputBinding	= nullptr;
	}

	SceneRenderEffect::~SceneRenderEffect()
	{
		TIKI_ASSERT( m_pShader == nullptr );

		TIKI_ASSERT( m_pBlendState == nullptr );
		TIKI_ASSERT( m_pDepthStencilState == nullptr );
		TIKI_ASSERT( m_pRasterizerState == nullptr );
		TIKI_ASSERT( m_pSampler == nullptr );

		TIKI_ASSERT( m_pVertexFormat == nullptr );
		TIKI_ASSERT( m_pVertexInputBinding == nullptr );
	}

	bool SceneRenderEffect::createInternal( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		bool success = true;
		m_pShader				= resourceManager.loadResource< ShaderSet >( "scene.shader" );
		success &= ( m_pShader != nullptr );

		m_pBlendState			= graphicsSystem.createBlendState( false, Blend_One, Blend_Zero, BlendOperation_Add, ColorWriteMask_All );
		m_pDepthStencilState	= graphicsSystem.createDepthStencilState( true, true );
		m_pRasterizerState		= graphicsSystem.createRasterizerState( FillMode_Solid, CullMode_Back, WindingOrder_Clockwise );
		m_pSampler				= graphicsSystem.createSamplerState( AddressMode_Clamp, AddressMode_Clamp, AddressMode_Clamp, FilterMode_Linear, FilterMode_Linear );
		success &= ( m_pBlendState != nullptr && m_pDepthStencilState != nullptr && m_pRasterizerState != nullptr && m_pSampler != nullptr );

		success &= m_vertexConstantBuffer.create( graphicsSystem, alignValue( sizeof( SceneVertexConstantData ), 16u ) );
		success &= m_pixelConstantBuffer.create( graphicsSystem, alignValue( sizeof( ScenePixelConstantData ), 16u ) );
		
		VertexAttribute attributes[] =
		{
			{ VertexSementic_Position,		0u, VertexAttributeFormat_x32y32z32_float,		0u, VertexInputType_PerVertex },
			{ VertexSementic_Normal,		0u, VertexAttributeFormat_x32y32z32_float,		0u, VertexInputType_PerVertex },
			{ VertexSementic_TangentFlip,	0u, VertexAttributeFormat_x32y32z32w32_float,	0u, VertexInputType_PerVertex },
			{ VertexSementic_TexCoord,		0u, VertexAttributeFormat_x16y16_float,			0u, VertexInputType_PerVertex }
		};

		m_pVertexFormat			= graphicsSystem.createVertexFormat( attributes, TIKI_COUNT( attributes ) );
		m_pVertexInputBinding	= graphicsSystem.createVertexInputBinding( m_pShader->getShader( ShaderType_VertexShader, 0u ), m_pVertexFormat );
		success &= ( m_pVertexFormat != nullptr && m_pVertexInputBinding != nullptr );

		if ( !success )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		return true;
	}

	void SceneRenderEffect::disposeInternal( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		graphicsSystem.disposeBlendState( m_pBlendState );
		m_pBlendState = nullptr;

		graphicsSystem.disposeDepthStencilState( m_pDepthStencilState );
		m_pDepthStencilState = nullptr;

		graphicsSystem.disposeRasterizerState( m_pRasterizerState );
		m_pRasterizerState = nullptr;

		graphicsSystem.disposeSamplerState( m_pSampler );
		m_pSampler = nullptr;

		graphicsSystem.disposeVertexInputBinding( m_pVertexInputBinding );
		m_pVertexInputBinding = nullptr;

		graphicsSystem.disposeVertexFormat( m_pVertexFormat );
		m_pVertexFormat = nullptr;

		resourceManager.unloadResource< ShaderSet >( m_pShader );
		m_pShader = nullptr;

		m_vertexConstantBuffer.dispose( graphicsSystem );
		m_pixelConstantBuffer.dispose( graphicsSystem );
	}

	void SceneRenderEffect::executeRenderSequencesInternal( GraphicsContext& graphicsContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount, const FrameData& frameData, const RendererContext& rendererContext )
	{
		const Shader* pVertexShader = m_pShader->getShader( ShaderType_VertexShader, 0u );

		graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleList );
		graphicsContext.setBlendState( m_pBlendState );
		graphicsContext.setDepthStencilState( m_pDepthStencilState );
		graphicsContext.setRasterizerState( m_pRasterizerState );
		graphicsContext.setVertexInputBinding( m_pVertexInputBinding );

		graphicsContext.setVertexShader( pVertexShader );
		graphicsContext.setPixelShader( m_pShader->getShader( ShaderType_PixelShader, 0u ) );

		graphicsContext.setVertexShaderConstant( 0u, m_vertexConstantBuffer );
		graphicsContext.setPixelShaderConstant( 0u, m_pixelConstantBuffer );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSampler );

		for (uint sequnceIndex = 0u; sequnceIndex < sequenceCount; ++sequnceIndex)
		{
			const RenderSequence& sequence = pSequences[ sequnceIndex ];

			for (uint commandIndex = 0u; commandIndex < sequence.commandCount; ++commandIndex)
			{
				const RenderCommand& command = sequence.pCommands[ commandIndex ];
				const ModelGeometry& geometry = *command.pGeometry;

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
				if ( geometry.getVertexFormat() != m_pVertexFormat )
				{
					TIKI_TRACE_ERROR( "[renderer] Invalid vertex format. Geometry will be ignored.\n" );
					continue;
				}

				if ( command.pRenderEffectData->renderEffectId != RenderEffectId_Scene )
				{
					TIKI_TRACE_ERROR( "[renderer] Invalid effect data. Geometry will be ignored.\n" );
					continue;
				}
#endif

				const SceneRenderEffectData* pRenderEffectData = static_cast< const SceneRenderEffectData* >( command.pRenderEffectData );

				Matrix33 mvMtx = command.worldTransform.rot;
				matrix::mul( mvMtx, frameData.mainCamera.getViewMatrix().rot );

				Matrix44 mvpMtx;
				matrix::set( mvpMtx, command.worldTransform );
				matrix::mul( mvpMtx, frameData.mainCamera.getViewProjectionMatrix() );

				SceneVertexConstantData* pVertexConstants = static_cast< SceneVertexConstantData* >( graphicsContext.mapBuffer( m_vertexConstantBuffer ) );
				createGraphicsMatrix44( pVertexConstants->mvpMatrix, mvpMtx );
				createGraphicsMatrix33( pVertexConstants->modelViewMatrix, mvMtx );
				graphicsContext.unmapBuffer( m_vertexConstantBuffer );	

				ScenePixelConstantData* pPixelConstants = static_cast< ScenePixelConstantData* >( graphicsContext.mapBuffer( m_pixelConstantBuffer ) );
				pPixelConstants->selfIlluminationFactor = pRenderEffectData->selfIlluminationFactor;
				pPixelConstants->specluarBrightness		= pRenderEffectData->specluarBrightness;
				pPixelConstants->specluarIntensity		= pRenderEffectData->specluarIntensity;
				pPixelConstants->specluarPower			= pRenderEffectData->specluarPower;
				graphicsContext.unmapBuffer( m_pixelConstantBuffer );	

				const Texture* pDiffuseMap	= pRenderEffectData->diffuseMap.getData();
				const Texture* pNormalMap	= pRenderEffectData->normalMap.getData();
				const Texture* pSelfilluMap	= pRenderEffectData->selfilluMap.getData();
				const TextureData* pDiffuseMapData	= ( pDiffuseMap == nullptr ? nullptr : &pDiffuseMap->getTextureData() );
				const TextureData* pNormalMapData	= ( pNormalMap == nullptr ? nullptr : &pNormalMap->getTextureData() );
				const TextureData* pSelfilluMapFata	= ( pSelfilluMap == nullptr ? nullptr : &pSelfilluMap->getTextureData() );

				graphicsContext.setPixelShaderTexture( 0u, pDiffuseMapData );
				graphicsContext.setPixelShaderTexture( 1u, pNormalMapData );
				graphicsContext.setPixelShaderTexture( 2u, pSelfilluMapFata );

				geometry.render( graphicsContext );
			}
		} 
	}

}