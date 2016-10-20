#pragma once
#ifndef TIKI_RENDERER2D_HPP_INCLUDED
#define TIKI_RENDERER2D_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/container/array.hpp"
#include "tiki/container/chunkedpool.hpp"
#include "tiki/container/fixedarray.hpp"
#include "tiki/container/fixedsizedarray.hpp"
#include "tiki/container/linkedlist.hpp"
#include "tiki/graphics/immediatevertex.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/rendereffects/postbloom.hpp"

namespace tiki
{
	class GraphicsSystem;
	class ResourceRequestPool;
	struct AxisAlignedRectangle;
	struct Rectangle;

	class Renderer2d
	{
		TIKI_NONCOPYABLE_WITHCTOR_CLASS( Renderer2d );

	public:

		bool				create( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool, uint width, uint height, uint layerCount );
		bool				createResources( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool );
		void				dispose( ResourceRequestPool& resourcePool );

		bool				resize( uint width, uint height );

		void				queueSprite( TextureData* pTexture, const Rectangle& destinationRectangle, uint layer );
		void				queueSprite( TextureData* pTexture, const Rectangle& destinationRectangle, const AxisAlignedRectangle& sourceCoodinates, uint layer );
		void				queueSprite( TextureData* pTexture, const AxisAlignedRectangle& destinationRectangle, uint layer );
		void				queueSprite( TextureData* pTexture, const AxisAlignedRectangle& destinationRectangle, const AxisAlignedRectangle& sourceCoordinates, uint layer );

		//void				queueText( Font* pFont, Vector2 position, uint layer );

		void				render( GraphicsContext& graphicscontext ) const;

		const TextureData&	getOffscreenColorData() const { return m_offscreenColorData; }

	private:

		struct RenderCommand
		{
			typedef FixedArray< ImmediateVertex, 4u > VertexArray;
			
			const TextureData*			pTexture;
			VertexArray					vertices;
		};

		struct RenderChunk : LinkedItem< RenderChunk >
		{
			typedef FixedSizedArray< RenderCommand, 64u > CommandArray;

			CommandArray				commands;
		};

		struct RenderLayer
		{
			LinkedList< RenderChunk >	chunks;
		};

		GraphicsSystem*					m_pGraphicsSystem;

		TextureData						m_offscreenColorData;

		ChunkedPool< RenderChunk >		m_chunks;
		Array< RenderLayer >			m_layers;

		PostProcessBloom				m_bloom;

		RenderCommand&					allocateCommand( RenderLayer& layer );
	};
}

#endif // TIKI_RENDERER2D_HPP_INCLUDED
