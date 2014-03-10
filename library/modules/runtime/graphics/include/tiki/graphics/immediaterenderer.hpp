#pragma once
#ifndef TIKI_IMMEDIATERENDERER_HPP__INCLUDED
#define TIKI_IMMEDIATERENDERER_HPP__INCLUDED

#include "tiki/base/sizedarray.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/constantbuffer.hpp"
#include "tiki/graphics/vertexbuffer.hpp"
#include "tiki/graphicsbase/color.hpp"

namespace tiki
{
	class Font;
	class GraphicsContext;
	class GraphicsSystem;
	class ResourceManager;
	class SamplerState;
	class ShaderSet;
	class TextureData;
	class VertexFormat;
	class VertexInputBinding;
	struct Rectangle; 
	struct Vector2;

	class ImmediateRenderer
	{
		TIKI_NONCOPYABLE_CLASS( ImmediateRenderer );

	public:

							ImmediateRenderer();
							~ImmediateRenderer();
		
		bool				create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );
		void				dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

		void				flush( GraphicsContext& graphicsContext );

		void				drawTexture( const TextureData* pTexture, const Rectangle& dest, Color color = TIKI_COLOR_WHITE );
		void				drawTexture( const TextureData* pTexture, const Rectangle& dest, const Rectangle& src, Color color = TIKI_COLOR_WHITE );
		void				drawText( const Vector2& position, const Font& font, const char* pText, Color color = TIKI_COLOR_WHITE );

	private:

		enum
		{
			MaxSprites		= 100u,
			MaxVertices		= 1000u
		};

		struct Sprite
		{
			uint				vertexOffset;
			uint				vertexCount;
			uint				shaderBitMask;

			const TextureData*	pTexture;
		};

		struct SpriteVertex
		{
			float3	position;
			uint16	u;
			uint16	v;
			Color	color;
		};

		const ShaderSet*					m_pShaderSet;
		
		const BlendState*					m_pBlendState;
		const DepthStencilState*			m_pDepthStencilState;
		const RasterizerState*				m_pRasterizerState;
		const SamplerState*					m_pSamplerState;
		const VertexFormat*					m_pVertexFormat;
		const VertexInputBinding*			m_pVertexInputBinding;

		SizedArray< Sprite >				m_sprites;
		SizedArray< SpriteVertex >			m_vertices;
		
		VertexBuffer						m_vertexBuffer;
		ConstantBuffer						m_constantBuffer;

	};
}

#endif // TIKI_IMMEDIATERENDERER_HPP__INCLUDED
