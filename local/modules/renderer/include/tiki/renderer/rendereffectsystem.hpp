#pragma once
#ifndef __TIKI_RENDEREFFECTSYSTEM_HPP_INCLUDED__
#define __TIKI_RENDEREFFECTSYSTEM_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/base/fixedarray.hpp"
#include "tiki/renderer/rendererdefinition.hpp"

namespace tiki
{
	class GraphicsContext;
	class RenderBatch;
	class RenderEffect;
	struct FrameData;
	struct RendererContext;

	class RenderEffectSystem
	{
		TIKI_NONCOPYABLE_CLASS( RenderEffectSystem );

	public:

		RenderEffectSystem();
		~RenderEffectSystem();

		bool			create( const RendererContext& rendererContext );
		void			dispose();

		void			registerRenderEffect( RenderEffect* pRenderEffect );

		void			setFrameData( const FrameData& frameData );
		void			render( GraphicsContext& graphicsContext, RenderPass pass, const RenderBatch& batch ) const;

	private:

		typedef	FixedArray< RenderEffect*, RenderEffectId_Count > RenderEffectArray;
		
		const RendererContext*	m_pRendererContext;

		RenderEffectArray		m_effects;

	};
}

#endif // __TIKI_RENDEREFFECTSYSTEM_HPP_INCLUDED__
