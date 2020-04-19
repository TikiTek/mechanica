#pragma once

#include "tiki/container/array.hpp"
#include "tiki/graphics/render_target.hpp"
#include "tiki/graphics/texture_data.hpp"
#include "tiki/render_effects/post_blur.hpp"

namespace tiki
{
	class ResourceRequestPool;

	struct PostProcessBloomParameters
	{
		PostProcessBloomParameters()
		{
			width		= 0u;
			height		= 0u;
			passCount	= 0u;
		}

		uintreg	width;
		uintreg	height;
		uintreg	passCount;
	};

	struct PostProcessBloomRenderParameters
	{
		const TextureData*	pSourceData;
		const TextureData*	pEmissivData;
		Vector3				cutoffThreshold;
	};

	class PostProcessBloom
	{
		TIKI_NONCOPYABLE_CLASS( PostProcessBloom );

	public:

							PostProcessBloom();
							~PostProcessBloom();

		bool				create( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool, const PostProcessBloomParameters& parameters );
		bool				createResources( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool );
		void				dispose( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool );

		bool				resize( GraphicsSystem& graphicsSystem, uintreg width, uintreg height, uintreg passCount = TIKI_SIZE_T_MAX );

		void				render( GraphicsContext& graphicsContext, const PostProcessBloomRenderParameters& parameters ) const;

		const TextureData&	getResultData() const;
		const TextureData&	getResultData( uintreg index ) const { return m_textures[ index ]; }

	private:

		const ShaderSet*			m_pShader;

		PostProcessBlur				m_blur;

		uintreg						m_width;
		uintreg						m_height;
		uintreg						m_passCount;

		const BlendState*			m_pBlendStateCutoff;
		const BlendState*			m_pBlendStateAdd;
		const DepthStencilState*	m_pDepthState;
		const RasterizerState*		m_pRasterizerState;
		const SamplerState*			m_pSamplerState;

		const VertexInputBinding*	m_pInputBinding;

		ConstantBuffer				m_cutoffPixelConstants;
		Array< TextureData >		m_textures;
		Array< RenderTarget >		m_renderTargets;

		bool						createRenderTargets( GraphicsSystem& graphicsSystem );
		void						disposeRenderTargets( GraphicsSystem& graphicsSystem );
	};
}
