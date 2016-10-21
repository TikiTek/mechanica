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
#include "tiki/math/camera.hpp"
#include "tiki/math/rectanglepoint.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/rendereffects/postbloom.hpp"

namespace tiki
{
	class Font;
	class GraphicsSystem;
	class ResourceRequestPool;
	struct AxisAlignedRectangle;
	struct Matrix32;
	struct Rectangle;

	struct Renderer2dCreationParameters
	{
		uint16	width;
		uint16	height;
		uint32	layerCount;
		float	drawToWorldFactor;
	};

	struct Renderer2dRenderParameters
	{
		Renderer2dRenderParameters()
		{
			backgroundColor		= TIKI_COLOR_BLACK;

			enableBloom			= false;
			color::toHdrColor( bloomCutoffThreshold, TIKI_COLOR_WHITE );

			enableColorGrading	= false;
			pColorGradingMap	= nullptr;
		}

		Color				backgroundColor;

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

		bool				create( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool, const Renderer2dCreationParameters& parameters );
		bool				createResources( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool );
		void				dispose( ResourceRequestPool& resourcePool );

		bool				resize( uint16 width, uint16 height );

		void				queueSprite( const TextureData& texture, const Matrix32& worldTransform, uint32 layerId );
		void				queueSprite( const TextureData& texture, const Rectangle& destinationRectangle, uint32 layerId );
		void				queueSprite( const TextureData& texture, const Rectangle& destinationRectangle, const AxisAlignedRectangle& sourceCoordinates, uint32 layerId );
		void				queueSprite( const TextureData& texture, const AxisAlignedRectangle& destinationRectangle, uint32 layerId );
		void				queueSprite( const TextureData& texture, const AxisAlignedRectangle& destinationRectangle, const AxisAlignedRectangle& sourceCoordinates, uint32 layerId );

		void				queueText( Font* pFont, const Vector2& position, const char* pText, uint32 layerId );

		void				update( float deltaTime );
		void				render( GraphicsContext& graphicsContext, const Renderer2dRenderParameters& parameters );
		
		const Camera&		getCamera() const { return m_camera; }

		void				setTargetZoom( float zoom );
		void				setCameraPosition( const Vector2& position );

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

		float							m_targetZoom;
		float							m_currentZoom;
		Camera							m_camera;

		Vector2							m_defaultTexCoords[ RectanglePoint_Count ];
		float							m_drawToWorldFactor;

		const ShaderSet*				m_pSpriteShader;
		const ShaderSet*				m_pCompositeShader;

		const BlendState*				m_pBlendState;
		const DepthStencilState*		m_pDepthState;
		const RasterizerState*			m_pRasterizerState;
		const SamplerState*				m_pSamplerState;

		const VertexInputBinding*		m_pSpriteInputBinding;
		const VertexInputBinding*		m_pCompositeInputBinding;

		ConstantBuffer					m_vertexConstants;
		TextureData						m_offscreenColorData;
		RenderTarget					m_offscreenTarget;

		ChunkedPool< RenderChunk >		m_chunks;
		Array< RenderLayer >			m_layers;

		PostProcessBloom				m_bloom;

		RenderCommand&					allocateCommand( RenderLayer& layer );
	};
}

#endif // TIKI_RENDERER2D_HPP_INCLUDED
