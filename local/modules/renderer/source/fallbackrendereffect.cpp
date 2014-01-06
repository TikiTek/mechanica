
#include "tiki/renderer/fallbackrendereffect.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/samplerstate.hpp"
#include "tiki/graphicsbase/graphicstypes.hpp"
#include "tiki/graphicsresources/material.hpp"
#include "tiki/graphicsresources/modelgeometry.hpp"
#include "tiki/graphicsresources/shaderset.hpp"
#include "tiki/graphicsresources/texture.hpp"
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

	struct FallbackMaterialData : public MaterialBaseData
	{
		ResRef< Texture >	defaultTexture;
	};


	FallbackRenderEffect::FallbackRenderEffect()
	{
		m_pShaderSet	= nullptr;
		m_pSampler		= nullptr;
	}

	FallbackRenderEffect::~FallbackRenderEffect()
	{
		TIKI_ASSERT( m_pShaderSet == nullptr );
		TIKI_ASSERT( m_pSampler == nullptr );
	}

	bool FallbackRenderEffect::createInternal()
	{
		m_pShaderSet = framework::getResourceManager().loadResource< ShaderSet >( "fallback.shader" );

		m_pSampler = framework::getGraphicsSystem().createSamplerState(
			AddressMode_Clamp,
			AddressMode_Clamp,
			AddressMode_Clamp,
			FilterMode_Linear,
			FilterMode_Linear
		);

		m_vertexConstantBuffer.create( framework::getGraphicsSystem(), sizeof( FallbackVertexConstants ) );

		m_vertexInputBindings.create( 32u );

		return true;
	}

	void FallbackRenderEffect::disposeInternal()
	{
		framework::getGraphicsSystem().disposeSamplerState( m_pSampler );
		m_pSampler = nullptr;

		framework::getResourceManager().unloadResource< ShaderSet >( m_pShaderSet );
		m_pShaderSet = nullptr;

		m_vertexConstantBuffer.dispose( framework::getGraphicsSystem() );

		for (uint i = 0u; i < m_vertexInputBindings.getCount(); ++i)
		{
			framework::getGraphicsSystem().disposeVertexInputBinding(
				m_vertexInputBindings.getValueAt( i )
			);
		}
		m_vertexInputBindings.dispose();
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
					pVertexInputBinding = framework::getGraphicsSystem().createVertexInputBinding(
						pVertexShader,
						geometry.getVertexFormat()
					);
					m_vertexInputBindings.set( vertexFormatHash, pVertexInputBinding );
				}

				graphicsContext.setVertexInputBinding( pVertexInputBinding );

				if ( command.pMaterialData != nullptr )
				{
					if ( command.pMaterialData->renderEffectId == RenderEffectId_Fallback )
					{
						const FallbackMaterialData* pMaterialData = static_cast< const FallbackMaterialData* >( command.pMaterialData );
						const Texture* pTexture = pMaterialData->defaultTexture.getData();

						if ( pTexture != nullptr )
						{
							graphicsContext.setPixelShaderTexture( 0u, &pTexture->getTextureData() );
						}
					}
					else if ( command.pMaterialData->defaultTextureOffset != MaterialBaseData::InvalidTextureOffset )
					{
						const Texture* pDefaultTexture = addPtrCast< const Texture >( command.pMaterialData, command.pMaterialData->defaultTextureOffset );

						if ( pDefaultTexture != nullptr )
						{
							graphicsContext.setPixelShaderTexture( 0u, &pDefaultTexture->getTextureData() );
						}
					}
				}

				geometry.render( graphicsContext );
			}
		} 
	}

}