#pragma once
#ifndef __TIKI_RENDEREFFECT_HPP_INCLUDED__
#define __TIKI_RENDEREFFECT_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/renderer/rendererdefinition.hpp"

namespace tiki
{
	class GraphicsContext;
	class GraphicsSystem;
	class ResourceManager;
	struct FrameData;
	struct RenderSequence;
	struct RendererContext;

	class RenderEffect
	{
		TIKI_NONCOPYABLE_CLASS( RenderEffect );

	public:

								RenderEffect();
		virtual					~RenderEffect();

		bool					create( const RendererContext& rendererContext, GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );
		void					dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

		virtual RenderEffectId	getRenderEffectId() const = 0;

		void					setFrameData( const FrameData& frameData );
		void					executeRenderSequences( GraphicsContext& graphisContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount );

	protected:

		virtual bool			createInternal( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager ) = 0;
		virtual void			disposeInternal( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager ) = 0;

		virtual void			executeRenderSequencesInternal( GraphicsContext& graphisContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount, const FrameData& frameData, const RendererContext& rendererContext ) = 0;

	private:

		const FrameData*		m_pFrameData;
		const RendererContext*	m_pRendererContext;

	};
}

#endif // __TIKI_RENDEREFFECT_HPP_INCLUDED__
