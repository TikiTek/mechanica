#pragma once

#include "tiki/base/types.hpp"
#include "tiki/graphics/constant_buffer.hpp"
#include "tiki/graphics/pixel_format.hpp"
#include "tiki/graphics/render_target.hpp"
#include "tiki/graphics/texture_data.hpp"

namespace tiki
{
	class BlendState;
	class DepthStencilState;
	class GraphicsContext;
	class GraphicsSystem;
	class RasterizerState;
	class ResourceRequestPool;
	class SamplerState;
	class ShaderSet;
	class VertexInputBinding;

	class PostProcessBlur
	{
		TIKI_NONCOPYABLE_CLASS( PostProcessBlur );

	public:

				PostProcessBlur();
				~PostProcessBlur();

		bool	create( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool, uint maxWidth, uint maxHeight, PixelFormat format );
		bool	createResources( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool );
		void	dispose( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool );

		bool	resize( GraphicsSystem& graphicsSystem, uint width, uint height );

		void	render( GraphicsContext& graphicsContext, const TextureData& sourceData, const RenderTarget& target ) const;

	private:

		const ShaderSet*			m_pShader;

		PixelFormat					m_format;

		const BlendState*			m_pBlendState;
		const DepthStencilState*	m_pDepthState;
		const RasterizerState*		m_pRasterizerState;
		const SamplerState*			m_pSamplerState;

		const VertexInputBinding*	m_pInputBinding;

		TextureData					m_textureData;
		RenderTarget				m_renderTarget;

		ConstantBuffer				m_pixelConstants;

		bool						createRenderTargets( GraphicsSystem& graphicsSystem, uint width, uint height );
		void						disposeRenderTargets( GraphicsSystem& graphicsSystem );
	};
}
