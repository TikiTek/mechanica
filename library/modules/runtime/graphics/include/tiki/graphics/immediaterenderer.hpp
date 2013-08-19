#ifndef TIKI_SPRITEBATCH_HPP__
#define TIKI_SPRITEBATCH_HPP__

#include "tiki/base/array.hpp"
#include "tiki/base/sizedarray.hpp"
#include "tiki/base/string.hpp"
#include "tiki/base/types.hpp"

#include "tiki/graphics/dynamicbuffer.hpp"
#include "tiki/graphics/sampler.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/graphicsbase/fontchar.hpp"

#include "tiki/base/structs.hpp"
#include "tiki/math/vector2.hpp"
#include "tiki/math/vector3.hpp"
#include "tiki/math/rectangle.hpp"

namespace tiki
{
	class GpuContext;
	class Material;
	class VertexFormat;
	class TextureData;
	class Font;

	class SpritRenderer
	{
		TIKI_NONCOPYABLE_CLASS( SpritRenderer );
	public:
		friend class GpuContext;

							~SpritRenderer();
		
		bool				create( GpuContext* pContext );
		void				dispose();

		void				flush();

		void				drawText( const Vector2& position, const Font& font, const string& text );

		void				drawTexture( const Rectangle& rect, const TextureData& textureData, bool percentage = false  );
		void				drawTexture( const Rectangle& dest, const Rectangle& src, const TextureData& tex );

	private:

		enum
		{
			MaxSprites		= 100,
			MaxTextTextures = 100,
			MaxTextChars	= 1000
		};

		struct SpriteVertex
		{
			float3 position;
			float2 uv;
		};

		struct FontVertex
		{
			float2		position;
			byte4		identifier;
			FontChar	character;
		};

											SpritRenderer();

		void								toScreenSpace( Rectangle& result );
		void								toScreenSpacePercentage( Rectangle& result );

		void								toUVSpace( Rectangle& rec, const TextureData& texture );


		GpuContext*							m_pGpuContext;

		const Material*						m_pMaterial;
		const VertexFormat*					m_pVertexFormat;
		Sampler								m_sampler;

		Vector2								m_viewPort;

		SizedArray< const TextureData* >	m_textures;
		SizedArray< SpriteVertex >			m_sprites;

		const VertexFormat*					m_pTextVertexFormat;
		const Material*						m_pTextMaterial;
		DVertexBuffer< FontVertex >			m_textVertexBuffer;

		SizedArray< const TextureData* >	m_textTextures;
		SizedArray< FontVertex >			m_textChars;
		SizedArray< size_t >				m_textLength;

		DVertexBuffer< SpriteVertex >		m_vertexBuffer;

	};
}

#endif // TIKI_SPRITEBATCH_HPP__
