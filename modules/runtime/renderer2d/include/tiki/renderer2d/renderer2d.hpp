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
#include "tiki/math/vector.hpp"
#include "tiki/rendereffects/postbloom.hpp"
#include "tiki/math/rectanglepoint.hpp"

namespace tiki
{
	class Font;
	class GraphicsSystem;
	class ResourceRequestPool;
	struct AxisAlignedRectangle;
	struct Rectangle;

	struct Renderer2dRenderParameters
	{
		Renderer2dRenderParameters()
		{
			enableBloom			= false;
			color::toHdrColor( bloomCutoffThreshold, TIKI_COLOR_WHITE );

			enableColorGrading	= false;
			pColorGradingMap	= nullptr;
		}

		bool				enableBloom;
		HdrColor			bloomCutoffThreshold;

		bool				enableColorGrading;
		const TextureData*	pColorGradingMap;
	};

	class Renderer2d
	{
		TIKI_NONCOPYABLE_CLASS( Renderer2d );

	public:

							Renderer2d();
							~Renderer2d();

		bool				create( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool, uint16 width, uint16 height, uint layerCount );
		bool				createResources( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool );
		void				dispose( ResourceRequestPool& resourcePool );

		bool				resize( uint16 width, uint16 height );

		void				queueSprite( const TextureData& texture, const Rectangle& destinationRectangle, uint layerId );
		void				queueSprite( const TextureData& texture, const Rectangle& destinationRectangle, const AxisAlignedRectangle& sourceCoordinates, uint layerId );
		void				queueSprite( const TextureData& texture, const AxisAlignedRectangle& destinationRectangle, uint layerId );
		void				queueSprite( const TextureData& texture, const AxisAlignedRectangle& destinationRectangle, const AxisAlignedRectangle& sourceCoordinates, uint layerId );

		void				queueText( Font* pFont, const Vector2& position, const char* pText, uint layerId );

		void				render( GraphicsContext& graphicsContext, const Renderer2dRenderParameters& parameters );

		const TextureData&	getOffscreenColorData() const { return m_offscreenColorData; }

	private:

		struct RenderVertex
		{
			float2						position;
			float2						texCoord;
		};

		struct RenderCommand
		{
			typedef FixedArray< RenderVertex, 4u > VertexArray;
			
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

		const ShaderSet*				m_pSpriteShader;
		const ShaderSet*				m_pCompositeShader;

		const BlendState*				m_pBlendState;
		const DepthStencilState*		m_pDepthState;
		const RasterizerState*			m_pRasterizerState;
		const SamplerState*				m_pSamplerState;

		const VertexInputBinding*		m_pSpriteInputBinding;
		const VertexInputBinding*		m_pCompositeInputBinding;

		TextureData						m_offscreenColorData;
		RenderTarget					m_offscreenTarget;

		ChunkedPool< RenderChunk >		m_chunks;
		Array< RenderLayer >			m_layers;

		Vector2							m_defaultTexCoords[ RectanglePoint_Count ];

		PostProcessBloom				m_bloom;

		RenderCommand&					allocateCommand( RenderLayer& layer );
	};
}

#endif // TIKI_RENDERER2D_HPP_INCLUDED
