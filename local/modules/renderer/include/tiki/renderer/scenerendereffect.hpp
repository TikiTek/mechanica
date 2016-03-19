#pragma once
#ifndef __TIKI_SCENERENDEREFFECT_HPP_INCLUDED__
#define __TIKI_SCENERENDEREFFECT_HPP_INCLUDED__

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
	class VertexFormat;
	class VertexInputBinding;

	class SceneRenderEffect : public RenderEffect
	{
		TIKI_NONCOPYABLE_CLASS( SceneRenderEffect );

	public:

								SceneRenderEffect();
		virtual					~SceneRenderEffect();

		virtual RenderEffectId	getRenderEffectId() const  TIKI_OVERRIDE TIKI_FINAL { return RenderEffectId_Scene; }

	protected:

		virtual bool			createInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool ) TIKI_OVERRIDE TIKI_FINAL;
		virtual bool			createShaderResourcesInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool ) TIKI_OVERRIDE TIKI_FINAL;
		virtual void			disposeInternal( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourceRequestPool ) TIKI_OVERRIDE TIKI_FINAL;

		virtual void			executeRenderSequencesInternal( GraphicsContext& graphicsContext, RenderPass pass, const RenderSequence* pSequences, uint sequenceCount, const RenderView& view, const RendererContext& rendererContext ) TIKI_OVERRIDE TIKI_FINAL;

	private:

		enum ModelType
		{
			ModelType_Static,
			ModelType_Skinned,

			ModelType_Count
		};

		const ShaderSet*			m_pShader;

		const BlendState*			m_pBlendState;
		const DepthStencilState*	m_pDepthStencilState;
		const RasterizerState*		m_pRasterizerState;
		const SamplerState*			m_pSampler;

		const VertexFormat*			m_pVertexFormat[ ModelType_Count ];
		const VertexInputBinding*	m_pVertexInputBinding[ ModelType_Count ];

		ConstantBuffer				m_vertexConstantBuffer;
		ConstantBuffer				m_pixelConstantBuffer;

	};
}

#endif // __TIKI_SCENERENDEREFFECT_HPP_INCLUDED__
