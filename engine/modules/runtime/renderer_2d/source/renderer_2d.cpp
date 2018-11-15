#include "tiki/renderer_2d/renderer_2d.hpp"

#include "tiki/base/debug_property.hpp"
#include "tiki/base/numbers.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphics_system.hpp"
#include "tiki/graphics/shader_set.hpp"
#include "tiki/math/axis_aligned_rectangle.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/resource/resource_request_pool.hpp"

#include "shader/2d_sprite_shader.hpp"

namespace tiki
{
	Renderer2d::Renderer2d()
	{
		m_pGraphicsSystem			= nullptr;

		m_pSpriteShader				= nullptr;
		m_pCompositeShader			= nullptr;

		m_pBlendStateSolid			= nullptr;
		m_pBlendStateAlpha			= nullptr;
		m_pDepthState				= nullptr;
		m_pRasterizerState			= nullptr;
		m_pSamplerState				= nullptr;

		m_pSpriteInputBinding		= nullptr;
		m_pCompositeInputBinding	= nullptr;
	}

	Renderer2d::~Renderer2d()
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
	}

	bool Renderer2d::create( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool, const Renderer2dCreationParameters& parameters )
	{
		m_pGraphicsSystem	= &graphicsSystem;
		m_pSpriteShader		= nullptr;
		m_pCompositeShader	= nullptr;
		m_currentZoom		= 1.0f;
		m_targetZoom		= 1.0f;

		m_camera.create();

		if( !m_chunks.create( 16u ) ||
			!m_layers.create( parameters.layerCount ) ||
			!m_vertexConstants.create( graphicsSystem, sizeof( SpriteVertexConstantData ), "2d_sprite_vertex" ) )
		{
			dispose( resourcePool );
			return false;
		}

		m_pBlendStateSolid	= graphicsSystem.createBlendState( false );
		m_pBlendStateAlpha	= graphicsSystem.createBlendState( true, Blend_SourceAlpha, Blend_InverseSourceAlpha, BlendOperation_Add );
		m_pDepthState		= graphicsSystem.createDepthStencilState( false, false );
		m_pRasterizerState	= graphicsSystem.createRasterizerState( FillMode_Solid, CullMode_None, WindingOrder_Clockwise );
		m_pSamplerState		= graphicsSystem.createSamplerState( AddressMode_Clamp, AddressMode_Clamp, AddressMode_Clamp, FilterMode_Linear, FilterMode_Linear );
		if( m_pBlendStateSolid == nullptr || m_pBlendStateAlpha == nullptr || m_pDepthState == nullptr || m_pRasterizerState == nullptr || m_pSamplerState == nullptr )
		{
			dispose( resourcePool );
			return false;
		}

		PostProcessBloomParameters bloomParameters;
		bloomParameters.width		= parameters.width / 2u;
		bloomParameters.height		= parameters.height / 2u;
		bloomParameters.passCount	= 6u;
		if( !m_bloom.create( graphicsSystem, resourcePool, bloomParameters ) )
		{
			dispose( resourcePool );
			return false;
		}

		if( !resize( parameters.width, parameters.height ) )
		{
			dispose( resourcePool );
			return false;
		}
		resourcePool.beginLoadResource( &m_pSpriteShader, "2d_sprite.shader" );
		resourcePool.beginLoadResource( &m_pCompositeShader, "2d_composite.shader" );

		const AxisAlignedRectangle defaultRectangle = createAxisAlignedRectangle( 0.0f, 0.0f, 1.0f, 1.0f );
		defaultRectangle.getVertices( m_defaultTexCoords );

		m_drawToWorldFactor = parameters.drawToWorldFactor;

		return true;
	}

	bool Renderer2d::createResources( GraphicsSystem& graphicsSystem, ResourceRequestPool& resourcePool )
	{
		if( m_pSpriteShader == nullptr || m_pCompositeShader == nullptr )
		{
			dispose( resourcePool );
			return false;
		}

		m_pSpriteInputBinding = graphicsSystem.createVertexInputBinding( m_pSpriteShader->getShader( ShaderType_VertexShader, 0u ), graphicsSystem.getStockVertexFormat( StockVertexFormat_Pos2Tex2 ) );
		m_pCompositeInputBinding = graphicsSystem.createVertexInputBinding( m_pCompositeShader->getShader( ShaderType_VertexShader, 0u ), graphicsSystem.getStockVertexFormat( StockVertexFormat_Pos2Tex2 ) );
		if( m_pSpriteInputBinding == nullptr || m_pCompositeInputBinding == nullptr )
		{
			dispose( resourcePool );
			return false;
		}

		if( !m_bloom.createResources( graphicsSystem, resourcePool ) )
		{
			dispose( resourcePool );
			return false;
		}

		return true;
	}

	void Renderer2d::dispose( ResourceRequestPool& resourcePool )
	{
		m_bloom.dispose( *m_pGraphicsSystem, resourcePool );

		m_vertexConstants.dispose( *m_pGraphicsSystem );
		m_layers.dispose();
		m_chunks.dispose();

		m_offscreenEmissivTarget.dispose( *m_pGraphicsSystem );
		m_offscreenColorTarget.dispose( *m_pGraphicsSystem );
		m_offscreenEmissivData.dispose( *m_pGraphicsSystem );
		m_offscreenColorData.dispose( *m_pGraphicsSystem );

		m_pGraphicsSystem->disposeVertexInputBinding( m_pCompositeInputBinding );
		m_pGraphicsSystem->disposeVertexInputBinding( m_pSpriteInputBinding );

		m_pGraphicsSystem->disposeSamplerState( m_pSamplerState );
		m_pGraphicsSystem->disposeRasterizerState( m_pRasterizerState );
		m_pGraphicsSystem->disposeDepthStencilState( m_pDepthState );
		m_pGraphicsSystem->disposeBlendState( m_pBlendStateAlpha );
		m_pGraphicsSystem->disposeBlendState( m_pBlendStateSolid );

		resourcePool.unloadResource( m_pSpriteShader );
		resourcePool.unloadResource( m_pCompositeShader );

		m_pGraphicsSystem	= nullptr;
	}

	bool Renderer2d::resize( uint16 width, uint16 height )
	{
		m_offscreenEmissivTarget.dispose( *m_pGraphicsSystem );
		m_offscreenColorTarget.dispose( *m_pGraphicsSystem );
		m_offscreenEmissivData.dispose( *m_pGraphicsSystem );
		m_offscreenColorData.dispose( *m_pGraphicsSystem );

		Projection projection;
		projection.createOrthographic( width, width, 0.0f, 10.0f );

		TextureDescription textureDescription;
		textureDescription.width	= width;
		textureDescription.height	= height;
		textureDescription.type		= TextureType_2d;
		textureDescription.flags	= TextureFlags_RenderTarget | TextureFlags_ShaderInput;
		textureDescription.format	= PixelFormat_R8G8B8A8_Gamma;
		if( !m_offscreenColorData.create( *m_pGraphicsSystem, textureDescription, nullptr, "OffscreenColorData" ) ||
			!m_offscreenEmissivData.create( *m_pGraphicsSystem, textureDescription, nullptr, "OffscreenEmissivData" ) )
		{
			return false;
		}

		RenderTargetBuffer colorTargetBuffer( m_offscreenColorData );
		RenderTargetBuffer emissivTargetBuffer( m_offscreenEmissivData );
		if( !m_offscreenColorTarget.create( *m_pGraphicsSystem, width, height, &colorTargetBuffer, 1u, nullptr ) ||
			!m_offscreenEmissivTarget.create( *m_pGraphicsSystem, width, height, &emissivTargetBuffer, 1u, nullptr ))
		{
			return false;
		}

		return m_bloom.resize( *m_pGraphicsSystem, width / 2u, height / 2u );
	}

	void Renderer2d::queueSprite( const TextureData& texture, const Matrix32& worldTransform, uint32 layerId )
	{
		RenderLayer& layer = m_layers[ layerId ];

		RenderCommand& command = allocateCommand( layer );
		command.pTexture = &texture;

		const AxisAlignedRectangle destinationRectangle = createAxisAlignedRectangle(
			Vector2::zero,
			vector::create( texture.getWidth() * m_drawToWorldFactor, texture.getHeight() * m_drawToWorldFactor )
		);

		Vector2 rectangleVertices[ RectanglePoint_Count ];
		destinationRectangle.getVertices( rectangleVertices );

		for( uint i = 0u; i < command.vertices.getCount(); ++i )
		{
			RenderVertex& vertex = command.vertices[ i ];

			matrix::transform( rectangleVertices[ i ], worldTransform );
			vector::toFloat( vertex.position, rectangleVertices[ i ] );
			vector::toFloat( vertex.texCoord, m_defaultTexCoords[ i ] );
		}
	}

	void Renderer2d::queueSprite( const TextureData& texture, const Matrix32& worldTransform, const Vector2& originOffset, uint32 layerId )
	{
		RenderLayer& layer = m_layers[ layerId ];

		RenderCommand& command = allocateCommand( layer );
		command.pTexture = &texture;

		const AxisAlignedRectangle destinationRectangle = createAxisAlignedRectangle(
			Vector2::zero,
			vector::create( texture.getWidth() * m_drawToWorldFactor, texture.getHeight() * m_drawToWorldFactor )
		);

		Vector2 rectangleVertices[ RectanglePoint_Count ];
		destinationRectangle.getVertices( rectangleVertices );

		for (uint i = 0u; i < command.vertices.getCount(); ++i)
		{
			RenderVertex& vertex = command.vertices[ i ];

			vector::sub( rectangleVertices[ i ], originOffset );
			matrix::transform( rectangleVertices[ i ], worldTransform );

			vector::toFloat( vertex.position, rectangleVertices[ i ] );
			vector::toFloat( vertex.texCoord, m_defaultTexCoords[ i ] );
		}
	}

	void Renderer2d::queueSprite( const TextureData& texture, const Rectangle& destinationRectangle, uint32 layerId )
	{
		RenderLayer& layer = m_layers[ layerId ];

		RenderCommand& command = allocateCommand( layer );
		command.pTexture = &texture;

		Vector2 rectangleVertices[ RectanglePoint_Count ];
		destinationRectangle.getVertices( rectangleVertices );

		for( uint i = 0u; i < command.vertices.getCount(); ++i )
		{
			RenderVertex& vertex = command.vertices[ i ];

			vector::toFloat( vertex.position, rectangleVertices[ i ] );
			vector::toFloat( vertex.texCoord, m_defaultTexCoords[ i ] );
		}
	}

	void Renderer2d::queueSprite( const TextureData& texture, const Rectangle& destinationRectangle, const AxisAlignedRectangle& sourceCoordinates, uint32 layerId )
	{
		RenderLayer& layer = m_layers[ layerId ];

		RenderCommand& command = allocateCommand( layer );
		command.pTexture = &texture;

		Vector2 rectangleVertices[ RectanglePoint_Count ];
		destinationRectangle.getVertices( rectangleVertices );

		Vector2 sourcePoints[ RectanglePoint_Count ];
		sourceCoordinates.getVertices( sourcePoints );

		for( uint i = 0u; i < command.vertices.getCount(); ++i )
		{
			RenderVertex& vertex = command.vertices[ i ];

			vector::toFloat( vertex.position, rectangleVertices[ i ] );
			vector::toFloat( vertex.texCoord, sourcePoints[ i ] );
		}
	}

	void Renderer2d::queueSprite( const TextureData& texture, const AxisAlignedRectangle& destinationRectangle, uint32 layerId )
	{
		RenderLayer& layer = m_layers[ layerId ];

		RenderCommand& command = allocateCommand( layer );
		command.pTexture = &texture;

		Vector2 rectangleVertices[ RectanglePoint_Count ];
		destinationRectangle.getVertices( rectangleVertices );

		for( uint i = 0u; i < command.vertices.getCount(); ++i )
		{
			RenderVertex& vertex = command.vertices[ i ];

			vector::toFloat( vertex.position, rectangleVertices[ i ] );
			vector::toFloat( vertex.texCoord, m_defaultTexCoords[ i ] );
		}
	}

	void Renderer2d::queueSprite( const TextureData& texture, const AxisAlignedRectangle& destinationRectangle, const AxisAlignedRectangle& sourceCoordinates, uint32 layerId )
	{
		RenderLayer& layer = m_layers[ layerId ];

		RenderCommand& command = allocateCommand( layer );
		command.pTexture = &texture;

		Vector2 rectangleVertices[ RectanglePoint_Count ];
		destinationRectangle.getVertices( rectangleVertices );

		Vector2 sourcePoints[ RectanglePoint_Count ];
		sourceCoordinates.getVertices( sourcePoints );

		for( uint i = 0u; i < command.vertices.getCount(); ++i )
		{
			RenderVertex& vertex = command.vertices[ i ];

			vector::toFloat( vertex.position, rectangleVertices[ i ] );
			vector::toFloat( vertex.texCoord, sourcePoints[ i ] );
		}
	}

	void Renderer2d::queueText( Font* pFont, const Vector2& position, const char* pText, uint32 layerId )
	{
		TIKI_ASSERT( pFont != nullptr );
		TIKI_ASSERT( pText != nullptr );

		const TextureData& fontTexture = pFont->getTextureData();

		Vector2 currentPosition = position;
		while( *pText != '\0' )
		{
			const FontChar& c = pFont->getChar( *pText );

			const AxisAlignedRectangle destinationRectangle = createAxisAlignedRectangle(
				currentPosition.x,
				currentPosition.y,
				c.width,
				c.height
			);

			const AxisAlignedRectangle sourceCoordinates = createAxisAlignedRectangleMinMax(
				u16::unormToFloat( c.x1 ),
				u16::unormToFloat( c.y1 ),
				u16::unormToFloat( c.x2 ),
				u16::unormToFloat( c.y2 )
			);

			queueSprite( fontTexture, destinationRectangle, sourceCoordinates, layerId );
			pText++;
		}
	}

	void Renderer2d::update( float deltaTime )
	{
		m_currentZoom = f32::lerp( m_currentZoom, m_targetZoom, deltaTime );

		const float offscreenWidth = (float)m_offscreenColorData.getWidth() / 100.0f;
		const float offscreenHeight = (float)m_offscreenColorData.getHeight() / -100.0f;

		Projection projection;
		projection.createOrthographic( offscreenWidth / m_currentZoom, offscreenHeight / m_currentZoom, 0.0f, 10.0f );

		m_camera.setProjection( projection );
	}

	void Renderer2d::render( GraphicsContext& graphicsContext, const Renderer2dRenderParameters& parameters )
	{
		SpriteVertexConstantData* pVertexData = graphicsContext.mapBuffer< SpriteVertexConstantData >( m_vertexConstants );
		createGraphicsMatrix44( pVertexData->projection, m_camera.getViewProjectionMatrix() );
		graphicsContext.unmapBuffer( m_vertexConstants );

		// color
		graphicsContext.clear( m_offscreenColorTarget, parameters.backgroundColor );
		graphicsContext.beginRenderPass( m_offscreenColorTarget );
		applyRenderStates( graphicsContext );
		for( uint layerIndex = 0u; layerIndex < m_layers.getCount(); ++layerIndex )
		{
			if( layerIndex == m_emissivLayer )
			{
				continue;
			}

			renderLayer( graphicsContext, m_layers[ layerIndex ] );
		}
		graphicsContext.endRenderPass();

		// emissiv
		graphicsContext.clear( m_offscreenEmissivTarget, TIKI_COLOR_TRANSPARENT );
		graphicsContext.beginRenderPass( m_offscreenEmissivTarget );
		applyRenderStates( graphicsContext );
		renderLayer( graphicsContext, m_layers[ m_emissivLayer ] );
		graphicsContext.endRenderPass();

		if( parameters.enableBloom )
		{
			PostProcessBloomRenderParameters bloomParameters;
			bloomParameters.pSourceData		= &m_offscreenColorData;
			bloomParameters.pEmissivData	= &m_offscreenEmissivData;
			color::toVector3( bloomParameters.cutoffThreshold, parameters.bloomCutoffThreshold );
			m_bloom.render( graphicsContext, bloomParameters );
		}

		const RenderTarget* pRenderTarget = parameters.pRenderTarget;
		if( pRenderTarget == nullptr )
		{
			pRenderTarget = &graphicsContext.getBackBuffer();
		}

		graphicsContext.beginRenderPass( *pRenderTarget );

		uint32 pixelShaderIndex = 0u;
		pixelShaderIndex |= (parameters.enableBloom ? 1u : 0u);
		pixelShaderIndex |= (parameters.enableColorGrading ? 2u : 0u);

		graphicsContext.setVertexShader( m_pCompositeShader->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setPixelShader( m_pCompositeShader->getShader( ShaderType_PixelShader, pixelShaderIndex ) );

		graphicsContext.setBlendState( m_pBlendStateSolid );
		graphicsContext.setDepthStencilState( m_pDepthState );
		graphicsContext.setRasterizerState( m_pRasterizerState );
		graphicsContext.setVertexInputBinding( m_pCompositeInputBinding );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerState );

		graphicsContext.setPixelShaderTexture( 0u, &m_offscreenColorData );
		if( parameters.enableBloom )
		{
			graphicsContext.setPixelShaderTexture( 1u, &m_bloom.getResultData() );
		}
		if( parameters.enableColorGrading )
		{
			graphicsContext.setPixelShaderTexture( 2u, parameters.pColorGradingMap );
		}

		graphicsContext.drawFullScreenQuadPos2Tex2();

		graphicsContext.endRenderPass();
	}

	void Renderer2d::setZoom( float zoom )
	{
		m_targetZoom = zoom;
		m_currentZoom = zoom;
	}

	void Renderer2d::setCameraPosition( const Vector2& position )
	{
		Vector3 position3;
		vector::set( position3, position, 0.0f );

		m_camera.setTransform( position3, Quaternion::identity );
	}

	Renderer2d::RenderCommand& Renderer2d::allocateCommand( RenderLayer& layer )
	{
		RenderChunk* pChunk = nullptr;
		if( !layer.chunks.isEmpty() )
		{
			pChunk = &layer.chunks.getLast();
		}

		if( pChunk == nullptr || pChunk->commands.isFull() )
		{
			pChunk = m_chunks.push();
			layer.chunks.push( pChunk );
		}

		return pChunk->commands.push();
	}

	void Renderer2d::applyRenderStates( GraphicsContext& graphicsContext )
	{
		graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleStrip );
		graphicsContext.setVertexShader( m_pSpriteShader->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setPixelShader( m_pSpriteShader->getShader( ShaderType_PixelShader, 0u ) );

		graphicsContext.setBlendState( m_pBlendStateAlpha );
		graphicsContext.setDepthStencilState( m_pDepthState );
		graphicsContext.setRasterizerState( m_pRasterizerState );
		graphicsContext.setVertexInputBinding( m_pSpriteInputBinding );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerState );

		graphicsContext.setVertexShaderConstant( 0u, m_vertexConstants );
	}

	void Renderer2d::renderLayer( GraphicsContext& graphicsContext, RenderLayer& layer )
	{
		while( !layer.chunks.isEmpty() )
		{
			RenderChunk& chunk = layer.chunks.getFirst();

			for( uint commandIndex = 0u; commandIndex < chunk.commands.getCount(); ++commandIndex )
			{
				RenderCommand& command = chunk.commands[ commandIndex ];

				graphicsContext.setPixelShaderTexture( 0u, command.pTexture );

				StaticArray< RenderVertex > vertices;
				graphicsContext.beginImmediateGeometry( vertices, 4u );
				vertices[ 0u ] = command.vertices[ 0u ];
				vertices[ 1u ] = command.vertices[ 1u ];
				vertices[ 2u ] = command.vertices[ 2u ];
				vertices[ 3u ] = command.vertices[ 3u ];
				graphicsContext.endImmediateGeometry( vertices );
			}

			layer.chunks.removeSortedByValue( chunk );
			m_chunks.removeUnsortedByValue( chunk );
		}
	}
}
