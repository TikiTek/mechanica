
#include "tiki/graphics/immediaterenderer.hpp"

#include "tiki/framework/framework.hpp"
#include "tiki/graphics/graphicssystem.hpp"
#include "tiki/graphics/texturedata.hpp"
#include "tiki/graphics/vertexformat.hpp"
#include "tiki/graphicsresources/font.hpp"
#include "tiki/graphicsresources/material.hpp"
#include "tiki/graphicsresources/shader.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	ImmediateRenderer::~ImmediateRenderer()
	{
	}

	bool ImmediateRenderer::create( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager, const WindowEventBuffer& eventBuffer )
	{
		m_pEventBuffer	= &eventBuffer;

		m_pMaterial = resourceManager.loadResource< Material >( "immediate.material" );
		TIKI_ASSERT( m_pMaterial );

		{
			const VertexAttribute attributes[] =
			{
				{ VertexSementic_Position,	0u,	VertexAttributeFormat_x32y32z32_float,		0u, VertexInputType_PerVertex },
				{ VertexSementic_TexCoord,	0u,	VertexAttributeFormat_x32y32_float,			0u, VertexInputType_PerVertex },
				{ VertexSementic_Color,		0u,	VertexAttributeFormat_x32y32z32w32_float,	0u, VertexInputType_PerVertex }
			};		

			VertexFormatParameters vertexParams;
			vertexParams.pAttributes	= attributes;
			vertexParams.attributeCount	= TIKI_COUNT( attributes );
			vertexParams.pShader		= m_pMaterial->getVertexShader();

			m_pVertexFormat = VertexFormat::getVertexFormat( vertexParams );
			TIKI_ASSERT( m_pVertexFormat );
		}

		SamplerStateParamters samplerParams;
		m_pSamplerState = graphicsSystem.createSamplerState( samplerParams );

		m_sprites.create( MaxSprites );
		m_vertices.create( MaxVertices );

		m_vertexBuffer.create( graphicsSystem, MaxVertices, sizeof( SpriteVertex ), true );
		
		return true;
	}

	void ImmediateRenderer::dispose( GraphicsSystem& graphicsSystem, ResourceManager& resourceManager )
	{
		resourceManager.unloadResource( m_pMaterial );
		m_pMaterial = nullptr;

		VertexFormat::releaseVertexFormat( m_pVertexFormat );
		m_pVertexFormat = nullptr;

		graphicsSystem.disposeSamplerState( m_pSamplerState );
		m_pSamplerState = nullptr;

		m_sprites.dispose();
		m_vertices.dispose();

		m_vertexBuffer.dispose();
	}

	void tiki::ImmediateRenderer::drawTexture( const TextureData& texture, const Rectangle& r )
	{
		Sprite& sprite = m_sprites.push();
		sprite.offset	= m_vertices.getCount();
		sprite.count	= 4u;
		sprite.pTexture = &texture;

		SpriteVertex* pVertices = m_vertices.pushRange( 4u );

		// bottom left
		pVertices[ 0u ].position.x = -1.0f + r.x;
		pVertices[ 0u ].position.y =  1.0f  - r.y - r.height;
		pVertices[ 0u ].position.z =  0.0f;
		pVertices[ 0u ].uv.x = 0.0f;
		pVertices[ 0u ].uv.y = 1.0f;

		// top left
		pVertices[ 1u ].position.x = -1.0f + r.x;
		pVertices[ 1u ].position.y =  1.0f - r.y;
		pVertices[ 1u ].position.z =  0.0f;
		pVertices[ 1u ].uv.x = 0.0f;
		pVertices[ 1u ].uv.y = 0.0f;

		// bottom right
		pVertices[ 2u ].position.x = -1.0f + r.x + r.width;
		pVertices[ 2u ].position.y =  1.0f - r.y - r.height;
		pVertices[ 2u ].position.z =  0.0f;
		pVertices[ 2u ].uv.x = 1.0f;
		pVertices[ 2u ].uv.y = 1.0f;

		// top right
		pVertices[ 3u ].position.x = -1.0f + r.x + r.width;
		pVertices[ 3u ].position.y =  1.0f - r.y;
		pVertices[ 3u ].position.z =  0.0f;
		pVertices[ 3u ].uv.x = 1.0f;
		pVertices[ 3u ].uv.y = 0.0f;
	}

	void ImmediateRenderer::drawTexture( const TextureData& texture, const Rectangle& d, const Rectangle& s )
	{
		Sprite& sprite = m_sprites.push();
		sprite.offset	= m_vertices.getCount();
		sprite.count	= 4u;
		sprite.pTexture = &texture;

		SpriteVertex* pVertices = m_vertices.pushRange( 4u );

		// bottom left
		pVertices[ 0u ].position.x = -1.0f	+ d.x;
		pVertices[ 0u ].position.y =  1.0f  - d.y - d.height;
		pVertices[ 0u ].position.z =  0.0f;
		pVertices[ 0u ].uv.x = s.x;
		pVertices[ 0u ].uv.y = s.y + s.height;

		// top left
		pVertices[ 1u ].position.x = -1.0f + d.x;
		pVertices[ 1u ].position.y =  1.0f - d.y;
		pVertices[ 1u ].position.z =  0.0f;
		pVertices[ 1u ].uv.x = s.x;
		pVertices[ 1u ].uv.y = s.y;

		// bottom right
		pVertices[ 2u ].position.x = -1.0f + d.x + d.width;
		pVertices[ 2u ].position.y =  1.0f - d.y - d.height;
		pVertices[ 2u ].position.z =  0.0f;
		pVertices[ 2u ].uv.x = s.x + s.width;
		pVertices[ 2u ].uv.y = s.y + s.height;

		// top right
		pVertices[ 3u ].position.x = -1.0f + d.x + d.width;
		pVertices[ 3u ].position.y =  1.0f - d.y;
		pVertices[ 3u ].position.z =  0.0f;
		pVertices[ 3u ].uv.x = s.x + s.width;
		pVertices[ 3u ].uv.y = s.y;
	}

	void ImmediateRenderer::drawText( const Vector2& position, const Font& font, const string& text, Color color )
	{
		if ( text.isEmpty() )
		{
			return;
		}

		TIKI_ASSERT( text.length() <= 128u );
		const size_t vertexCount = text.length() * 4u;

		Sprite& sprite = m_sprites.push();
		sprite.offset	= m_vertices.getCount();
		sprite.count	= vertexCount;
		sprite.pTexture = &font.getTextureData();

		SpriteVertex* pVertices = m_vertices.pushRange( vertexCount );

		FontChar chars[ 128u ];
		font.fillVertices( chars, TIKI_COUNT( chars ), text.cStr(), text.length() );

		const Vector2 screenSize; // = m_pGpuContext->getBackBufferSize();

		float x = 0.0f;
		for (size_t i = 0u; i < vertexCount; i += 4u)
		{
			const size_t vertexIndex = i * 4u;
			const FontChar& character = chars[ i ];

			const Rectangle rect = Rectangle(
				( position.x + x ) / screenSize.x,
				position.y / screenSize.y,
				character.width / screenSize.x,
				character.height / screenSize.y
			);

		//	// bottom left
		//	createFloat3( pVertices[ i + 0u ].position, rect.x, rect.y + rect.height, 0.0f );
		//	createByte4( pVertices[ i + 0u ].identifier, 255u, 0u, 0u, 255u );

		//	// top left
		//	createFloat2( pVertices[ i + 1u ].position, rect.x, rect.y );
		//	createByte4( pVertices[ i + 1u ].identifier, 255u, 255u, 0u, 0u );

		//	// bottom right
		//	createFloat2( pVertices[ i + 2u ].position, rect.x + rect.width, rect.y + rect.height );
		//	createByte4( pVertices[ i + 2u ].identifier, 0u, 0u, 255u, 255u );

		//	// top right
		//	createFloat2( pVertices[ i + 3u ].position, rect.x + rect.width, rect.y );
		//	createByte4( pVertices[ i + 3u ].identifier, 0u, 255u, 255u, 0u );
		//
		//	x += pVertices[ i ].character.width;
		}

		//m_textTextures.push( &font.getTextureData() );
		//m_textLength.push( text.length() );
	}

	void ImmediateRenderer::flush( GraphicsContext& graphicsContext )
	{
		graphicsContext.setPixelShaderSamplerState( 0u, m_pSamplerState );

		//m_pGpuContext->disableDepth();
		//m_pGpuContext->enableAlpha();

		// render sprites
		if( m_sprites.getCount() )
		{
			const uint vertexCount	= m_vertices.getCount();
			const uint count		= m_sprites.getCount();

			SpriteVertex* sv = (SpriteVertex*)graphicsContext.mapBuffer( m_vertexBuffer );
			memory::copy( sv, m_sprites.getData(), sizeof( SpriteVertex ) * vertexCount );
			graphicsContext.unmapBuffer( m_vertexBuffer );

			graphicsContext.setInputLayout( m_pVertexFormat );
			//graphicsContext.setMaterial( m_pMaterial );

			graphicsContext.setVertexBuffer( 0u, m_vertexBuffer );
			graphicsContext.setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

			for( uint i = 0u; i < count; ++i )
			{
				const Sprite& sprite = m_sprites[ i ];

				graphicsContext.setPixelShaderTexture( 0u, sprite.pTexture );
				//graphicsContext.draw( 4u, i * 4u );
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
}



