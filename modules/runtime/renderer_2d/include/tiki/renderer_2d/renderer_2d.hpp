#pragma once

#include "tiki/base/types.hpp"
#include "tiki/container/array.hpp"
#include "tiki/container/chunked_pool.hpp"
#include "tiki/container/fixed_array.hpp"
#include "tiki/container/fixed_sized_array.hpp"
#include "tiki/container/linked_list.hpp"
#include "tiki/graphics/immediate_vertex.hpp"
#include "tiki/graphics/texture_data.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/math/rectangle_point.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/render_effects/post_bloom.hpp"

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
		uint32	emissivLayerId;

		float	drawToWorldFactor;
	};

	struct Renderer2dRenderParameters
	{
		const RenderTarget*	pRenderTarget			= nullptr;
		Color				backgroundColor			= TIKI_COLOR_BLACK;

		bool				enableBloom				= false;
		HdrColor			bloomCutoffThreshold	= color::toHdrColor( TIKI_COLOR_WHITE );

		bool				enableColorGrading		= false;
		const TextureData*	pColorGradingMap		= nullptr;
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
		void				queueSprite( const TextureData& texture, const Matrix32& worldTransform, const Vector2& originOffset, uint32 layerId );
		void				queueSprite( const TextureData& texture, const Rectangle& destinationRectangle, uint32 layerId );
		void				queueSprite( const TextureData& texture, const Rectangle& destinationRectangle, const AxisAlignedRectangle& sourceCoordinates, uint32 layerId );
		void				queueSprite( const TextureData& texture, const AxisAlignedRectangle& destinationRectangle, uint32 layerId );
		void				queueSprite( const TextureData& texture, const AxisAlignedRectangle& destinationRectangle, const AxisAlignedRectangle& sourceCoordinates, uint32 layerId );

		void				queueText( Font* pFont, const Vector2& position, const char* pText, uint32 layerId );

		void				update( float deltaTime );
		void				render( GraphicsContext& graphicsContext, const Renderer2dRenderParameters& parameters );

		const Camera&		getCamera() const { return m_camera; }
		float				getTargetZoom() const { return m_targetZoom; }
		void				setTargetZoom( float zoom ) { m_targetZoom = zoom; }
		void				setZoom( float zoom );
		void				setCameraPosition( const Vector2& position );

		Vector2				getVectorSize() const { return vector::create( float( m_width ), float( m_height ) ); }

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

		uint16							m_width;
		uint16							m_height;

		float							m_targetZoom;
		float							m_currentZoom;
		Camera							m_camera;

		Vector2							m_defaultTexCoords[ RectanglePoint_Count ];
		float							m_drawToWorldFactor;

		const ShaderSet*				m_pSpriteShader;
		const ShaderSet*				m_pCompositeShader;

		const BlendState*				m_pBlendStateSolid;
		const BlendState*				m_pBlendStateAlpha;
		const DepthStencilState*		m_pDepthState;
		const RasterizerState*			m_pRasterizerState;
		const SamplerState*				m_pSamplerState;

		const VertexInputBinding*		m_pSpriteInputBinding;
		const VertexInputBinding*		m_pCompositeInputBinding;

		ConstantBuffer					m_vertexConstants;
		TextureData						m_offscreenColorData;
		TextureData						m_offscreenEmissivData;
		RenderTarget					m_offscreenColorTarget;
		RenderTarget					m_offscreenEmissivTarget;

		ChunkedPool< RenderChunk >		m_chunks;
		Array< RenderLayer >			m_layers;
		uint32							m_emissivLayer;

		PostProcessBloom				m_bloom;

		RenderCommand&					allocateCommand( RenderLayer& layer );

		void							applyRenderStates( GraphicsContext& graphicsContext );
		void							renderLayer( GraphicsContext& graphicsContext, RenderLayer& layer );
	};
}
