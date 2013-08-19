#include "tiki/graphics/spriterenderer.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/resource/resourcemanager.hpp"
#include "tiki/graphics/graphicsystem.hpp"
#include "tiki/graphics/shader.hpp"
#include "tiki/graphics/material.hpp"
#include "tiki/graphics/texture.hpp"
#include "tiki/graphics/vertexformat.hpp"


namespace tiki
{
	tiki::SpritRenderer::SpritRenderer() 
		: m_pGpuContext( nullptr ), m_pMaterial( nullptr ), m_pVertexFormat( nullptr )
	{

	}

	tiki::SpritRenderer::~SpritRenderer()
	{
	}

	bool tiki::SpritRenderer::create( GpuContext* pContext )
	{
		TIKI_ASSERT( pContext );
		m_pGpuContext = pContext;

		ResourceManager& content = framework::getResourceManager();

		m_pMaterial = content.loadResource< Material >( "sprite.material" );
		TIKI_ASSERT( m_pMaterial );

		m_pTextMaterial = content.loadResource< Material >( "font.material" );
		TIKI_ASSERT( m_pTextMaterial );

		// create sprite vertex format
		{
			const VertexAttribute attributes[] =
			{
				{ VertexSementic_Position,	0u,	VertexAttributeFormat_x32y32z32_float,	0u, VertexInputType_PerVertex },
				{ VertexSementic_TexCoord,	0u,	VertexAttributeFormat_x32y32_float,		0u, VertexInputType_PerVertex }
			};		

			VertexFormatParameters vertexParams;
			vertexParams.pAttributes	= attributes;
			vertexParams.attributeCount	= TIKI_COUNT( attributes );
			vertexParams.pShader		= m_pMaterial->getVertexShader();

			m_pVertexFormat = VertexFormat::getVertexFormat( vertexParams );
			TIKI_ASSERT( m_pVertexFormat );
		}

		// create font vertex format
		{
			const VertexAttribute attributes[] =
			{
				{ VertexSementic_Position,	0u,	VertexAttributeFormat_x32y32_float,			0u, VertexInputType_PerVertex },
				{ VertexSementic_TexCoord,	0u,	VertexAttributeFormat_x8y8z8w8_unorm,		0u, VertexInputType_PerVertex },
				{ VertexSementic_TexCoord,	1u,	VertexAttributeFormat_x32y32_float,			0u, VertexInputType_PerVertex },
				{ VertexSementic_TexCoord,	2u,	VertexAttributeFormat_x16y16z16w16_unorm,	0u, VertexInputType_PerVertex }
			};		

			VertexFormatParameters vertexParams;
			vertexParams.pAttributes	= attributes;
			vertexParams.attributeCount	= TIKI_COUNT( attributes );
			vertexParams.pShader		= m_pTextMaterial->getVertexShader();

			m_pTextVertexFormat = VertexFormat::getVertexFormat( vertexParams );
			TIKI_ASSERT( m_pTextVertexFormat );
		}

		m_sampler.create();

		const Vector2 screen = m_pGpuContext->getBackBufferSize();
		m_viewPort.div( Vector2( 2.0f ), screen );

		m_textures.create( MaxSprites );
		m_sprites.create( MaxSprites * 4u );
		
		m_textTextures.create( MaxTextTextures );
		m_textChars.create( MaxTextChars );
		m_textLength.create( MaxTextTextures );

		return true;
	}

	void tiki::SpritRenderer::dispose()
	{
		framework::getResourceManager().unloadResource( m_pMaterial );
		m_pMaterial = nullptr;

		VertexFormat::releaseVertexFormat( m_pVertexFormat );
		m_pVertexFormat = nullptr;

		framework::getResourceManager().unloadResource( m_pTextMaterial );
		m_pTextMaterial = nullptr;

		VertexFormat::releaseVertexFormat( m_pTextVertexFormat );
		m_pTextVertexFormat = nullptr;

		m_sampler.dispose();
		m_vertexBuffer.dispose();

		m_textures.dispose();
		m_sprites.dispose();

		m_textVertexBuffer.dispose();
		m_textChars.dispose();
		m_textTextures.dispose();
		m_textLength.dispose();
	}

	void tiki::SpritRenderer::drawTexture( const Rectangle& rect, const TextureData& textureData, bool percentage /*= false*/ )
	{
		Rectangle r = rect;

		if( percentage )
			toScreenSpacePercentage( r );
		else
			toScreenSpace( r );


		m_textures.push( &textureData );

		SpriteVertex* pVertices = m_sprites.pushRange( 4u );

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

	void SpritRenderer::drawTexture( const Rectangle& dest, const Rectangle& src, const TextureData& tex )
	{
		Rectangle d = dest;
		Rectangle s = src;

		toScreenSpace( d );
		toUVSpace( s, tex );


		m_textures.push( &tex );

		SpriteVertex* pVertices = m_sprites.pushRange( 4u );

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

	void SpritRenderer::drawText( const Vector2& position, const Font& font, const string& text )
	{
		if ( text.isEmpty() )
		{
			return;
		}

		const size_t vertexCount = text.length() * 4u;
		FontVertex* pVertices = m_textChars.pushRange( vertexCount );

		font.fillVertices( &pVertices->character, sizeof( FontVertex ), 4u, text.cStr(), text.length() );

		const Vector2& screenSize = m_pGpuContext->getBackBufferSize();

		float x = 0.0f;
		for (size_t i = 0u; i < vertexCount; i += 4u)
		{
			const FontChar& character = pVertices[ i ].character;

			const Rectangle rect = Rectangle(
				( position.x + x ) / screenSize.x,
				position.y / screenSize.y,
				character.width / screenSize.x,
				character.height / screenSize.y
			);

			// bottom left
			createFloat2( pVertices[ i + 0u ].position, rect.x, rect.y + rect.height );
			createByte4( pVertices[ i + 0u ].identifier, 255u, 0u, 0u, 255u );

			// top left
			createFloat2( pVertices[ i + 1u ].position, rect.x, rect.y );
			createByte4( pVertices[ i + 1u ].identifier, 255u, 255u, 0u, 0u );

			// bottom right
			createFloat2( pVertices[ i + 2u ].position, rect.x + rect.width, rect.y + rect.height );
			createByte4( pVertices[ i + 2u ].identifier, 0u, 0u, 255u, 255u );

			// top right
			createFloat2( pVertices[ i + 3u ].position, rect.x + rect.width, rect.y );
			createByte4( pVertices[ i + 3u ].identifier, 0u, 255u, 255u, 0u );
		
			x += pVertices[ i ].character.width;
		}

		m_textTextures.push( &font.getTextureData() );
		m_textLength.push( text.length() );
	}

	void SpritRenderer::toUVSpace( Rectangle& rec, const TextureData& texture )
	{
		const Vector2 scal( 1.0f / texture.getWidth(), 1.0f / texture.getHeight() );
		rec.mul( scal );
	}

	void tiki::SpritRenderer::toScreenSpace( Rectangle& rect )
	{
		rect = Rectangle( rect.x * m_viewPort.x ,
						  rect.y * m_viewPort.y,
						  rect.width * m_viewPort.x,
						  rect.height * m_viewPort.y );
	}

	void tiki::SpritRenderer::toScreenSpacePercentage( Rectangle& rect )
	{
		Vector2 vp = m_pGpuContext->getBackBufferSize();
		rect = Rectangle( rect.x * 2.0f, 
							rect.y * 2.0f, 
							rect.width * 2.0f, 
							rect.height * 2.0f);
	}

	void tiki::SpritRenderer::flush()
	{
		m_pGpuContext->setSampler( m_sampler );

		m_pGpuContext->disableDepth();
		m_pGpuContext->enableAlpha();

		// render sprites
		if( m_sprites.getCount() )
		{
			const uint vertexCount	= m_sprites.getCount();
			const uint count		= vertexCount / 4u;

			SpriteVertex* sv = m_vertexBuffer.map( vertexCount );
			memory::copy( sv, m_sprites.getData(), sizeof( SpriteVertex ) * vertexCount );
			m_vertexBuffer.unmap();

			m_pGpuContext->setInputLayout( m_pVertexFormat );
			m_pGpuContext->setMaterial( m_pMaterial );

			m_pGpuContext->setVertexBuffer( m_vertexBuffer );
			m_pGpuContext->setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

			for( uint i = 0u; i < count; ++i )
			{
				m_pGpuContext->setPixelShaderTexture( m_textures[ i ] );
				m_pGpuContext->draw( 4u, i * 4u );
			}
		}

		// render text
		if ( m_textChars.getCount() )
		{
			FontVertex* pTextVertices = m_textVertexBuffer.map( m_textChars.getCount() );
			memory::copy( pTextVertices, m_textChars.getData(), m_textChars.getCount() * sizeof( FontVertex ) );
			m_textVertexBuffer.unmap();

			m_pGpuContext->setInputLayout( m_pTextVertexFormat );
			m_pGpuContext->setMaterial( m_pTextMaterial );

			m_pGpuContext->setVertexBuffer( m_textVertexBuffer );
			m_pGpuContext->setPrimitiveTopology( PrimitiveTopology_TriangleStrip );

			size_t offset = 0u;
			while ( m_textTextures.getCount() )
			{
				const TextureData* pTextureData = m_textTextures.pop();
				const size_t vertexCount = m_textLength.pop() * 4u;
				
				m_pGpuContext->setPixelShaderTexture( pTextureData );
				m_pGpuContext->draw( vertexCount, offset );
				offset += vertexCount;
			}

			m_textChars.clear();
			TIKI_ASSERT( m_textLength.getCount() == 0u );
			TIKI_ASSERT( m_textTextures.getCount() == 0u );
		}

		m_pGpuContext->enableDepth();
		m_pGpuContext->disableAlpha();

		m_sprites.clear();
		m_textures.clear();
	}
}



