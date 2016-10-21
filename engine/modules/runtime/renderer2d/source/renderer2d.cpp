#include "tiki/renderer2d/renderer2d.hpp"

#include "tiki/base/numbers.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/math/axisalignedrectangle.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/resource/resourcerequestpool.hpp"

#include "shader/2d_sprite_shader.hpp"

namespace tiki
{
	Renderer2d::Renderer2d()
	{
		m_pGraphicsSystem			= nullptr;

		m_pSpriteShader				= nullptr;
		m_pCompositeShader			= nullptr;

		m_pBlendState				= nullptr;
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

		m_pBlendState		= graphicsSystem.createBlendState( true, Blend_SourceAlpha, Blend_InverseSourceAlpha, BlendOperation_Add, ColorWriteMask_All );
		m_pDepthState		= graphicsSystem.createDepthStencilState( false, false );
		m_pRasterizerState	= graphicsSystem.createRasterizerState( FillMode_Solid, CullMode_None, WindingOrder_Clockwise );
		m_pSamplerState		= graphicsSystem.createSamplerState( AddressMode_Clamp, AddressMode_Clamp, AddressMode_Clamp, FilterMode_Linear, FilterMode_Linear );
		if( m_pBlendState == nullptr || m_pDepthState == nullptr || m_pRasterizerState == nullptr || m_pSamplerState == nullptr )
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

		m_offscreenTarget.dispose( *m_pGraphicsSystem );
		m_offscreenColorData.dispose( *m_pGraphicsSystem );

		m_pGraphicsSystem->disposeVertexInputBinding( m_pCompositeInputBinding );
		m_pGraphicsSystem->disposeVertexInputBinding( m_pSpriteInputBinding );

		m_pGraphicsSystem->disposeSamplerState( m_pSamplerState );
		m_pGraphicsSystem->disposeRasterizerState( m_pRasterizerState );
		m_pGraphicsSystem->disposeDepthStencilState( m_pDepthState );
		m_pGraphicsSystem->disposeBlendState( m_pBlendState );

		resourcePool.unloadResource( m_pSpriteShader );
		resourcePool.unloadResource( m_pCompositeShader );

		m_pGraphicsSystem	= nullptr;
	}

	bool Renderer2d::resize( uint16 width, uint16 height )
	{
		m_offscreenTarget.dispose( *m_pGraphicsSystem );
		m_offscreenColorData.dispose( *m_pGraphicsSystem );

		Projection projection;
		projection.createOrthographic( width, width, 0.0f, 10.0f );

		TextureDescription textureDescription;
		textureDescription.width	= width;
		textureDescription.height	= height;
		textureDescription.type		= TextureType_2d;
		textureDescription.flags	= TextureFlags_RenderTarget | TextureFlags_ShaderInput;
		textureDescription.format	= PixelFormat_R8G8B8A8_Gamma;		
		if( !m_offscreenColorData.create( *m_pGraphicsSystem, textureDescription, nullptr, "OffscreenColorData" ) )
		{
			return false;
		}

		RenderTargetBuffer renderTargetBuffer;
		renderTargetBuffer.pDataBuffer	= &m_offscreenColorData;		
		if( !m_offscreenTarget.create( *m_pGraphicsSystem, width, height, &renderTargetBuffer, 1u, nullptr ) )
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

		const float offscreenWidth = (float)m_offscreenColorData.getWidth() / (100.0f * m_currentZoom);
		const float offscreenHeight = (float)m_offscreenColorData.getHeight() / (-100.0f * m_currentZoom);

		Projection projection;
		projection.createOrthographic( offscreenWidth, offscreenHeight, 0.0f, 10.0f );

		m_camera.setProjection( projection );
	}

	void Renderer2d::render( GraphicsContext& graphicsContext, const Renderer2dRenderParameters& parameters )
	{
		SpriteVertexConstantData* pVertexData = graphicsContext.mapBuffer< SpriteVertexConstantData >( m_vertexConstants );
		createGraphicsMatrix44( pVertexData->projection, m_camera.getViewProjectionMatrix() );
		graphicsContext.unmapBuffer( m_vertexConstants );

		graphicsContext.clear( m_offscreenTarget, parameters.backgroundColor );
		graphicsContext.beginRenderPass( m_offscreenTarget );

		graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleStrip );
		graphicsContext.setVertexShader( m_pSpriteShader->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setPixelShader( m_pSpriteShader->getShader( ShaderType_PixelShader, 0u ) );

		graphicsContext.setBlendState( m_pBlendState );
		graphicsContext.setDepthStencilState( m_pDepthState );
		graphicsContext.setRasterizerState( m_pRasterizerState );
		graphicsContext.setVertexInputBinding( m_pSpriteInputBinding );
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerState );

		graphicsContext.setVertexShaderConstant( 0u, m_vertexConstants );

		for( uint layerIndex = 0u; layerIndex < m_layers.getCount(); ++layerIndex )
		{
			RenderLayer& layer = m_layers[ layerIndex ];

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

		graphicsContext.endRenderPass();

		if( parameters.enableBloom )
		{
			PostProcessBloomRenderParameters bloomParameters;
			bloomParameters.pSourceData				= &m_offscreenColorData;
			bloomParameters.pSelfIlluminationData	= nullptr;
			color::toVector3( bloomParameters.cutoffThreshold, parameters.bloomCutoffThreshold );
			m_bloom.render( graphicsContext, bloomParameters );
		}

		graphicsContext.beginRenderPass( graphicsContext.getBackBuffer() );

		graphicsContext.setVertexShader( m_pCompositeShader->getShader( ShaderType_VertexShader, 0u ) );
		graphicsContext.setPixelShader( m_pCompositeShader->getShader( ShaderType_PixelShader, 0u ) );

		graphicsContext.setBlendState( m_pBlendState );
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

	void Renderer2d::setTargetZoom( float zoom )
	{
		m_targetZoom = zoom;
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
}
