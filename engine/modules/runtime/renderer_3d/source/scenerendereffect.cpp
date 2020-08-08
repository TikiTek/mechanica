
#include "tiki/renderer/scenerendereffect.hpp"

#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/graphicstypes.hpp"
#include "tiki/graphics/material.hpp"
#include "tiki/graphics/modelgeometry.hpp"
#include "tiki/graphics/samplerstate.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/graphics/skinningdata.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/renderer/rendercommand.hpp"
#include "tiki/renderer/renderercontext.hpp"
#include "tiki/renderer/renderview.hpp"
#include "tiki/resource/resourcerequestpool.hpp"

#include "shader/scene_shader.hpp"

#include "renderer.hpp"

namespace tiki
{
	SceneRenderEffect::SceneRenderEffect()
	{
		m_pShader				= nullptr;
		
		m_pBlendState			= nullptr;
		m_pDepthStencilState	= nullptr;
		m_pRasterizerState		= nullptr;
		m_pSampler				= nullptr;

		m_pVertexFormat[ ModelType_Static ]			= nullptr;
		m_pVertexInputBinding[ ModelType_Static ]	= nullptr;
		m_pVertexFormat[ ModelType_Skinned ]		= nullptr;
		m_pVertexInputBinding[ ModelType_Skinned ]	= nullptr;
	}

	SceneRenderEffect::~SceneRenderEffect()
	{
		TIKI_ASSERT( m_pShader == nullptr );

		TIKI_ASSERT( m_pBlendState == nullptr );
		TIKI_ASSERT( m_pDepthStencilState == nullptr );
		TIKI_ASSERT( m_pRasterizerState == nullptr );
		TIKI_ASSERT( m_pSampler == nullptr );

		TIKI_ASSERT( m_pVertexFormat[ ModelType_Static ]		== nullptr );
		TIKI_ASSERT( m_pVertexInputBinding[ ModelType_Static ]	== nullptr );
		TIKI_ASSERT( m_pVertexFormat[ ModelType_Skinned ]		== nullptr );
		TIKI_ASSERT( m_pVertexInputBinding[ ModelType_Skinned ]	== nullptr );
	}

	bool SceneRenderEffect::createInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool )
	{
		bool success = true;
		
		m_pBlendState			= graphicsSystem.createBlendState( false, Blend_One, Blend_Zero, BlendOperation_Add, ColorWriteMask_All );
		m_pDepthStencilState	= graphicsSystem.createDepthStencilState( true, true );
		m_pRasterizerState		= graphicsSystem.createRasterizerState( FillMode_Solid, CullMode_Back, WindingOrder_Clockwise );
		m_pSampler				= graphicsSystem.createSamplerState( AddressMode_Wrap, AddressMode_Wrap, AddressMode_Clamp, FilterMode_Linear, FilterMode_Linear );
		success &= ( m_pBlendState != nullptr && m_pDepthStencilState != nullptr && m_pRasterizerState != nullptr && m_pSampler != nullptr );

		success &= m_vertexConstantBuffer.create( graphicsSystem, sizeof( SceneVertexConstantData ), "SceneVertexConstants" );
		success &= m_pixelConstantBuffer.create( graphicsSystem, sizeof( ScenePixelConstantData ), "ScenePixelConstants" );
		
		VertexAttribute attributesStatic[] =
		{
			{ VertexSementic_Position,		0u, VertexAttributeFormat_x32y32z32_float,		0u, VertexInputType_PerVertex },
			{ VertexSementic_Normal,		0u, VertexAttributeFormat_x32y32z32_float,		0u, VertexInputType_PerVertex },
			{ VertexSementic_TangentFlip,	0u, VertexAttributeFormat_x32y32z32w32_float,	0u, VertexInputType_PerVertex },
			{ VertexSementic_TexCoord,		0u, VertexAttributeFormat_x16y16_float,			0u, VertexInputType_PerVertex }
		};

		m_pVertexFormat[ ModelType_Static ]			= graphicsSystem.createVertexFormat( attributesStatic, TIKI_COUNT( attributesStatic ) );
		success &= ( m_pVertexFormat[ ModelType_Static ] != nullptr );

		VertexAttribute attributesSkinned[] =
		{
			{ VertexSementic_Position,		0u, VertexAttributeFormat_x32y32z32_float,		0u, VertexInputType_PerVertex },
			{ VertexSementic_Normal,		0u, VertexAttributeFormat_x32y32z32_float,		0u, VertexInputType_PerVertex },
			{ VertexSementic_TangentFlip,	0u, VertexAttributeFormat_x32y32z32w32_float,	0u, VertexInputType_PerVertex },
			{ VertexSementic_TexCoord,		0u, VertexAttributeFormat_x16y16_float,			0u, VertexInputType_PerVertex },
			{ VertexSementic_JointIndex,	0u, VertexAttributeFormat_x8y8z8w8,				0u, VertexInputType_PerVertex },
			{ VertexSementic_JointWeight,	0u, VertexAttributeFormat_x16y16z16w16_unorm,	0u, VertexInputType_PerVertex }
		};

		m_pVertexFormat[ ModelType_Skinned ]		= graphicsSystem.createVertexFormat( attributesSkinned, TIKI_COUNT( attributesSkinned ) );
		success &= ( m_pVertexFormat[ ModelType_Skinned ] != nullptr );

		if ( !success )
		{
			dispose( graphicsSystem, resourceRequestPool );
			return false;
		}

		resourceRequestPool.beginLoadResource< ShaderSet >( &m_pShader, "scene.shader" );

		return true;
	}

	bool SceneRenderEffect::createShaderResourcesInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool )
	{
		m_pVertexInputBinding[ ModelType_Static ]	= graphicsSystem.createVertexInputBinding( m_pShader->getShader( ShaderType_VertexShader, 0u ), m_pVertexFormat[ ModelType_Static ] );
		m_pVertexInputBinding[ ModelType_Skinned ]	= graphicsSystem.createVertexInputBinding( m_pShader->getShader( ShaderType_VertexShader, 1u ), m_pVertexFormat[ ModelType_Skinned ] );

		return m_pVertexInputBinding[ ModelType_Static ] != nullptr && m_pVertexInputBinding[ ModelType_Skinned ] != nullptr;
	}

	void SceneRenderEffect::disposeInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool )
	{
		graphicsSystem.disposeBlendState( m_pBlendState );
		m_pBlendState = nullptr;

		graphicsSystem.disposeDepthStencilState( m_pDepthStencilState );
		m_pDepthStencilState = nullptr;

		graphicsSystem.disposeRasterizerState( m_pRasterizerState );
		m_pRasterizerState = nullptr;

		graphicsSystem.disposeSamplerState( m_pSampler );
		m_pSampler = nullptr;

		graphicsSystem.disposeVertexInputBinding( m_pVertexInputBinding[ ModelType_Static ] );
		m_pVertexInputBinding[ ModelType_Static ] = nullptr;

		graphicsSystem.disposeVertexFormat( m_pVertexFormat[ ModelType_Static ] );
		m_pVertexFormat[ ModelType_Static ] = nullptr;

		graphicsSystem.disposeVertexInputBinding( m_pVertexInputBinding[ ModelType_Skinned ] );
		m_pVertexInputBinding[ ModelType_Skinned ] = nullptr;

		graphicsSystem.disposeVertexFormat( m_pVertexFormat[ ModelType_Skinned ] );
		m_pVertexFormat[ ModelType_Skinned ] = nullptr;

		resourceRequestPool.unloadResource( m_pShader );

		m_vertexConstantBuffer.dispose( graphicsSystem );
		m_pixelConstantBuffer.dispose( graphicsSystem );
	}

	void SceneRenderEffect::executeRenderSequencesInternal( GraphicsContext& graphicsContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount, const RenderView& view, const RendererContext& rendererContext )
	{
		graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleList );
		graphicsContext.setBlendState( m_pBlendState );
		graphicsContext.setDepthStencilState( m_pDepthStencilState );
		graphicsContext.setRasterizerState( m_pRasterizerState );

		graphicsContext.setPixelShader( m_pShader->getShader( ShaderType_PixelShader, 0u ) );

		graphicsContext.setVertexShaderConstant( 0u, m_vertexConstantBuffer );
		graphicsContext.setPixelShaderConstant( 0u, m_pixelConstantBuffer );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSampler );

		for (uint sequnceIndex = 0u; sequnceIndex < sequenceCount; ++sequnceIndex)
		{
			const RenderSequence& sequence = pSequences[ sequnceIndex ];
			
			for (uint commandIndex = 0u; commandIndex < sequence.commandCount; ++commandIndex)
			{
				const RenderCommand& command	= sequence.pCommands[ commandIndex ];
				const ModelGeometry& geometry	= *command.pGeometry;
				const ModelType modelType		= ( command.pSkinningData == nullptr ? ModelType_Static : ModelType_Skinned );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
				if ( geometry.getVertexFormat() != m_pVertexFormat[ modelType ] )
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

				graphicsContext.setVertexShader( m_pShader->getShader( ShaderType_VertexShader, modelType ) );
				graphicsContext.setVertexInputBinding( m_pVertexInputBinding[ modelType ] );

				if ( modelType == ModelType_Skinned )
				{
					graphicsContext.setVertexShaderConstant( 1u, command.pSkinningData->matrices );
				}
				
				const SceneRenderEffectData* pRenderEffectData = static_cast< const SceneRenderEffectData* >( command.pRenderEffectData );

				Matrix44 mvpMtx;
				matrix::set( mvpMtx, command.worldTransform );
				matrix::mul( mvpMtx, view.getCamera().getViewProjectionMatrix() );

				Matrix43 modelView = command.worldTransform;
				//matrix::mul( modelView, frameData.mainCamera.getViewMatrix() );

				Matrix44 modelInverseTranspose;
				matrix::set( modelInverseTranspose, modelView );
				matrix::invert( modelInverseTranspose, modelInverseTranspose );
				matrix::transpose( modelInverseTranspose );

				//Matrix33 modelInverseTranspose;
				//modelInverseTranspose = modelView.rot;
				//matrix::invert( modelInverseTranspose, modelView.rot );
				//matrix::transpose( modelInverseTranspose );

				SceneVertexConstantData* pVertexConstants = static_cast< SceneVertexConstantData* >( graphicsContext.mapBuffer( m_vertexConstantBuffer ) );
				TIKI_ASSERT( pVertexConstants != nullptr );
				createGraphicsMatrix44( pVertexConstants->mvpMatrix, mvpMtx );
				createGraphicsMatrix44( pVertexConstants->modelNormalMatrix, modelInverseTranspose );
				graphicsContext.unmapBuffer( m_vertexConstantBuffer );	

				ScenePixelConstantData* pPixelConstants = static_cast< ScenePixelConstantData* >( graphicsContext.mapBuffer( m_pixelConstantBuffer ) );
				TIKI_ASSERT( pPixelConstants != nullptr );
				pPixelConstants->selfIlluminationFactor = pRenderEffectData->selfIlluminationFactor;
				pPixelConstants->specluarIntensity		= pRenderEffectData->specluarIntensity;
				pPixelConstants->specluarPower			= pRenderEffectData->specluarPower;
				graphicsContext.unmapBuffer( m_pixelConstantBuffer );	

				const Texture* pDiffuseMap	= pRenderEffectData->diffuseMap.getData();
				const Texture* pNormalMap	= pRenderEffectData->normalMap.getData();
				const Texture* pSpecularMap	= pRenderEffectData->specularMap.getData();
				const Texture* pGlossMap	= pRenderEffectData->glossMap.getData();
				const TextureData* pDiffuseMapData	= ( pDiffuseMap == nullptr ? nullptr : &pDiffuseMap->getTextureData() );
				const TextureData* pNormalMapData	= ( pNormalMap == nullptr ? nullptr : &pNormalMap->getTextureData() );
				const TextureData* pSelfilluMapData	= ( pSpecularMap == nullptr ? nullptr : &pSpecularMap->getTextureData() );
				const TextureData* pGlossMapData	= ( pGlossMap == nullptr ? nullptr : &pGlossMap->getTextureData() );

				graphicsContext.setPixelShaderTexture( 0u, pDiffuseMapData );
				graphicsContext.setPixelShaderTexture( 1u, pNormalMapData );
				graphicsContext.setPixelShaderTexture( 2u, pSelfilluMapData );
				graphicsContext.setPixelShaderTexture( 3u, pGlossMapData );

				geometry.render( graphicsContext );
			}
		} 
	}

}