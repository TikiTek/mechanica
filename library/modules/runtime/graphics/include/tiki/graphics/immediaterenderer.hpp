#pragma once
#ifndef TIKI_IMMEDIATERENDERER_HPP__INCLUDED
#define TIKI_IMMEDIATERENDERER_HPP__INCLUDED

#include "tiki/base/array.hpp"
#include "tiki/base/sizedarray.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"

#include "tiki/graphics/dynamicbuffer.hpp"
#include "tiki/graphics/samplerstate.hpp"
#include "tiki/graphicsresources/texture.hpp"
#include "tiki/graphicsbase/fontchar.hpp"

#include "tiki/base/structs.hpp"
#include "tiki/math/vector2.hpp"
#include "tiki/math/vector3.hpp"
#include "tiki/math/rectangle.hpp"

namespace tiki
{
	class Font;
	class GraphicsContext;
	class Material;
	class TextureData;
	class VertexFormat;
	struct SamplerState;

	class ImmediateRenderer
	{
		TIKI_NONCOPYABLE_CLASS( ImmediateRenderer );

	public:

							~ImmediateRenderer();
		
		bool				create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );
		void				dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager );

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

		const Material*						m_pMaterial;
		const VertexFormat*					m_pVertexFormat;
		const SamplerState*					m_pSamplerState;

		SizedArray< Sprite >				m_sprites;
		SizedArray< SpriteVertex >			m_vertices;
		
		DVertexBuffer< SpriteVertex >		m_vertexBuffer;

	};
}

#endif // TIKI_IMMEDIATERENDERER_HPP__INCLUDED
