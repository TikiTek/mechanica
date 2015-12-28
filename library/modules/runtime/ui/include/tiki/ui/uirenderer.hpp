#pragma once
#ifndef TIKI_UIRENDERER_HPP_INCLUDED
#define TIKI_UIRENDERER_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/pool.hpp"
#include "tiki/graphics/constantbuffer.hpp"
#include "tiki/graphics/pixelformat.hpp"
#include "tiki/graphics/rendertarget.hpp"

namespace tiki
{
	class BlendState;
	class DepthStencilState;
	class GraphicsContext;
	class RasterizerState;
	class SamplerState;
	class ShaderSet;
	struct UiRenderData;
	struct UiRenderElement;

	struct UiRendererParameters
	{
		uint		width;
		uint		height;
		PixelFormat	targetFormat;
	};

	class UiRenderer
	{
		TIKI_NONCOPYABLE_CLASS( UiRenderer );

	public:

					UiRenderer();
					~UiRenderer();

		bool		create( const UiRendererParameters& parameters );
		void		dispose();

		void		update( const UiRenderData& renderData );
		void		render( GraphicsContext& context ) const;

	private:

		RenderTarget				m_renderTarget;

		const ShaderSet*			m_pShader;

		const BlendState*			m_pBlendState;
		const DepthStencilState*	m_pDepthStencilState;
		const RasterizerState*		m_pRasterizerState;
		const SamplerState*			m_pSampler;

		ConstantBuffer				m_vertexConstantBuffer;

		Pool< UiRenderElement >		m_renderElements;

	};
}

#endif // TIKI_UIRENDERER_HPP_INCLUDED
