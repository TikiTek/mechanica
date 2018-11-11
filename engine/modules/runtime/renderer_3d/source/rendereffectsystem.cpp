
#include "tiki/renderer/rendereffectsystem.hpp"

#include "tiki/renderer/renderbatch.hpp"
#include "tiki/renderer/rendereffect.hpp"
#include "tiki/renderer/renderscene.hpp"
#include "tiki/renderer/renderview.hpp"

#include "renderer.hpp"

namespace tiki
{
	RenderEffectSystem::RenderEffectSystem()
	{
		for (uint i = 0u; i < m_effects.getCount(); ++i)
		{
			m_effects[ i ] = nullptr;
		} 
	}

	RenderEffectSystem::~RenderEffectSystem()
	{
	}

	bool RenderEffectSystem::create()
	{
		return true;
	}

	bool RenderEffectSystem::createShaderResourcees( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool )
	{
		for (uint i = 0u; i < m_effects.getCount(); ++i)
		{
			if ( m_effects[ i ] != nullptr )
			{
				if ( !m_effects[ i ]->createShaderResources( graphicsSystem, resourceRequestPool ) )
				{
					return false;
				}
			}
		}

		return true;
	}

	void RenderEffectSystem::dispose()
	{
		for (uint i = 0u; i < m_effects.getCount(); ++i)
		{
			m_effects[ i ] = nullptr;
		} 
	}

	void RenderEffectSystem::registerRenderEffect( RenderEffect* pRenderEffect )
	{
		TIKI_ASSERT( pRenderEffect != nullptr );

		const RenderEffectId renderEffectId = pRenderEffect->getRenderEffectId();
		m_effects[ renderEffectId ] = pRenderEffect;
	}
	
	void RenderEffectSystem::unregisterRenderEffect( RenderEffect* pRenderEffect )
	{
		TIKI_ASSERT( pRenderEffect != nullptr );

		const RenderEffectId renderEffectId = pRenderEffect->getRenderEffectId();
		m_effects[ renderEffectId ] = nullptr;
	}

	void RenderEffectSystem::renderView( GraphicsContext& graphicsContext, const RendererContext& context, const RenderView& view, RenderPass pass ) const
	{
		RenderSequenceEnumerator enumerator = view.getScene().getRenderBatch().createEnumerator( pass );
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

			pRenderEffect->executeRenderSequences( graphicsContext, context, view, pass, pSequence, 1u );
		} 
	}
}