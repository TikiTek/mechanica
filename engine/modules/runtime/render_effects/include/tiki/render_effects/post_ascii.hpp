#pragma once
#ifndef __TIKI_POSTASCII_HPP_INCLUDED__
#define __TIKI_POSTASCII_HPP_INCLUDED__

#include "tiki/graphics/constantbuffer.hpp"
#include "tiki/graphics/rendertarget.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/math/camera.hpp"

namespace tiki
{
	class BlendState;
	class DepthStencilState;
	class Font;
	class GraphicsContext;
	class RasterizerState;
	class RenderView;
	class ResourceManager;
	class SamplerState;
	class SamplerState;
	class ShaderSet;
	class VertexInputBinding;
	struct RendererContext;

	struct PostProcessAsciiParameters
	{
		PostProcessAsciiParameters()
		{
			resultDataWidth		= 1280u;
			resultDataHeight	= 720u;

			maxCharsWidth		= 105u;
			maxCharsHeight		= 60u;
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

		const TextureData&	getDownSampleData() const	{ return m_downSampleData; }
		const TextureData&	getResultData() const		{ return m_finalResultData; }

		void				render( GraphicsContext& graphicsContext, const Camera& camera, const TextureData* pOffscreenColorData, const TextureData* pOffscreenDepthData ) const;
		
	private:

		const Font*					m_pAsciiCharTexture;
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

		ConstantBuffer				m_pixelConstants;

	};
}

#endif // __TIKI_POSTASCII_HPP_INCLUDED__
