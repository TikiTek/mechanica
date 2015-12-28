#pragma once
#ifndef TIKI_UIRENDERER_HPP_INCLUDED
#define TIKI_UIRENDERER_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/chunkstorage.hpp"
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
	struct UiElement;
	struct UiRenderData;
	struct UiRenderElement;

	struct UiRendererParameters
	{
		UiRendererParameters()
		{
			width				= 0u;
			height				= 0u;
			targetFormat		= PixelFormat_Color;

			maxRenderElements	= 4096u;
		}

		uint		width;
		uint		height;
		PixelFormat	targetFormat;

		uint		maxRenderElements;
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

		RenderTarget					m_renderTarget;

		const ShaderSet*				m_pShader;

		const BlendState*				m_pBlendState;
		const DepthStencilState*		m_pDepthStencilState;
		const RasterizerState*			m_pRasterizerState;
		const SamplerState*				m_pSampler;

		ConstantBuffer					m_vertexConstantBuffer;

		ChunkStorage					m_renderElements;
		ChunkTypeId						m_colorRectangleId;
		ChunkTypeId						m_textureRectangleId;
		ChunkTypeId						m_textId;

		void							updateRecursiveRenderTree( const UiElement& element );

	};
}

#endif // TIKI_UIRENDERER_HPP_INCLUDED
