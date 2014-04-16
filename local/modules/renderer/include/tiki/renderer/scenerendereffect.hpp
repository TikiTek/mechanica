#pragma once
#ifndef __TIKI_SCENERENDEREFFECT_HPP_INCLUDED__
#define __TIKI_SCENERENDEREFFECT_HPP_INCLUDED__

#include "tiki/renderer/rendereffect.hpp"

#include "tiki/base/sortedsizedmap.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/constantbuffer.hpp"


namespace tiki
{
	class BlendState;
	class DepthStencilState;
	class RasterizerState;
	class SamplerState;
	class ShaderSet;
	class VertexInputBinding;

	class SceneRenderEffect : public RenderEffect
	{
		TIKI_NONCOPYABLE_CLASS( SceneRenderEffect );

	public:

		SceneRenderEffect();
		virtual					~SceneRenderEffect();

		virtual RenderEffectId	getRenderEffectId() const { return RenderEffectId_Scene; }

	protected:

		virtual bool			createInternal( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );
		virtual void			disposeInternal( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

		virtual void			executeRenderSequencesInternal( GraphicsContext& graphicsContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount, const FrameData& frameData, const RendererContext& rendererContext );

	private:

		const ShaderSet*			m_pShader;

		const BlendState*			m_pBlendState;
		const DepthStencilState*	m_pDepthStencilState;
		const RasterizerState*		m_pRasterizerState;
		const SamplerState*			m_pSampler;

		const VertexFormat*			m_pVertexFormat;
		const VertexInputBinding*	m_pVertexInputBinding;

		ConstantBuffer				m_vertexConstantBuffer;

	};
}

#endif // __TIKI_SCENERENDEREFFECT_HPP_INCLUDED__
