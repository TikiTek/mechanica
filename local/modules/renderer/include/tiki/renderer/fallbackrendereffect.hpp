#pragma once
#ifndef __TIKI_FALLBACKRENDEREFFECT_HPP_INCLUDED__
#define __TIKI_FALLBACKRENDEREFFECT_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/renderer/rendereffect.hpp"

namespace tiki
{
	class FallbackRenderEffect : public RenderEffect
	{
		TIKI_NONCOPYABLE_CLASS( FallbackRenderEffect );

	public:

		FallbackRenderEffect();
		~FallbackRenderEffect();

		virtual RenderEffectId	getRenderEffectId() const { return RenderEffectId_Fallback; }

	protected:

		virtual bool			createInternal();
		virtual void			disposeInternal();

		virtual void			executeRenderSequencesInternal( GraphicsContext& graphicsContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount );

	private:

		const ShaderSet*		m_pShaderSet;

		const SamplerState*		m_pSampler;

		ConstantBuffer			m_vertexConstantBuffer;

	};
}

#endif // __TIKI_FALLBACKRENDEREFFECT_HPP_INCLUDED__
