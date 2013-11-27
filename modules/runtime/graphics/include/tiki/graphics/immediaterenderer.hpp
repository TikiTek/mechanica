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
	class Material;
	class ResourceManager;
	class SamplerState;
	class TextureData;
	class VertexInputBinding;
	class VertexFormat;
	class WindowEventBuffer;
	struct Rectangle; 
	struct Vector2;

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
			uint16	u;
			uint16	v;
			Color	color;
		};

		const WindowEventBuffer*			m_pEventBuffer;

		const Material*						m_pMaterial;
		const VertexFormat*					m_pVertexFormat;
		const SamplerState*					m_pSamplerState;
		const VertexInputBinding*			m_pVertexInputBinding;

		SizedArray< Sprite >				m_sprites;
		SizedArray< SpriteVertex >			m_vertices;
		
		VertexBuffer						m_vertexBuffer;
		ConstantBuffer						m_constantBuffer;

	};
}

#endif // TIKI_IMMEDIATERENDERER_HPP__INCLUDED
