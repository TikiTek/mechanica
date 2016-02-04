#pragma once
#ifndef __TIKI_POSTBLOOM_HPP_INCLUDED__
#define __TIKI_POSTBLOOM_HPP_INCLUDED__

#include "tiki/container/array.hpp"
#include "tiki/graphics/rendertarget.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/renderer/postblur.hpp"

namespace tiki
{
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

		bool				create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const PostProcessBloomParameters& parameters );
		void				dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );
		
		bool				resize( GraphicsSystem& graphicsSystem, uint width, uint height, uint passCount = TIKI_SIZE_T_MAX );

		void				render( GraphicsContext& graphicsContext, const TextureData& accumulationData, const TextureData& selfilluData ) const;

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

#endif // __TIKI_POSTBLOOM_HPP_INCLUDED__
