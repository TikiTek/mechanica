#pragma once
#ifndef __TIKI_FALLBACKRENDEREFFECT_HPP_INCLUDED__
#define __TIKI_FALLBACKRENDEREFFECT_HPP_INCLUDED__

#include "tiki/renderer/rendereffect.hpp"

#include "tiki/base/types.hpp"
#include "tiki/container/sortedsizedmap.hpp"
#include "tiki/graphics/constantbuffer.hpp"


namespace tiki
{
	class BlendState;
	class DepthStencilState;
	class RasterizerState;
	class SamplerState;
	class ShaderSet;
	class VertexInputBinding;

	class FallbackRenderEffect : public RenderEffect
	{
		TIKI_NONCOPYABLE_CLASS( FallbackRenderEffect );

	public:

								FallbackRenderEffect();
		virtual					~FallbackRenderEffect();

		virtual RenderEffectId	getRenderEffectId() const TIKI_OVERRIDE TIKI_FINAL { return RenderEffectId_Fallback; }

	protected:

		virtual bool			createInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void			disposeInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool ) TIKI_OVERRIDE TIKI_FINAL;

		virtual void			executeRenderSequencesInternal( GraphicsContext& graphicsContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount, const RenderView& view, const RendererContext& rendererContext ) TIKI_OVERRIDE TIKI_FINAL;

	private:

		typedef SortedSizedMap< crc32, const VertexInputBinding* > VertexInputBindingMap;

		GraphicsSystem*				m_pGraphicsSystem;

		const ShaderSet*			m_pShader;

		const BlendState*			m_pBlendState;
		const DepthStencilState*	m_pDepthStencilState;
		const RasterizerState*		m_pRasterizerState;
		const SamplerState*			m_pSampler;

		VertexInputBindingMap		m_vertexInputBindings;

		ConstantBuffer				m_vertexConstantBuffer;

	};
}

#endif // __TIKI_FALLBACKRENDEREFFECT_HPP_INCLUDED__
