#pragma once
#ifndef TIKI_POSTSKYBOX_HPP_INCLUDED
#define TIKI_POSTSKYBOX_HPP_INCLUDED

#include "tiki/base/types.hpp"

#include "tiki/graphics/constantbuffer.hpp"

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
	class Texture;
	class VertexInputBinding;

	class PostProcessSkybox
	{
		TIKI_NONCOPYABLE_CLASS( PostProcessSkybox );

	public:

				PostProcessSkybox();
				~PostProcessSkybox();

		bool	create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );
		void	dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

		void	render( GraphicsContext& graphicsContext, const RenderTarget& renderTarget );

	private:

		const ShaderSet*			m_pShader;
		const Texture*				m_pTexture;

		ConstantBuffer				m_vertexConstants;
		ConstantBuffer				m_pixelConstants;

		const BlendState*			m_pBlendState;
		const DepthStencilState*	m_pDepthState;
		const RasterizerState*		m_pRasterizerState;
		const SamplerState*			m_pSamplerState;

		const VertexInputBinding*	m_pInputBinding;
		
	};
}

#endif // TIKI_POSTSKYBOX_HPP_INCLUDED
