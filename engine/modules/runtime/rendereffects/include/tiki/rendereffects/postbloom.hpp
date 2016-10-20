#pragma once
#ifndef TIKI_POSTBLOOM_HPP_INCLUDED
#define TIKI_POSTBLOOM_HPP_INCLUDED

#include "tiki/container/array.hpp"
#include "tiki/graphics/rendertarget.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/rendereffects/postblur.hpp"

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

		uint	width;
		uint	height;
		uint	passCount;
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
		
		bool				resize( GraphicsSystem& graphicsSystem, uint width, uint height, uint passCount = TIKI_SIZE_T_MAX );

		void				render( GraphicsContext& graphicsContext, const TextureData& sourceData, const TextureData* pSelfIlluminationData ) const;

		const TextureData&	getResultData() const;
		const TextureData&	getResultData( uint index ) const { return m_textures[ index ]; }

	private:

		const ShaderSet*			m_pShader;

		PostProcessBlur				m_blur;

		uint						m_width;
		uint						m_height;
		uint						m_passCount;

		const BlendState*			m_pBlendStateCuroff;
		const BlendState*			m_pBlendStateAdd;
		const DepthStencilState*	m_pDepthState;
		const RasterizerState*		m_pRasterizerState;
		const SamplerState*			m_pSamplerState;

		const VertexInputBinding*	m_pInputBinding;
		
		Array< TextureData >		m_textures;
		Array< RenderTarget >		m_renderTargets;

		bool						createRenderTargets( GraphicsSystem& graphicsSystem );
		void						disposeRenderTargets( GraphicsSystem& graphicsSystem );

	};
}

#endif // TIKI_POSTBLOOM_HPP_INCLUDED
