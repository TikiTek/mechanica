
#include "tiki/renderer/rendereffectsystem.hpp"

#include "tiki/renderer/renderbatch.hpp"
#include "tiki/renderer/rendereffect.hpp"
#include "tiki/renderer/rendererdefinition.hpp"

namespace tiki
{
	RenderEffectSystem::RenderEffectSystem()
	{
		m_pRendererContext = nullptr;

		for (uint i = 0u; i < m_effects.getCount(); ++i)
		{
			m_effects[ i ] = nullptr;
		} 
	}

	RenderEffectSystem::~RenderEffectSystem()
	{
		TIKI_ASSERT( m_pRendererContext == nullptr );
	}

	bool RenderEffectSystem::create( const RendererContext& rendererContext )
	{
		TIKI_ASSERT( m_pRendererContext == nullptr );
		m_pRendererContext = &rendererContext;

		return true;
	}

	void RenderEffectSystem::dispose()
	{
		m_pRendererContext = nullptr;

		for (uint i = 0u; i < m_effects.getCount(); ++i)
		{
			m_effects[ i ] = nullptr;
		} 
	}

	void RenderEffectSystem::registerRenderEffect( RenderEffect* pRenderEffect )
	{
		TIKI_ASSERT( m_pRendererContext != nullptr );
		TIKI_ASSERT( pRenderEffect != nullptr );

		const RenderEffectId renderEffectId = pRenderEffect->getRenderEffectId();
		m_effects[ renderEffectId ] = pRenderEffect;
	}

	void RenderEffectSystem::setFrameData( const FrameData& frameData )
	{
		for (uint i = 0u; i < m_effects.getCount(); ++i)
		{
			if ( m_effects[ i ] != nullptr )
			{
				m_effects[ i ]->setFrameData( frameData );
			}
		} 
	}

	void RenderEffectSystem::render( GraphicsContext& graphicsContext, RenderPass pass, const RenderBatch& batch ) const
	{
		TIKI_ASSERT( m_pRendererContext != nullptr );

		RenderSequenceEnumerator enumerator = batch.createEnumerator( pass );
		while ( true )
		{
			const RenderSequence* pSequence = enumerator.enumerate();

			if ( pSequence == nullptr )
			{
				return;
			}
			
			RenderEffect* pRenderEffect = nullptr;
			if ( m_effects[ pSequence->renderEffectId ] != nullptr )
			{
				pRenderEffect = m_effects[ pSequence->renderEffectId ];
			}
			else if ( m_effects[ RenderEffectId_Fallback ] != nullptr )
			{
				pRenderEffect = m_effects[ RenderEffectId_Fallback ];
			}

			if ( pRenderEffect == nullptr )
			{
				TIKI_TRACE_WARNING( "[rendereffectsystem] RenderEffect with id '%u' is not registed.\n", pSequence->renderEffectId );
				continue;
			}

			pRenderEffect->executeRenderSequences( graphicsContext, pass, pSequence, 1u );
		} 
	}
}