#pragma once
#ifndef TIKI_IMMEDIATERENDERER_HPP__INCLUDED
#define TIKI_IMMEDIATERENDERER_HPP__INCLUDED

#include "tiki/base/sizedarray.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/structs.hpp"
#include "tiki/base/types.hpp"
#include "tiki/graphics/dynamicbuffer.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphicsbase/fontchar.hpp"
#include "tiki/math/rectangle.hpp"

namespace tiki
{
	class Font;
	class GraphicsContext;
	class Material;
	class TextureData;
	class VertexFormat;
	class SamplerState;
	struct Rectangle; 

	class ImmediateRenderer
	{
		TIKI_NONCOPYABLE_CLASS( ImmediateRenderer );

	public:

							ImmediateRenderer();
							~ImmediateRenderer();
		
		bool				create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const WindowEventBuffer& eventBuffer );
		void				dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

		void				update();

		void				flush( GraphicsContext& graphicsContext );

		void				drawText( const Vector2& position, const Font& font, const string& text, Color color );

		void				drawTexture( const TextureData& texture, const Rectangle& dest );
		void				drawTexture( const TextureData& texture, const Rectangle& dest, const Rectangle& src );

	private:

		enum
		{
			MaxSprites		= 100,
			MaxVertices		= 1000
		};

		struct Sprite
		{
			size_t				offset;
			size_t				count;
			const TextureData*	pTexture;
		};

		struct SpriteVertex
		{
			float3	position;
			float2	uv;
			float4	color;
		};

		const WindowEventBuffer*			m_pEventBuffer;

		const Material*						m_pMaterial;
		const VertexFormat*					m_pVertexFormat;
		const SamplerState*					m_pSamplerState;

		SizedArray< Sprite >				m_sprites;
		SizedArray< SpriteVertex >			m_vertices;
		
		VertexBuffer						m_vertexBuffer;

	};
}

#endif // TIKI_IMMEDIATERENDERER_HPP__INCLUDED
