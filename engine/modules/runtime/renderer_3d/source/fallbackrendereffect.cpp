
#include "tiki/renderer/fallbackrendereffect.hpp"

#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/graphicstypes.hpp"
#include "tiki/graphics/material.hpp"
#include "tiki/graphics/modelgeometry.hpp"
#include "tiki/graphics/samplerstate.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/renderer/rendercommand.hpp"
#include "tiki/renderer/renderercontext.hpp"
#include "tiki/renderer/renderview.hpp"
#include "tiki/resource/resourcerequestpool.hpp"

#include "renderer.hpp"

namespace tiki
{
	struct FallbackVertexConstants
	{
		GraphicsMatrix44	mvpMatrix;
		GraphicsMatrix44	modelViewMatrix;
	};

	FallbackRenderEffect::FallbackRenderEffect()
	{
		m_pGraphicsSystem		= nullptr;

		m_pShader				= nullptr;

		m_pBlendState			= nullptr;
		m_pDepthStencilState	= nullptr;
		m_pRasterizerState		= nullptr;
		m_pSampler				= nullptr;
	}

	FallbackRenderEffect::~FallbackRenderEffect()
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );

		TIKI_ASSERT( m_pShader == nullptr );

		TIKI_ASSERT( m_pBlendState == nullptr );
		TIKI_ASSERT( m_pDepthStencilState == nullptr );
		TIKI_ASSERT( m_pRasterizerState == nullptr );
		TIKI_ASSERT( m_pSampler == nullptr );
	}

	bool FallbackRenderEffect::createInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool )
	{
		m_pGraphicsSystem		= &graphicsSystem;

		m_pBlendState			= graphicsSystem.createBlendState( false, Blend_One, Blend_Zero, BlendOperation_Add, ColorWriteMask_All );
		m_pDepthStencilState	= graphicsSystem.createDepthStencilState( true, true );
		m_pRasterizerState		= graphicsSystem.createRasterizerState( FillMode_Solid, CullMode_Back, WindingOrder_Clockwise );
		m_pSampler				= graphicsSystem.createSamplerState( AddressMode_Clamp, AddressMode_Clamp, AddressMode_Clamp, FilterMode_Linear, FilterMode_Linear );

		m_vertexConstantBuffer.create( graphicsSystem, sizeof( FallbackVertexConstants ), "FallbackVertexConstants" );

		if ( !m_vertexInputBindings.create( 32u ) )
		{
			dispose( graphicsSystem, resourceRequestPool );
			return false;
		}

		resourceRequestPool.beginLoadResource< ShaderSet >( &m_pShader, "fallback.shader" );

		return true;
	}

	void FallbackRenderEffect::disposeInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool )
	{
		graphicsSystem.disposeBlendState( m_pBlendState );
		m_pBlendState = nullptr;

		graphicsSystem.disposeDepthStencilState( m_pDepthStencilState );
		m_pDepthStencilState = nullptr;

		graphicsSystem.disposeRasterizerState( m_pRasterizerState );
		m_pRasterizerState = nullptr;

		graphicsSystem.disposeSamplerState( m_pSampler );
		m_pSampler = nullptr;

		m_vertexConstantBuffer.dispose( graphicsSystem );

		for (uint i = 0u; i < m_vertexInputBindings.getCount(); ++i)
		{
			graphicsSystem.disposeVertexInputBinding(
				m_vertexInputBindings.getValueAt( i )
			);
		}
		m_vertexInputBindings.dispose();

		resourceRequestPool.unloadResource( m_pShader );

		m_pGraphicsSystem = nullptr;
	}

	void FallbackRenderEffect::executeRenderSequencesInternal( GraphicsContext& graphicsContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount, const RenderView& view, const RendererContext& rendererContext )
	{
		const Shader* pVertexShader = m_pShader->getShader( ShaderType_VertexShader, 0u );

		graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleList );
		graphicsContext.setBlendState( m_pBlendState );
		graphicsContext.setDepthStencilState( m_pDepthStencilState );
		graphicsContext.setRasterizerState( m_pRasterizerState );

		graphicsContext.setVertexShader( pVertexShader );
		graphicsContext.setPixelShader( m_pShader->getShader( ShaderType_PixelShader, 0u ) );

		graphicsContext.setVertexShaderConstant( 0u, m_vertexConstantBuffer );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSampler );

		for (uint sequnceIndex = 0u; sequnceIndex < sequenceCount; ++sequnceIndex)
		{
			const RenderSequence& sequence = pSequences[ sequnceIndex ];

			for (uint commandIndex = 0u; commandIndex < sequence.commandCount; ++commandIndex)
			{
				const RenderCommand& command = sequence.pCommands[ commandIndex ];

				Matrix44 mvMtx;
				matrix::set( mvMtx, command.worldTransform );
				matrix::mul( mvMtx, view.getCamera().getViewMatrix() );

				Matrix44 mvpMtx;
				matrix::set( mvpMtx, command.worldTransform );
				matrix::mul( mvpMtx, view.getCamera().getViewProjectionMatrix() );

				FallbackVertexConstants* pVertexConstants = static_cast< FallbackVertexConstants* >( graphicsContext.mapBuffer( m_vertexConstantBuffer ) );
				TIKI_ASSERT( pVertexConstants != nullptr );
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
					else if ( command.pRenderEffectData->defaultTextureOffset != RenderEffectValues_InvalidTextureOffset )
					{
						const Texture* pDefaultTexture = *addPointerCast< const Texture* >( command.pRenderEffectData, command.pRenderEffectData->defaultTextureOffset );

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