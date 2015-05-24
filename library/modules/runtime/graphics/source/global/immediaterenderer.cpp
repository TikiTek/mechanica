
#include "tiki/graphics/immediaterenderer.hpp"

#include "tiki/base/numbers.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/immediaterenderer_shader.hpp"
#include "tiki/graphics/primitivetopologies.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/math/camera.hpp"
#include "tiki/math/projection.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/math/box.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	ImmediateRenderer::ImmediateRenderer()
	{
		m_pContext = nullptr;

		m_pShaderSet = nullptr;

		m_pBlendState[ ImmediateBlendState_None ] = nullptr;
		m_pBlendState[ ImmediateBlendState_Add ] = nullptr;
		m_pDepthStencilState[ ImmediateDepthState_TestOffWriteOff ] = nullptr;
		m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOff ] = nullptr;
		m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOn ] = nullptr;
		m_pRasterizerState		= nullptr;
		m_pSamplerState			= nullptr;

		m_pVertexFormat			= nullptr;
		m_pVertexInputBinding	= nullptr;
	}

	ImmediateRenderer::~ImmediateRenderer()
	{
		TIKI_ASSERT( m_pContext == nullptr );
		TIKI_ASSERT( m_pShaderSet == nullptr );

		TIKI_ASSERT( m_pBlendState[ ImmediateBlendState_None ]						== nullptr );
		TIKI_ASSERT( m_pBlendState[ ImmediateBlendState_Add ]						== nullptr );
		TIKI_ASSERT( m_pDepthStencilState[ ImmediateDepthState_TestOffWriteOff ]	== nullptr );
		TIKI_ASSERT( m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOff ]		== nullptr );
		TIKI_ASSERT( m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOn ]		== nullptr );
		TIKI_ASSERT( m_pRasterizerState		== nullptr );
		TIKI_ASSERT( m_pSamplerState		== nullptr );

		TIKI_ASSERT( m_pVertexFormat		== nullptr );
		TIKI_ASSERT( m_pVertexInputBinding	== nullptr );
	}

	bool ImmediateRenderer::create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		m_pShaderSet = resourceManager.loadResource< ShaderSet >( "immediate.shader" );
		if ( m_pShaderSet == nullptr )
		{
			return false;
		}

		{
			const VertexAttribute attributes[] =
			{
				{ VertexSementic_Position,	0u,	VertexAttributeFormat_x32y32z32_float,	0u, VertexInputType_PerVertex },
				{ VertexSementic_TexCoord,	0u,	VertexAttributeFormat_x16y16_unorm,		0u, VertexInputType_PerVertex },
				{ VertexSementic_Color,		0u,	VertexAttributeFormat_x8y8z8w8_unorm,	0u, VertexInputType_PerVertex }
			};		

			m_pVertexFormat = graphicsSystem.createVertexFormat( attributes, TIKI_COUNT( attributes ) );
			if ( m_pVertexFormat == nullptr )
			{
				dispose( graphicsSystem, resourceManager );
				return false;
			}

			m_pVertexInputBinding = graphicsSystem.createVertexInputBinding( m_pShaderSet->getShader( ShaderType_VertexShader, 0u ), m_pVertexFormat );
			if ( m_pVertexInputBinding == nullptr )
			{
				dispose( graphicsSystem, resourceManager );
				return false;
			}
		}

		m_pBlendState[ ImmediateBlendState_None ]	= graphicsSystem.createBlendState( false, Blend_One, Blend_Zero, BlendOperation_Add, ColorWriteMask_All );
		m_pBlendState[ ImmediateBlendState_Add ]	= graphicsSystem.createBlendState( true, Blend_SourceAlpha, Blend_InverseSourceAlpha, BlendOperation_Add, ColorWriteMask_All );

		m_pDepthStencilState[ ImmediateDepthState_TestOffWriteOff ]	= graphicsSystem.createDepthStencilState( false, false );
		m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOff ]	= graphicsSystem.createDepthStencilState( true, false );
		m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOn ]	= graphicsSystem.createDepthStencilState( true, true );

		m_pRasterizerState		= graphicsSystem.createRasterizerState( FillMode_Solid, CullMode_Back, WindingOrder_Clockwise );
		m_pSamplerState			= graphicsSystem.createSamplerState( SamplerStateParamters() );

		if ( m_vertexConstantBuffer.create( graphicsSystem, sizeof( ImmediateRendererConstantData ) ) == false )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}
		
		return true;
	}

	void ImmediateRenderer::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		TIKI_ASSERT( m_pContext == nullptr );

		resourceManager.unloadResource( m_pShaderSet );
		m_pShaderSet = nullptr;

		graphicsSystem.disposeBlendState( m_pBlendState[ ImmediateBlendState_None ] );
		graphicsSystem.disposeBlendState( m_pBlendState[ ImmediateBlendState_Add ] );
		graphicsSystem.disposeDepthStencilState( m_pDepthStencilState[ ImmediateDepthState_TestOffWriteOff ] );
		graphicsSystem.disposeDepthStencilState( m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOff ] );
		graphicsSystem.disposeDepthStencilState( m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOn ] );
		graphicsSystem.disposeRasterizerState( m_pRasterizerState );
		graphicsSystem.disposeSamplerState( m_pSamplerState );
		m_pBlendState[ ImmediateBlendState_None ] = nullptr;
		m_pBlendState[ ImmediateBlendState_Add ] = nullptr;
		m_pDepthStencilState[ ImmediateDepthState_TestOffWriteOff ] = nullptr;
		m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOff ] = nullptr;
		m_pDepthStencilState[ ImmediateDepthState_TestOnWriteOn ] = nullptr;
		m_pRasterizerState = nullptr;
		m_pSamplerState = nullptr;

		graphicsSystem.disposeVertexInputBinding( m_pVertexInputBinding );
		m_pVertexInputBinding = nullptr;

		graphicsSystem.disposeVertexFormat( m_pVertexFormat );
		m_pVertexFormat = nullptr;

		m_vertexConstantBuffer.dispose( graphicsSystem );
	}

	void ImmediateRenderer::beginRendering( GraphicsContext& graphicsContext ) const
	{
		TIKI_ASSERT( m_pContext == nullptr );
		m_pContext = &graphicsContext;
	}

	void ImmediateRenderer::endRendering() const
	{
		TIKI_ASSERT( m_pContext != nullptr );
		m_pContext = nullptr;
	}

	void ImmediateRenderer::beginRenderPass( const RenderTarget* pRenderTarget /*= nullptr*/, const Camera* pCamera /*= nullptr*/ ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );

		if ( pRenderTarget == nullptr )
		{
			pRenderTarget = &m_pContext->getBackBuffer();
		}
		m_pContext->beginRenderPass( *pRenderTarget );
			
		Matrix44 transformMatrix;
		if ( pCamera == nullptr )
		{
			Projection projection;
			projection.createOrthographicCenter(
				(float)pRenderTarget->getWidth(),
				-(float)pRenderTarget->getHeight(),
				0.0f,
				1.0f
			);
			transformMatrix = projection.getMatrix();
		}
		else
		{
			transformMatrix = pCamera->getViewProjectionMatrix();
		}

		ImmediateRendererConstantData* pConstantData = static_cast< ImmediateRendererConstantData* >( m_pContext->mapBuffer( m_vertexConstantBuffer ) );
		TIKI_ASSERT( pConstantData != nullptr );
		createGraphicsMatrix44( pConstantData->projection, transformMatrix );
		m_pContext->unmapBuffer( m_vertexConstantBuffer );

		setState();
	}

	void ImmediateRenderer::endRenderPass() const
	{
		TIKI_ASSERT( m_pContext != nullptr );
		m_pContext->endRenderPass();
	}

	void ImmediateRenderer::setBlendState( ImmediateBlendState blendState ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );
		TIKI_ASSERT( blendState < ImmediateBlendState_Count );
		m_pContext->setBlendState( m_pBlendState[ blendState ] );
	}

	void ImmediateRenderer::setDepthState( ImmediateDepthState depthState ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );
		TIKI_ASSERT( depthState < ImmediateDepthState_Count );
		m_pContext->setDepthStencilState( m_pDepthStencilState[ depthState ] );
	}

	void ImmediateRenderer::drawRectangle( const Rectangle& d, Color color /*= TIKI_COLOR_WHITE */ ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );

		const float posLeft		= d.x;
		const float posRight	= d.x + d.width;
		const float posTop		= d.y;
		const float posBottom	= d.y + d.height;

		m_pContext->setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 2u ) );

		ImmediateVertex* pVertices = static_cast< ImmediateVertex* >( m_pContext->beginImmediateGeometry( sizeof( ImmediateVertex ), 4u ) );

		// bottom left
		createFloat3( pVertices[ 0u ].position, posLeft, posBottom, 0.0f );
		pVertices[ 0u ].u			= 0u;
		pVertices[ 0u ].v			= 0u;
		pVertices[ 0u ].color		= color;

		// top left
		createFloat3( pVertices[ 1u ].position, posLeft, posTop, 0.0f );
		pVertices[ 1u ].u			= 0u;
		pVertices[ 1u ].v			= 0u;
		pVertices[ 1u ].color		= color;

		// bottom right
		createFloat3( pVertices[ 2u ].position, posRight, posBottom, 0.0f );
		pVertices[ 2u ].u			= 0u;
		pVertices[ 2u ].v			= 0u;
		pVertices[ 2u ].color		= color;

		// top right
		createFloat3( pVertices[ 3u ].position, posRight, posTop, 0.0f );
		pVertices[ 3u ].u			= 0u;
		pVertices[ 3u ].v			= 0u;
		pVertices[ 3u ].color		= color;

		m_pContext->endImmediateGeometry();
	}

	void ImmediateRenderer::drawTexturedRectangle( const TextureData& texture, const Rectangle& d, Color color /*= TIKI_COLOR_WHITE*/  ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );

		const float posLeft		= d.x;
		const float posRight	= d.x + d.width;
		const float posTop		= d.y;
		const float posBottom	= d.y + d.height;

		m_pContext->setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 0u ) );
		m_pContext->setPixelShaderTexture( 0u, &texture );

		ImmediateVertex* pVertices = static_cast< ImmediateVertex* >( m_pContext->beginImmediateGeometry( sizeof( ImmediateVertex ), 4u ) );

		// bottom left
		createFloat3( pVertices[ 0u ].position, posLeft, posBottom, 0.0f );
		pVertices[ 0u ].u			= u16::floatToUnorm( 0.0f );
		pVertices[ 0u ].v			= u16::floatToUnorm( 1.0f );
		pVertices[ 0u ].color		= color;

		// top left
		createFloat3( pVertices[ 1u ].position, posLeft, posTop, 0.0f );
		pVertices[ 1u ].u			= u16::floatToUnorm( 0.0f );
		pVertices[ 1u ].v			= u16::floatToUnorm( 0.0f );
		pVertices[ 1u ].color		= color;

		// bottom right
		createFloat3( pVertices[ 2u ].position, posRight, posBottom, 0.0f );
		pVertices[ 2u ].u			= u16::floatToUnorm( 1.0f );
		pVertices[ 2u ].v			= u16::floatToUnorm( 1.0f );
		pVertices[ 2u ].color		= color;

		// top right
		createFloat3( pVertices[ 3u ].position, posRight, posTop, 0.0f );
		pVertices[ 3u ].u			= u16::floatToUnorm( 1.0f );
		pVertices[ 3u ].v			= u16::floatToUnorm( 0.0f );
		pVertices[ 3u ].color		= color;

		m_pContext->endImmediateGeometry();
	}

	void ImmediateRenderer::drawTexturedRectangle( const TextureData& texture, const Rectangle& d, const Rectangle& s, Color color /*= TIKI_COLOR_WHITE*/ ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );

		const float uScale = 1.0f / (float)texture.getWidth();
		const float vScale = 1.0f / (float)texture.getHeight();

		const float uRight	= s.x + s.width;
		const float vBottom	= s.y + s.height;

		const float posLeft		= d.x;
		const float posRight	= d.x + d.width;
		const float posTop		= d.y;
		const float posBottom	= d.y + d.height;

		m_pContext->setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 0u ) );
		m_pContext->setPixelShaderTexture( 0u, &texture );

		ImmediateVertex* pVertices = static_cast< ImmediateVertex* >( m_pContext->beginImmediateGeometry( sizeof( ImmediateVertex ), 4u ) );

		// bottom left
		createFloat3( pVertices[ 0u ].position, posLeft, posBottom, 0.0f );
		pVertices[ 0u ].u			= u16::floatToUnorm( s.x * uScale );
		pVertices[ 0u ].v			= u16::floatToUnorm( vBottom * vScale );
		pVertices[ 0u ].color		= color;

		// top left
		createFloat3( pVertices[ 1u ].position, posLeft, posTop, 0.0f );
		pVertices[ 1u ].u			= u16::floatToUnorm( s.x * uScale );
		pVertices[ 1u ].v			= u16::floatToUnorm( s.y * vScale );
		pVertices[ 1u ].color		= color;

		// bottom right
		createFloat3( pVertices[ 2u ].position, posRight, posBottom, 0.0f );
		pVertices[ 2u ].u			= u16::floatToUnorm( uRight * uScale );
		pVertices[ 2u ].v			= u16::floatToUnorm( vBottom * vScale );
		pVertices[ 2u ].color		= color;

		// top right
		createFloat3( pVertices[ 3u ].position, posRight, posTop, 0.0f );
		pVertices[ 3u ].u			= u16::floatToUnorm( uRight * uScale );
		pVertices[ 3u ].v			= u16::floatToUnorm( s.y * vScale );
		pVertices[ 3u ].color		= color;

		m_pContext->endImmediateGeometry();
	}

	void ImmediateRenderer::drawText( const Vector2& position, const Font& font, const char* pText, Color color /*= TIKI_COLOR_WHITE*/ ) const
	{
		TIKI_ASSERT( m_pContext != nullptr );

		const uint textLength = getStringLength( pText );
		if ( textLength == 0u )
		{
			return;
		}

		TIKI_ASSERT( textLength <= 128u );
		FontChar chars[ 128u ];
		font.fillVertices( chars, TIKI_COUNT( chars ), pText, textLength );

		m_pContext->setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 1u ) );
		m_pContext->setPixelShaderTexture( 0u, &font.getTextureData() );

		const size_t vertexCount = textLength * 4u;
		ImmediateVertex* pVertices = static_cast< ImmediateVertex* >( m_pContext->beginImmediateGeometry( sizeof( ImmediateVertex ), vertexCount ) );

		float x = 0.0f;
		for (size_t charIndex = 0u; charIndex < textLength; charIndex++)
		{
			const size_t vertexIndex = charIndex * 4u;
			const FontChar& character = chars[ charIndex ];

			const float charWidth	= character.width;
			const float charHeight	= character.height;

			const float posLeft		= position.x + x;
			const float posRight	= posLeft + charWidth;
			const float posTop		= position.y;
			const float posBottom	= position.y + charHeight;

			// bottom left
			createFloat3( pVertices[ vertexIndex + 0u ].position, posLeft, posBottom, 0.0f );
			pVertices[ vertexIndex + 0u ].u		= character.x1; 
			pVertices[ vertexIndex + 0u ].v		= character.y2; 
			pVertices[ vertexIndex + 0u ].color	= color;

			// top left
			createFloat3( pVertices[ vertexIndex + 1u ].position, posLeft, posTop, 0.0f );
			pVertices[ vertexIndex + 1u ].u		= character.x1; 
			pVertices[ vertexIndex + 1u ].v		= character.y1; 
			pVertices[ vertexIndex + 1u ].color	= color;

			// bottom right
			createFloat3( pVertices[ vertexIndex + 2u ].position, posRight, posBottom, 0.0f );
			pVertices[ vertexIndex + 2u ].u		= character.x2; 
			pVertices[ vertexIndex + 2u ].v		= character.y2; 
			pVertices[ vertexIndex + 2u ].color	= color;

			// top right
			createFloat3( pVertices[ vertexIndex + 3u ].position, posRight, posTop, 0.0f );
			pVertices[ vertexIndex + 3u ].u		= character.x2; 
			pVertices[ vertexIndex + 3u ].v		= character.y1; 
			pVertices[ vertexIndex + 3u ].color	= color;
		
			x += charWidth;
		}

		m_pContext->endImmediateGeometry();
	}
	
	void ImmediateRenderer::drawLines( const Vector3* pPoints, uint capacity, Color color /*= TIKI_COLOR_WHITE */ ) const
	{
		TIKI_ASSERT( pPoints != nullptr );
		TIKI_ASSERT( m_pContext != nullptr );

		m_pContext->setPrimitiveTopology( PrimitiveTopology_LineList );

		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 2u ) );

		ImmediateVertex* pVertices = static_cast< ImmediateVertex* >( m_pContext->beginImmediateGeometry( sizeof( ImmediateVertex ), capacity ) );

		for (uint i = 0u; i < capacity; ++i)
		{
			ImmediateVertex& targetVertex	= pVertices[ i ];
			const Vector3& sourcePoint		= pPoints[ i ];

			createFloat3( targetVertex.position, sourcePoint.x, sourcePoint.y, sourcePoint.z );
			targetVertex.u		= 0u;
			targetVertex.v		= 0u;
			targetVertex.color	= color;
		} 

		m_pContext->endImmediateGeometry();
	}

	void ImmediateRenderer::drawRay( const Ray3& ray, float length /* = 100.0f */, Color color /*= TIKI_COLOR_WHITE */ ) const
	{
		Vector3 scaledDir = ray.Direction;
		vector::scale( scaledDir, length );

		Vector3 end = ray.Origin;
		vector::add( end, scaledDir );

		const Vector3 points[ ] = { ray.Origin, end };
		drawLines( points, TIKI_COUNT( points ), color );
	}

	void ImmediateRenderer::drawBox( const Box& box, Color color /*= TIKI_COLOR_WHITE */ ) const
	{
		m_pContext->setPrimitiveTopology( PrimitiveTopology_LineList );

		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 2u ) );

		Vector3 boxVertices[8];
		box.getVertices( &boxVertices[0] );

		uint vertexCount = 24;
		ImmediateVertex* pVertices = static_cast<ImmediateVertex*>(m_pContext->beginImmediateGeometry( sizeof(ImmediateVertex), vertexCount ));
		
		// draw lower rect
		createFloat3( pVertices[ 0 ].position, boxVertices[ 0 ].x, boxVertices[ 0 ].y, boxVertices[ 0 ].z );
		createFloat3( pVertices[ 1 ].position, boxVertices[ 1 ].x, boxVertices[ 1 ].y, boxVertices[ 1 ].z );
		
		createFloat3( pVertices[ 2 ].position, boxVertices[ 1 ].x, boxVertices[ 1 ].y, boxVertices[ 1 ].z );
		createFloat3( pVertices[ 3 ].position, boxVertices[ 2 ].x, boxVertices[ 2 ].y, boxVertices[ 2 ].z );

		createFloat3( pVertices[ 4 ].position, boxVertices[ 2 ].x, boxVertices[ 2 ].y, boxVertices[ 2 ].z );
		createFloat3( pVertices[ 5 ].position, boxVertices[ 3 ].x, boxVertices[ 3 ].y, boxVertices[ 3 ].z );
		
		createFloat3( pVertices[ 6 ].position, boxVertices[ 3 ].x, boxVertices[ 3 ].y, boxVertices[ 3 ].z );
		createFloat3( pVertices[ 7 ].position, boxVertices[ 0 ].x, boxVertices[ 0 ].y, boxVertices[ 0 ].z );

		// draw upper rect
		createFloat3( pVertices[ 8 ].position, boxVertices[ 4 ].x, boxVertices[ 4 ].y, boxVertices[ 4 ].z );
		createFloat3( pVertices[ 9 ].position, boxVertices[ 5 ].x, boxVertices[ 5 ].y, boxVertices[ 5 ].z );

		createFloat3( pVertices[ 10 ].position, boxVertices[ 5 ].x, boxVertices[ 5 ].y, boxVertices[ 5 ].z );
		createFloat3( pVertices[ 11 ].position, boxVertices[ 6 ].x, boxVertices[ 6 ].y, boxVertices[ 6 ].z );

		createFloat3( pVertices[ 12 ].position, boxVertices[ 6 ].x, boxVertices[ 6 ].y, boxVertices[ 6 ].z );
		createFloat3( pVertices[ 13 ].position, boxVertices[ 7 ].x, boxVertices[ 7 ].y, boxVertices[ 7 ].z );

		createFloat3( pVertices[ 14 ].position, boxVertices[ 7 ].x, boxVertices[ 7 ].y, boxVertices[ 7 ].z );
		createFloat3( pVertices[ 15 ].position, boxVertices[ 4 ].x, boxVertices[ 4 ].y, boxVertices[ 4 ].z );


		// draw vertical lines
		createFloat3( pVertices[ 16 ].position, boxVertices[ 0 ].x, boxVertices[ 0 ].y, boxVertices[ 0 ].z );
		createFloat3( pVertices[ 17 ].position, boxVertices[ 4 ].x, boxVertices[ 4 ].y, boxVertices[ 4 ].z );
		
		createFloat3( pVertices[ 18 ].position, boxVertices[ 1 ].x, boxVertices[ 1 ].y, boxVertices[ 1 ].z );
		createFloat3( pVertices[ 19 ].position, boxVertices[ 5 ].x, boxVertices[ 5 ].y, boxVertices[ 5 ].z );
		
		createFloat3( pVertices[ 20 ].position, boxVertices[ 2 ].x, boxVertices[ 2 ].y, boxVertices[ 2 ].z );
		createFloat3( pVertices[ 21 ].position, boxVertices[ 6 ].x, boxVertices[ 6 ].y, boxVertices[ 6 ].z );

		createFloat3( pVertices[ 22 ].position, boxVertices[ 3 ].x, boxVertices[ 3 ].y, boxVertices[ 3 ].z );
		createFloat3( pVertices[ 23 ].position, boxVertices[ 7 ].x, boxVertices[ 7 ].y, boxVertices[ 7 ].z );

		// set color and uv
		for ( uint i = 0u; i < vertexCount; ++i )
		{
			ImmediateVertex& current = pVertices[ i ];
			current.color	= color;
			current.u		= 0u;
			current.v		= 0u;
		}

		m_pContext->endImmediateGeometry();
	}

	void ImmediateRenderer::drawGrid( int gridSpacing /*= 10*/, int gridSize /*= 512*/, Color color /*= TIKI_COLOR_WHITE */ ) const
	{
		if ( gridSpacing == 0 )
		{
			return;
		}

		m_pContext->setPrimitiveTopology( PrimitiveTopology_LineList );

		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 2u ) );

		uint vertexCount = ((gridSize / gridSpacing) * 8) + 4;

		ImmediateVertex* pVertices = static_cast<ImmediateVertex*>(m_pContext->beginImmediateGeometry( sizeof(ImmediateVertex), vertexCount ));

		// fill array
		uint idx = 0;
		for ( int i = 1; i < (gridSize / gridSpacing) + 1; i++ )
		{
			createFloat3( pVertices[ idx++ ].position, (float)(  i * gridSpacing ), 0.0f, (float)(  gridSize ) );
			createFloat3( pVertices[ idx++ ].position, (float)(  i * gridSpacing ), 0.0f, (float)( -gridSize ) );
			createFloat3( pVertices[ idx++ ].position, (float)( -i * gridSpacing ), 0.0f, (float)(  gridSize ) );
			createFloat3( pVertices[ idx++ ].position, (float)( -i * gridSpacing ), 0.0f, (float)( -gridSize ) );

			createFloat3( pVertices[ idx++ ].position, (float)(  gridSize ), 0.0f, (float)( i * gridSpacing ) );
			createFloat3( pVertices[ idx++ ].position, (float)( -gridSize ), 0.0f, (float)( i * gridSpacing ) );
			createFloat3( pVertices[ idx++ ].position, (float)(  gridSize ), 0.0f, (float)(-i * gridSpacing ) );
			createFloat3( pVertices[ idx++ ].position, (float)( -gridSize ), 0.0f, (float)(-i * gridSpacing ) );
		}

		// set color and uv
		for ( uint i = 0u; i < idx; ++i )
		{
			ImmediateVertex& current = pVertices[ i ];
			current.color = color;
			current.u = 0u;
			current.v = 0u;
		}

		// add highlights
		createFloat3( pVertices[ idx ].position, 0.0f, 0.0f, (float)( -gridSize ) );
		pVertices[ idx++ ].color = TIKI_COLOR_GREEN;
		createFloat3( pVertices[ idx ].position, 0.0f, 0.0f, (float)( gridSize ) );
		pVertices[ idx++ ].color = TIKI_COLOR_GREEN;
		createFloat3( pVertices[ idx ].position, (float)( gridSize ), 0.0f, 0.0f );
		pVertices[ idx++ ].color = TIKI_COLOR_GREEN;
		createFloat3( pVertices[ idx ].position, (float)( -gridSize ), 0.0f, 0.0f );
		pVertices[ idx++ ].color = TIKI_COLOR_GREEN;

		m_pContext->endImmediateGeometry();
	}

	void ImmediateRenderer::drawAxes( float lineLength, float lineOffset, const Matrix43& worldMatrix ) const
	{
		m_pContext->setPrimitiveTopology( PrimitiveTopology_LineList );

		m_pContext->setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, 2u ) );

		uint vertexCount = 14 * 3;

		ImmediateVertex* pVertices = static_cast<ImmediateVertex*>(m_pContext->beginImmediateGeometry( sizeof(ImmediateVertex), vertexCount ));
		
		float halfLineOffset	= lineOffset * 0.5f;
		float quarterLineLength = lineLength * 0.25f;
		float halfLineLength	= lineLength * 0.5f; 

		uint idx = 0;

		// -- X Axis -- 
		createFloat3( pVertices[ idx++ ].position, halfLineOffset, 0, 0 );
		createFloat3( pVertices[ idx++ ].position, lineLength, 0, 0 );				
		createFloat3( pVertices[ idx++ ].position, lineOffset, 0, 0 );
		createFloat3( pVertices[ idx++ ].position, lineOffset, lineOffset, 0 );
		createFloat3( pVertices[ idx++ ].position, lineOffset, 0, 0 );
		createFloat3( pVertices[ idx++ ].position, lineOffset, 0, lineOffset );

		// arrow
		createFloat3( pVertices[ idx++ ].position, lineLength, 0, 0 );
		createFloat3( pVertices[ idx++ ].position, lineLength - quarterLineLength, 0.0f, quarterLineLength );
		createFloat3( pVertices[ idx++ ].position, lineLength, 0, 0 );
		createFloat3( pVertices[ idx++ ].position, lineLength - quarterLineLength, 0.0f, -quarterLineLength );

		createFloat3( pVertices[ idx++ ].position, lineLength, 0, 0 );
		createFloat3( pVertices[ idx++ ].position, lineLength - quarterLineLength, quarterLineLength, 0.0f );
		createFloat3( pVertices[ idx++ ].position, lineLength, 0, 0 );
		createFloat3( pVertices[ idx++ ].position, lineLength - quarterLineLength, -quarterLineLength, 0.0f );


		// set color
		for ( uint i = 0u; i < idx; ++i )
		{
			ImmediateVertex& current = pVertices[ i ];
			current.color = TIKI_COLOR_RED;
		}

		uint lastIdx = idx;

		// -- Y Axis -- 
		createFloat3( pVertices[ idx++ ].position, 0, halfLineOffset, 0 );				
		createFloat3( pVertices[ idx++ ].position, 0, lineLength, 0 );
		createFloat3( pVertices[ idx++ ].position, 0, lineOffset, 0 );
		createFloat3( pVertices[ idx++ ].position, lineOffset, lineOffset, 0 );
		createFloat3( pVertices[ idx++ ].position, 0, lineOffset, 0 );
		createFloat3( pVertices[ idx++ ].position, 0, lineOffset, lineOffset );

		// arrow
		createFloat3( pVertices[ idx++ ].position, 0, lineLength, 0 );
		createFloat3( pVertices[ idx++ ].position, quarterLineLength, lineLength - quarterLineLength, 0 );
		createFloat3( pVertices[ idx++ ].position, 0, lineLength, 0 );
		createFloat3( pVertices[ idx++ ].position, -quarterLineLength, lineLength - quarterLineLength, 0 );

		createFloat3( pVertices[ idx++ ].position, 0, lineLength, 0 );
		createFloat3( pVertices[ idx++ ].position, 0, lineLength - quarterLineLength, quarterLineLength );
		createFloat3( pVertices[ idx++ ].position, 0, lineLength, 0 );
		createFloat3( pVertices[ idx++ ].position, 0, lineLength - quarterLineLength, -quarterLineLength );

		// set color
		for ( uint i = lastIdx + 1; i < idx; ++i )
		{
			ImmediateVertex& current = pVertices[ i ];
			current.color = TIKI_COLOR_GREEN;
		}

		lastIdx = idx;


		// -- Z Axis --
		createFloat3( pVertices[ idx++ ].position, 0, 0, halfLineOffset );				
		createFloat3( pVertices[ idx++ ].position, 0, 0, lineLength  );
		createFloat3( pVertices[ idx++ ].position, 0, 0, lineOffset  );
		createFloat3( pVertices[ idx++ ].position, lineOffset, 0, lineOffset );
		createFloat3( pVertices[ idx++ ].position, 0, 0, lineOffset );
		createFloat3( pVertices[ idx++ ].position, 0, lineOffset, lineOffset );

		// arrow
		createFloat3( pVertices[ idx++ ].position, 0, 0, lineLength );
		createFloat3( pVertices[ idx++ ].position, 0, quarterLineLength, lineLength - quarterLineLength );
		createFloat3( pVertices[ idx++ ].position, 0, 0, lineLength );
		createFloat3( pVertices[ idx++ ].position, 0, -quarterLineLength, lineLength - quarterLineLength );

		createFloat3( pVertices[ idx++ ].position, 0, 0, lineLength );
		createFloat3( pVertices[ idx++ ].position, quarterLineLength, 0, lineLength - quarterLineLength );
		createFloat3( pVertices[ idx++ ].position, 0, 0, lineLength );
		createFloat3( pVertices[ idx++ ].position, -quarterLineLength, 0, lineLength - quarterLineLength );

		//TIKI_ASSERT( vertexCount == idx );

		// set color
		for ( uint i = lastIdx + 1; i < vertexCount; ++i )
		{
			ImmediateVertex& current = pVertices[ i ];
			current.color = TIKI_COLOR_BLUE;
		}


		// todo: set immediate renderer world matrix?
		for ( uint i = 0; i < vertexCount; ++i )
		{
			Vector3 transformedPos;
			vector::set( transformedPos, pVertices[ i ].position.x, pVertices[ i ].position.y, pVertices[ i ].position.z );

			matrix::transform( transformedPos, worldMatrix );

			pVertices[ i ].position.x = transformedPos.x;
			pVertices[ i ].position.y = transformedPos.y;
			pVertices[ i ].position.z = transformedPos.z;
		}

		m_pContext->endImmediateGeometry();
	}

	void ImmediateRenderer::setState() const
	{
		TIKI_ASSERT( m_pContext != nullptr );

		m_pContext->setVertexShader( m_pShaderSet->getShader( ShaderType_VertexShader, 0u ) );
		m_pContext->setVertexShaderConstant( 0u, m_vertexConstantBuffer );

		m_pContext->setBlendState( m_pBlendState[ ImmediateBlendState_Add ] );
		m_pContext->setDepthStencilState( m_pDepthStencilState[ ImmediateDepthState_TestOffWriteOff ] );
		m_pContext->setRasterizerState( m_pRasterizerState );
		m_pContext->setVertexInputBinding( m_pVertexInputBinding );

		m_pContext->setPixelShaderSamplerState( 0u, m_pSamplerState );

		setBlendState( ImmediateBlendState_Add );
		setDepthState( ImmediateDepthState_TestOffWriteOff );
	}



}



