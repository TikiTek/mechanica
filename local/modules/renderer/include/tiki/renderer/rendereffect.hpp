#pragma once
#ifndef __TIKI_RENDEREFFECT_HPP_INCLUDED__
#define __TIKI_RENDEREFFECT_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/renderer/rendererdefinition.hpp"

namespace tiki
{
	class GraphicsContext;
	struct FrameData;
	struct RenderSequence;
	struct RendererContext;

	class RenderEffect
	{
		TIKI_NONCOPYABLE_CLASS( RenderEffect );

	public:

								RenderEffect();
		virtual					~RenderEffect();

		bool					create( const RendererContext& rendererContext );
		void					dispose();

		virtual RenderEffectId	getRenderEffectId() const = 0;

		void					setFrameData( const FrameData& frameData );
		void					executeRenderSequences( GraphicsContext& graphisContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount );

	protected:

		virtual bool			createInternal() = 0;
		virtual void			disposeInternal() = 0;

		virtual void			executeRenderSequencesInternal( GraphicsContext& graphisContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount, const FrameData& frameData, const RendererContext& rendererContext ) = 0;

	private:

		const FrameData*		m_pFrameData;
		const RendererContext*	m_pRendererContext;

	};
}

#endif // __TIKI_RENDEREFFECT_HPP_INCLUDED__
