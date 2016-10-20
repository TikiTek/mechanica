#pragma once
#ifndef __TIKI_RENDEREFFECTSYSTEM_HPP_INCLUDED__
#define __TIKI_RENDEREFFECTSYSTEM_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/container/fixedarray.hpp"

#include "renderer.hpp"

namespace tiki
{
	class GraphicsContext;
	class GraphicsSystem;
	class RenderBatch;
	class RenderEffect;
	class RenderView;
	class ResourceRequestPool;
	struct RendererContext;

	class RenderEffectSystem
	{
		TIKI_NONCOPYABLE_CLASS( RenderEffectSystem );

	public:

						RenderEffectSystem();
						~RenderEffectSystem();

		bool			create();
		bool			createShaderResourcees( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool );
		void			dispose();

		void			registerRenderEffect( RenderEffect* pRenderEffect );
		void			unregisterRenderEffect( RenderEffect* pRenderEffect );

		void			renderView( GraphicsContext& graphicsContext, const RendererContext& context, const RenderView& view, RenderPass pass ) const;

	private:

		typedef	FixedArray< RenderEffect*, RenderEffectId_Count > RenderEffectArray;
		
		RenderEffectArray		m_effects;

	};
}

#endif // __TIKI_RENDEREFFECTSYSTEM_HPP_INCLUDED__
