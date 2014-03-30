
#include "tiki/graphics/immediaterenderer.hpp"

#include "tiki/base/numbers.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/immediaterenderer_shader.hpp"
#include "tiki/graphics/primitivetopologies.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/math/projection.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	static TIKI_FORCE_INLINE float toClipSpaceX( float x )
	{
		return -1.0f + x * 2.0f;
	}

	static TIKI_FORCE_INLINE float toClipSpaceY( float y )
	{
		return 1.0f - y * 2.0f;
	}

	ImmediateRenderer::ImmediateRenderer()
	{
		m_pShaderSet			= nullptr;

		m_pBlendState			= nullptr;
		m_pDepthStencilState	= nullptr;
		m_pRasterizerState		= nullptr;
		m_pSamplerState			= nullptr;

		m_pVertexFormat			= nullptr;
		m_pVertexInputBinding	= nullptr;
	}

	ImmediateRenderer::~ImmediateRenderer()
	{
		TIKI_ASSERT( m_pShaderSet			== nullptr );

		TIKI_ASSERT( m_pBlendState			== nullptr );
		TIKI_ASSERT( m_pDepthStencilState	== nullptr );
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

		m_pBlendState			= graphicsSystem.createBlendState( true, Blend_SourceAlpha, Blend_InverseSourceAlpha, BlendOperation_Add, ColorWriteMask_All );
		m_pDepthStencilState	= graphicsSystem.createDepthStencilState( false, false );
		m_pRasterizerState		= graphicsSystem.createRasterizerState( FillMode_Solid, CullMode_Back, WindingOrder_Clockwise );
		m_pSamplerState			= graphicsSystem.createSamplerState( SamplerStateParamters() );

		if ( m_sprites.create( MaxSprites ) == false || m_vertices.create( MaxVertices ) == false )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		if ( m_vertexBuffer.create( graphicsSystem, MaxVertices, sizeof( SpriteVertex ), true ) == false )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}

		if ( m_constantBuffer.create( graphicsSystem, sizeof( ImmediateRendererConstantData ) ) == false )
		{
			dispose( graphicsSystem, resourceManager );
			return false;
		}
		
		Projection projection;
		projection.createOrthographicCenter( 1.0f, -1.0f, 0.0f, 1.0f );

		createGraphicsMatrix44(
			m_constantData.projection,
			projection.getMatrix()
		);

		return true;
	}

	void ImmediateRenderer::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		resourceManager.unloadResource( m_pShaderSet );
		m_pShaderSet = nullptr;

		graphicsSystem.disposeBlendState( m_pBlendState );
		graphicsSystem.disposeDepthStencilState( m_pDepthStencilState );
		graphicsSystem.disposeRasterizerState( m_pRasterizerState );
		graphicsSystem.disposeSamplerState( m_pSamplerState );
		m_pBlendState = nullptr;
		m_pDepthStencilState = nullptr;
		m_pRasterizerState = nullptr;
		m_pSamplerState = nullptr;

		graphicsSystem.disposeVertexInputBinding( m_pVertexInputBinding );
		m_pVertexInputBinding = nullptr;

		graphicsSystem.disposeVertexFormat( m_pVertexFormat );
		m_pVertexFormat = nullptr;

		m_sprites.dispose();
		m_vertices.dispose();

		m_vertexBuffer.dispose( graphicsSystem );
		m_constantBuffer.dispose( graphicsSystem );
	}

	void ImmediateRenderer::setProjection( const Projection& projection )
	{
		createGraphicsMatrix44(
			m_constantData.projection,
			projection.getMatrix()
		);
	}

	void ImmediateRenderer::flush( GraphicsContext& graphicsContext )
	{
		if( m_sprites.getCount() > 0u )
		{
			const uint vertexCount	= m_vertices.getCount();
			const uint count		= m_sprites.getCount();

			{
				ImmediateRendererConstantData* pConstantData = static_cast< ImmediateRendererConstantData* >( graphicsContext.mapBuffer( m_constantBuffer ) );
				*pConstantData = m_constantData;
				graphicsContext.unmapBuffer( m_constantBuffer );

				SpriteVertex* pTargetVertexData = static_cast< SpriteVertex* >( graphicsContext.mapBuffer( m_vertexBuffer ) );
				memory::copy( pTargetVertexData, m_vertices.getData(), sizeof( SpriteVertex ) * vertexCount );
				graphicsContext.unmapBuffer( m_vertexBuffer );
			}

			graphicsContext.setVertexShader( m_pShaderSet->getShader( ShaderType_VertexShader, 0u ) );
			graphicsContext.setVertexShaderConstant( 0u, m_constantBuffer );

			graphicsContext.setBlendState( m_pBlendState );
			graphicsContext.setDepthStencilState( m_pDepthStencilState );
			graphicsContext.setRasterizerState( m_pRasterizerState );
			graphicsContext.setVertexInputBinding( m_pVertexInputBinding );

			graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerState );

			graphicsContext.setVertexBuffer( 0u, m_vertexBuffer );
			graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

			for( uint i = 0u; i < count; ++i )
			{
				const Sprite& sprite = m_sprites[ i ];

				graphicsContext.setPixelShader( m_pShaderSet->getShader( ShaderType_PixelShader, sprite.shaderBitMask ) );
				graphicsContext.setPixelShaderTexture( 0u, sprite.pTexture );				
				graphicsContext.drawGeometry( sprite.vertexCount, sprite.vertexOffset );
			}
		}

		m_sprites.clear();
		m_vertices.clear();
	}

	void ImmediateRenderer::drawTexture( const TextureData* pTexture, const Rectangle& d, Color color /*= TIKI_COLOR_WHITE*/  )
	{
		Sprite& sprite	= m_sprites.push();
		sprite.vertexOffset		= m_vertices.getCount();
		sprite.vertexCount		= 4u;
		sprite.shaderBitMask	= ( pTexture == nullptr ? 2u : 0u );
		sprite.pTexture			= pTexture;

		SpriteVertex* pVertices = m_vertices.pushRange( 4u );

		const float posLeft		= d.x;
		const float posRight	= d.x + d.width;
		const float posTop		= d.y;
		const float posBottom	= d.y + d.height;

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
	}

	void ImmediateRenderer::drawTexture( const TextureData* pTexture, const Rectangle& d, const Rectangle& s, Color color /*= TIKI_COLOR_WHITE*/ )
	{
		Sprite& sprite = m_sprites.push();
		sprite.vertexOffset		= m_vertices.getCount();
		sprite.vertexCount		= 4u;
		sprite.shaderBitMask	= ( pTexture == nullptr ? 2u : 0u );
		sprite.pTexture			= pTexture;

		float uScale = 0.0f;
		float vScale = 0.0f;
		if ( pTexture != nullptr )
		{
			uScale = 1.0f / (float)pTexture->getWidth();
			vScale = 1.0f / (float)pTexture->getHeight();
		}

		const float uRight	= s.x + s.width;
		const float vBottom	= s.y + s.height;

		const float posLeft		= d.x;
		const float posRight	= d.x + d.width;
		const float posTop		= d.y;
		const float posBottom	= d.y + d.height;

		SpriteVertex* pVertices = m_vertices.pushRange( 4u );

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
	}

	void ImmediateRenderer::drawText( const Vector2& position, const Font& font, const char* pText, Color color /*= TIKI_COLOR_WHITE*/ )
	{
		const uint textLength = getStringLength( pText );
		if ( textLength == 0u )
		{
			return;
		}

		TIKI_ASSERT( textLength <= 128u );
		const size_t vertexCount = textLength * 4u;

		Sprite& sprite = m_sprites.push();
		sprite.vertexOffset		= m_vertices.getCount();
		sprite.vertexCount		= vertexCount;
		sprite.shaderBitMask	= 1u;
		sprite.pTexture			= &font.getTextureData();

		FontChar chars[ 128u ];
		font.fillVertices( chars, TIKI_COUNT( chars ), pText, textLength );

		SpriteVertex* pVertices = m_vertices.pushRange( vertexCount );

		const float texelWidth	= 1.0f; //m_constantData.projection.data[ 0u ] / 2.0f;
		const float texelHeight	= 1.0f; //m_constantData.projection.data[ 5u ] / -2.0f;

		float x = 0.0f;
		for (size_t charIndex = 0u; charIndex < textLength; charIndex++)
		{
			const size_t vertexIndex = charIndex * 4u;
			const FontChar& character = chars[ charIndex ];

			const float charWidth	= character.width * texelWidth;
			const float charHeight	= character.height * texelHeight;

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
	}
}



