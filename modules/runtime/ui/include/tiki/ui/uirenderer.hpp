#pragma once
#ifndef TIKI_UIRENDERER_HPP_INCLUDED
#define TIKI_UIRENDERER_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/sizedarray.hpp"
#include "tiki/graphics/constantbuffer.hpp"
#include "tiki/math/projection.hpp"

namespace tiki
{
	class BlendState;
	class DepthStencilState;
	class GraphicsContext;
	class GraphicsSystem;
	class RasterizerState;
	class RenderTarget;
	class ResourceManager;
	class SamplerState;
	class ShaderSet;
	class UiElement;
	class VertexFormat;
	class VertexInputBinding;
	struct UiRenderData;
	struct UiRenderElement;

	struct UiRendererParameters
	{
		UiRendererParameters()
		{
			maxRenderElements	= 4096u;
		}

		uint		maxRenderElements;
	};

	class UiRenderer
	{
		TIKI_NONCOPYABLE_CLASS( UiRenderer );

	public:

					UiRenderer();
					~UiRenderer();

		bool		create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const UiRendererParameters& parameters );
		void		dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

		void		update( const UiRenderData& renderData );
		void		render( GraphicsContext& context, const RenderTarget& renderTarget ) const;

		void		setScreenSize( float width, float height );

	private:

		const ShaderSet*				m_pShader;

		const VertexFormat*				m_pVertexFormat;
		const VertexInputBinding*		m_pVertexInputBinding;

		const BlendState*				m_pBlendState;
		const DepthStencilState*		m_pDepthStencilState;
		const RasterizerState*			m_pRasterizerState;
		const SamplerState*				m_pSamplerState;

		ConstantBuffer					m_vertexConstantBuffer;

		Projection						m_projection;

		SizedArray< UiRenderElement >	m_renderElements;

		void							updateRecursiveRenderTree( const UiElement& element );

	};
}

#endif // TIKI_UIRENDERER_HPP_INCLUDED
