
#include "tiki/renderer/fallbackrendereffect.hpp"

#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/material.hpp"
#include "tiki/graphics/modelgeometry.hpp"
#include "tiki/graphics/samplerstate.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/graphicsbase/graphicstypes.hpp"
#include "tiki/renderer/fallbackrendereffectdata.hpp"
#include "tiki/renderer/rendercommand.hpp"
#include "tiki/renderer/renderercontext.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	struct FallbackVertexConstants
	{
		GraphicsMatrix44	mvpMatrix;
		GraphicsMatrix44	modelViewMatrix;
	};

	FallbackRenderEffect::FallbackRenderEffect()
	{
		m_pGraphicsSystem	= nullptr;

		m_pShaderSet		= nullptr;
		m_pSampler			= nullptr;
	}

	FallbackRenderEffect::~FallbackRenderEffect()
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );

		TIKI_ASSERT( m_pShaderSet == nullptr );
		TIKI_ASSERT( m_pSampler == nullptr );
	}

	bool FallbackRenderEffect::createInternal( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		m_pGraphicsSystem	= &graphicsSystem;
		m_pShaderSet		= resourceManager.loadResource< ShaderSet >( "fallback.shader" );

		m_pSampler = graphicsSystem.createSamplerState(
			AddressMode_Clamp,
			AddressMode_Clamp,
			AddressMode_Clamp,
			FilterMode_Linear,
			FilterMode_Linear
		);

		m_vertexConstantBuffer.create( graphicsSystem, sizeof( FallbackVertexConstants ) );

		m_vertexInputBindings.create( 32u );

		return true;
	}

	void FallbackRenderEffect::disposeInternal( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		graphicsSystem.disposeSamplerState( m_pSampler );
		m_pSampler = nullptr;

		resourceManager.unloadResource< ShaderSet >( m_pShaderSet );
		m_pShaderSet = nullptr;

		m_vertexConstantBuffer.dispose( graphicsSystem );

		for (uint i = 0u; i < m_vertexInputBindings.getCount(); ++i)
		{
			graphicsSystem.disposeVertexInputBinding(
				m_vertexInputBindings.getValueAt( i )
			);
		}
		m_vertexInputBindings.dispose();

		m_pGraphicsSystem = nullptr;
	}

	void FallbackRenderEffect::executeRenderSequencesInternal( GraphicsContext& graphicsContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount, const FrameData& frameData, const RendererContext& rendererContext )
	{
		const Shader* pVertexShader = m_pShaderSet->getShader( ShaderType_VertexShader, 0u );

		graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleList );

		graphicsContext.setVertexShader( pVertexShader );
		graphicsContext.setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 0u ) );

		graphicsContext.setVertexShaderConstant( 0u, m_vertexConstantBuffer );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSampler );

		for (uint sequnceIndex = 0u; sequnceIndex < sequenceCount; ++sequnceIndex)
		{
			const RenderSequence& sequence = pSequences[ sequnceIndex ];

			for (uint commandIndex = 0u; commandIndex < sequence.commandCount; ++commandIndex)
			{
				const RenderCommand& command = sequence.pCommands[ commandIndex ];

				Matrix43 mvMtx = command.worldTransform;
				matrix::mul( mvMtx, frameData.mainCamera.getViewMatrix() );

				Matrix44 mvpMtx;
				matrix::set( mvpMtx, command.worldTransform );
				matrix::mul( mvpMtx, frameData.mainCamera.getViewProjectionMatrix() );

				FallbackVertexConstants* pVertexConstants = static_cast< FallbackVertexConstants* >( graphicsContext.mapBuffer( m_vertexConstantBuffer ) );
				createGraphicsMatrix44( pVertexConstants->mvpMatrix, mvpMtx );
				createGraphicsMatrix44( pVertexConstants->modelViewMatrix, mvMtx );
				graphicsContext.unmapBuffer( m_vertexConstantBuffer );	

				const ModelGeometry& geometry					= *command.pGeometry;
				const VertexInputBinding* pVertexInputBinding	= nullptr;
				const crc32 vertexFormatHash					= geometry.getVertexFormat()->getHashValue();

				if ( m_vertexInputBindings.findValue( &pVertexInputBinding, vertexFormatHash ) == false )
				{
					pVertexInputBinding = m_pGraphicsSystem->createVertexInputBinding(
						pVertexShader,
						geometry.getVertexFormat()
					);
					TIKI_ASSERT( pVertexInputBinding != nullptr );

					m_vertexInputBindings.set( vertexFormatHash, pVertexInputBinding );
				}

				graphicsContext.setVertexInputBinding( pVertexInputBinding );

				if ( command.pRenderEffectData != nullptr )
				{
					if ( command.pRenderEffectData->renderEffectId == RenderEffectId_Fallback )
					{
						const FallbackRenderEffectData* pRenderEffectData = static_cast< const FallbackRenderEffectData* >( command.pRenderEffectData );
						const Texture* pTexture = pRenderEffectData->defaultTexture.getData();

						if ( pTexture != nullptr )
						{
							graphicsContext.setPixelShaderTexture( 0u, &pTexture->getTextureData() );
						}
					}
					else if ( command.pRenderEffectData->defaultTextureOffset != RenderEffectDataInvalidTextureOffset )
					{
						const Texture* pDefaultTexture = addPtrCast< const Texture >( command.pRenderEffectData, command.pRenderEffectData->defaultTextureOffset );

						if ( pDefaultTexture != nullptr )
						{
							graphicsContext.setPixelShaderTexture( 0u, &pDefaultTexture->getTextureData() );
						}
					}
					else
					{
						graphicsContext.setPixelShaderTexture( 0u, nullptr );
					}
				}

				geometry.render( graphicsContext );
			}
		} 
	}

}