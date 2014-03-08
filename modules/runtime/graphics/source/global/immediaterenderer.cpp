
#include "tiki/graphics/immediaterenderer.hpp"

#include "tiki/base/numbers.hpp"
#include "tiki/graphics/font.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/immediaterenderer_shader.hpp"
#include "tiki/graphics/shaderset.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphicsbase/primitivetopologies.hpp"
#include "tiki/math/rectangle.hpp"
#include "tiki/resource/resourcemanager.hpp"

//#include "tiki/graphicsresources/material.hpp"
//#include "tiki/graphicsresources/shaderset.hpp"
//#include "tiki/math/rectangle.hpp"


namespace tiki
{
	ImmediateRenderer::~ImmediateRenderer()
	{
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
		}

		SamplerStateParamters samplerParams;
		m_pSamplerState = graphicsSystem.createSamplerState( samplerParams );

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
		
		return true;
	}

	void ImmediateRenderer::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		resourceManager.unloadResource( m_pShaderSet );
		m_pShaderSet = nullptr;

		graphicsSystem.disposeVertexFormat( m_pVertexFormat );
		m_pVertexFormat = nullptr;

		graphicsSystem.disposeSamplerState( m_pSamplerState );
		m_pSamplerState = nullptr;

		m_sprites.dispose();
		m_vertices.dispose();

		m_vertexBuffer.dispose( graphicsSystem );
		m_constantBuffer.dispose( graphicsSystem );
	}

	void ImmediateRenderer::flush( GraphicsContext& graphicsContext )
	{
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerState );

		//m_pGpuContext->disableDepth();
		//m_pGpuContext->enableAlpha();
				
		if( m_sprites.getCount() > 0u )
		{
			const uint vertexCount	= m_vertices.getCount();
			const uint count		= m_sprites.getCount();

			SpriteVertex* pTargetVertexData = static_cast< SpriteVertex* >( graphicsContext.mapBuffer( m_vertexBuffer ) );
			memory::copy( pTargetVertexData, m_sprites.getData(), sizeof( SpriteVertex ) * vertexCount );
			graphicsContext.unmapBuffer( m_vertexBuffer );

			graphicsContext.setVertexInputBinding( m_pVertexInputBinding );

			graphicsContext.setVertexBuffer( 0u, m_vertexBuffer );
			graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

			for( uint i = 0u; i < count; ++i )
			{
				const Sprite& sprite = m_sprites[ i ];

				graphicsContext.setPixelShaderTexture( 0u, sprite.pTexture );				
				graphicsContext.drawGeometry( sprite.vertexCount, sprite.vertexOffset );
			}
		}

		//// render text
		//if ( m_textChars.getCount() )
		//{
		//	FontVertex* pTextVertices = m_textVertexBuffer.map( m_textChars.getCount() );
		//	memory::copy( pTextVertices, m_textChars.getData(), m_textChars.getCount() * sizeof( FontVertex ) );
		//	m_textVertexBuffer.unmap();

		//	m_pGpuContext->setInputLayout( m_pTextVertexFormat );
		//	m_pGpuContext->setMaterial( m_pTextMaterial );

		//	m_pGpuContext->setVertexBuffer( m_textVertexBuffer );
		//	m_pGpuContext->setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

		//	size_t offset = 0u;
		//	while ( m_textTextures.getCount() )
		//	{
		//		const TextureData* pTextureData = m_textTextures.pop();
		//		const size_t vertexCount = m_textLength.pop() * 4u;
		//		
		//		m_pGpuContext->setPixelShaderTexture( pTextureData );
		//		m_pGpuContext->draw( vertexCount, offset );
		//		offset += vertexCount;
		//	}

		//	m_textChars.clear();
		//	TIKI_ASSERT( m_textLength.getCount() == 0u );
		//	TIKI_ASSERT( m_textTextures.getCount() == 0u );
		//}

		//m_pGpuContext->enableDepth();
		//m_pGpuContext->disableAlpha();

		m_sprites.clear();
		m_vertices.clear();
	}

	void tiki::ImmediateRenderer::drawTexture( const TextureData& texture, const Rectangle& r )
	{
		Sprite& sprite	= m_sprites.push();
		sprite.vertexOffset	= m_vertices.getCount();
		sprite.vertexCount	= 4u;
		sprite.pTexture		= &texture;

		SpriteVertex* pVertices = m_vertices.pushRange( 4u );

		// bottom left
		pVertices[ 0u ].position.x	= -1.0f + r.x;
		pVertices[ 0u ].position.y	=  1.0f  - r.y - r.height;
		pVertices[ 0u ].position.z	=  0.0f;
		pVertices[ 0u ].u			= u16::floatToUnorm( 0.0f );
		pVertices[ 0u ].v			= u16::floatToUnorm( 1.0f );
		pVertices[ 0u ].color		= TIKI_COLOR_WHITE;

		// top left
		pVertices[ 1u ].position.x	= -1.0f + r.x;
		pVertices[ 1u ].position.y	=  1.0f - r.y;
		pVertices[ 1u ].position.z	=  0.0f;
		pVertices[ 1u ].u			= u16::floatToUnorm( 0.0f );
		pVertices[ 1u ].v			= u16::floatToUnorm( 0.0f );
		pVertices[ 1u ].color		= TIKI_COLOR_WHITE;

		// bottom right
		pVertices[ 2u ].position.x	= -1.0f + r.x + r.width;
		pVertices[ 2u ].position.y	=  1.0f - r.y - r.height;
		pVertices[ 2u ].position.z	=  0.0f;
		pVertices[ 2u ].u			= u16::floatToUnorm( 1.0f );
		pVertices[ 2u ].v			= u16::floatToUnorm( 1.0f );
		pVertices[ 2u ].color		= TIKI_COLOR_WHITE;

		// top right
		pVertices[ 3u ].position.x	= -1.0f + r.x + r.width;
		pVertices[ 3u ].position.y	=  1.0f - r.y;
		pVertices[ 3u ].position.z	=  0.0f;
		pVertices[ 3u ].u			= u16::floatToUnorm( 1.0f );
		pVertices[ 3u ].v			= u16::floatToUnorm( 0.0f );
		pVertices[ 3u ].color		= TIKI_COLOR_WHITE;
	}

	void ImmediateRenderer::drawTexture( const TextureData& texture, const Rectangle& d, const Rectangle& s )
	{
		Sprite& sprite = m_sprites.push();
		sprite.vertexOffset	= m_vertices.getCount();
		sprite.vertexCount	= 4u;
		sprite.pTexture		= &texture;

		const float uScale = 1.0f / (float)texture.getWidth();
		const float vScale = 1.0f / (float)texture.getHeight();

		const float uRight	= s.x + s.width;
		const float vBottom	= s.y + s.height;

		const float posRight	= d.x + d.width;
		const float posBottom	= d.y + d.height;

		SpriteVertex* pVertices = m_vertices.pushRange( 4u );

		// bottom left
		createFloat3( pVertices[ 0u ].position, d.x, posBottom, 0.0f );
		pVertices[ 0u ].u			= u16::floatToUnorm( s.x * uScale );
		pVertices[ 0u ].v			= u16::floatToUnorm( vBottom * vScale );
		pVertices[ 0u ].color		= TIKI_COLOR_WHITE;

		// top left
		createFloat3( pVertices[ 1u ].position, d.x, d.y, 0.0f );
		pVertices[ 1u ].u			= u16::floatToUnorm( s.x * uScale );
		pVertices[ 1u ].v			= u16::floatToUnorm( s.y * vScale );
		pVertices[ 1u ].color		= TIKI_COLOR_WHITE;

		// bottom right
		createFloat3( pVertices[ 2u ].position, posRight, posBottom, 0.0f );
		pVertices[ 2u ].u			= u16::floatToUnorm( uRight * uScale );
		pVertices[ 2u ].v			= u16::floatToUnorm( vBottom * vScale );
		pVertices[ 2u ].color		= TIKI_COLOR_WHITE;

		// top right
		createFloat3( pVertices[ 3u ].position, posRight, d.y, 0.0f );
		pVertices[ 3u ].u			= u16::floatToUnorm( uRight * uScale );
		pVertices[ 3u ].v			= u16::floatToUnorm( s.y * vScale );
		pVertices[ 3u ].color		= TIKI_COLOR_WHITE;
	}

	void ImmediateRenderer::drawText( const Vector2& position, const Font& font, const char* pText, Color color )
	{
		const uint textLength = getStringLength( pText );
		if ( textLength == 0u )
		{
			return;
		}

		TIKI_ASSERT( textLength <= 128u );
		const size_t vertexCount = textLength * 4u;

		Sprite& sprite = m_sprites.push();
		sprite.vertexCount	= m_vertices.getCount();
		sprite.vertexCount	= vertexCount;
		sprite.pTexture		= &font.getTextureData();

		FontChar chars[ 128u ];
		font.fillVertices( chars, TIKI_COUNT( chars ), pText, textLength );

		SpriteVertex* pVertices = m_vertices.pushRange( vertexCount );

		float x = 0.0f;
		for (size_t i = 0u; i < vertexCount; i += 4u)
		{
			const size_t vertexIndex = i * 4u;
			const FontChar& character = chars[ i ];

			const float left = position.x + x;

			const float right = x + character.width;
			const float bottom = position.y + character.height;

			// bottom left
			createFloat3( pVertices[ vertexIndex + 0u ].position, left, bottom, 0.0f );
			pVertices[ vertexIndex + 0u ].u		= character.x1; 
			pVertices[ vertexIndex + 0u ].v		= character.y2; 
			pVertices[ vertexIndex + 0u ].color	= color;

			// top left
			createFloat3( pVertices[ vertexIndex + 1u ].position, left, position.y, 0.0f );
			pVertices[ vertexIndex + 1u ].u		= character.x1; 
			pVertices[ vertexIndex + 1u ].v		= character.y1; 
			pVertices[ vertexIndex + 1u ].color	= color;

			// bottom right
			createFloat3( pVertices[ vertexIndex + 2u ].position, right, bottom, 0.0f );
			pVertices[ vertexIndex + 2u ].u		= character.x2; 
			pVertices[ vertexIndex + 2u ].v		= character.y2; 
			pVertices[ vertexIndex + 2u ].color	= color;

			// top right
			createFloat3( pVertices[ vertexIndex + 3u ].position, right, position.y, 0.0f );
			pVertices[ vertexIndex + 3u ].u		= character.x2; 
			pVertices[ vertexIndex + 3u ].v		= character.y1; 
			pVertices[ vertexIndex + 3u ].color	= color;
		
			x += character.width;
		}
	}
}



