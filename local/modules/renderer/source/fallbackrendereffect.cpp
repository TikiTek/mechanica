
#include "tiki/renderer/fallbackrendereffect.hpp"

namespace tiki
{
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
	}

	void FallbackRenderEffect::disposeInternal()
	{
		framework::getGraphicsSystem().disposeSamplerState( m_pSampler );

		framework::getResourceManager().unloadResource< ShaderSet >( m_pShaderSet );
	}

	void FallbackRenderEffect::executeRenderSequencesInternal( GraphicsContext& graphicsContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount )
	{
		graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleList );

		graphicsContext.setVertexShader( m_pShaderSet->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 0u ) );

		graphicsContext.setVertexShaderConstant( 0u, m_vertexConstantBuffer );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSampler );

		for (uint sequnceIndex = 0u; sequnceIndex < sequenceCount; ++sequnceIndex)
		{
			const RenderSequence& sequence = pSequences[ sequnceIndex ];

			for (uint commandIndex = 0u; commandIndex < sequence.commandCount; ++commandIndex)
			{
				const RenderCommand& command = sequence.pCommands[ commandIndex ];

				FallbackVertexConstants* pVertexConstants = static_cast< FallbackVertexConstants* >( graphicsContext.mapBuffer( m_vertexConstantBuffer ) );
				createGraphicsMatrix44( pVertexConstants->mvpMatrix, mvp );
				createGraphicsMatrix44( pVertexConstants->modelViewMatrix, mv );
				graphicsContext.unmapBuffer( m_vertexConstantBuffer );
	
			}
		} 
	}

}