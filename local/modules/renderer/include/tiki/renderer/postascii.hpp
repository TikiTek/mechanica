#pragma once
#ifndef __TIKI_POSTASCII_HPP_INCLUDED__
#define __TIKI_POSTASCII_HPP_INCLUDED__

#include "tiki/graphics/rendertarget.hpp"
#include "tiki/graphics/texturedata.hpp"

namespace tiki
{
	class Texture;

	struct PostProcessAsciiParameters
	{
		PostProcessAsciiParameters()
		{
			resultDataWidth		= 1280u;
			resultDataHeight	= 720u;

			maxCharsWidth		= 128u;
			maxCharsHeight		= 72u;
		}

		uint	resultDataWidth;
		uint	resultDataHeight;

		uint	maxCharsWidth;
		uint	maxCharsHeight;
	};

	class PostProcessAscii
	{
		TIKI_NONCOPYABLE_CLASS( PostProcessAscii );

	public:

		PostProcessAscii();
		~PostProcessAscii();

		bool				create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const PostProcessAsciiParameters& parameters );
		void				dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

		const TextureData&	getResultData() const { return m_finalResultData; }

		void				render( GraphicsContext& graphicsContext, const TextureData& inputData );
		
	private:

		const Texture*				m_pAsciiCharTexture;
		const ShaderSet*			m_pShader;

		const VertexInputBinding*	m_pVertexInputBinding;

		const BlendState*			m_pBlendState;
		const DepthStencilState*	m_pDepthState;
		const RasterizerState*		m_pRasterizerState;
		const SamplerState*			m_pSamplerLinear;
		const SamplerState*			m_pSamplerNearest;

		TextureData					m_finalResultData;
		TextureData					m_downSampleData;

		RenderTarget				m_finalResultTarget;
		RenderTarget				m_downSampleTarget;

	};
}

#endif // __TIKI_POSTASCII_HPP_INCLUDED__
