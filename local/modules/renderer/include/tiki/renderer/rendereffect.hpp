#pragma once
#ifndef __TIKI_RENDEREFFECT_HPP_INCLUDED__
#define __TIKI_RENDEREFFECT_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#include "renderer.hpp"

namespace tiki
{
	class GraphicsContext;
	class GraphicsSystem;
	class ResourceRequestPool;
	struct FrameData;
	struct RenderSequence;
	struct RendererContext;

	class RenderEffect
	{
		TIKI_NONCOPYABLE_CLASS( RenderEffect );

	public:

								RenderEffect();
		virtual					~RenderEffect();

		bool					create( const RendererContext& rendererContext, GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool );
		bool					createShaderResources( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool );
		void					dispose( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool );
		
		virtual RenderEffectId	getRenderEffectId() const TIKI_PURE;

		void					setFrameData( const FrameData& frameData );
		void					executeRenderSequences( GraphicsContext& graphisContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount );

	protected:

		virtual bool			createInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool ) TIKI_PURE;
		virtual bool			createShaderResourcesInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool ) { return true; }
		virtual void			disposeInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool ) TIKI_PURE;

		virtual void			executeRenderSequencesInternal( GraphicsContext& graphisContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount, const FrameData& frameData, const RendererContext& rendererContext ) = 0;

	private:

		const FrameData*		m_pFrameData;
		const RendererContext*	m_pRendererContext;

	};
}

#endif // __TIKI_RENDEREFFECT_HPP_INCLUDED__
