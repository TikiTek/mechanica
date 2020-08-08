#pragma once
#ifndef __TIKI_RENDEREFFECT_HPP_INCLUDED__
#define __TIKI_RENDEREFFECT_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#include "renderer.hpp"

namespace tiki
{
	class GraphicsContext;
	class GraphicsSystem;
	class RenderView;
	class ResourceRequestPool;
	struct RenderSequence;
	struct RendererContext;

	class RenderEffect
	{
		TIKI_NONCOPYABLE_CLASS( RenderEffect );

	public:

								RenderEffect();
		virtual					~RenderEffect();

		bool					create( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool );
		bool					createShaderResources( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool );
		void					dispose( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool );
		
		virtual RenderEffectId	getRenderEffectId() const TIKI_PURE;

		void					executeRenderSequences( GraphicsContext& graphisContext, const RendererContext& rendererContext, const RenderView& view, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount );

	protected:

		virtual bool			createInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool ) TIKI_PURE;
		virtual bool			createShaderResourcesInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool ) { return true; }
		virtual void			disposeInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool ) TIKI_PURE;

		virtual void			executeRenderSequencesInternal( GraphicsContext& graphisContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount, const RenderView& view, const RendererContext& rendererContext ) TIKI_PURE;

	};
}

#endif // __TIKI_RENDEREFFECT_HPP_INCLUDED__
