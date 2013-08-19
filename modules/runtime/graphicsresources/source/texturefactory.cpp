
#include "tiki/graphics/texturefactory.hpp"

#include "tiki/graphics/texture.hpp"

namespace tiki
{
	TextureFactory::TextureFactory()
	{
		m_pGraphicsSystem	= nullptr;
	}

	TextureFactory::~TextureFactory()
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
	}

	void TextureFactory::create( GraphicsSystem& graphicsSystem )
	{
		m_pGraphicsSystem = &graphicsSystem;
	}

	void TextureFactory::dispose()
	{
		TIKI_ASSERT( m_pGraphicsSystem != nullptr );
		m_pGraphicsSystem = nullptr;
	}

	fourcc TextureFactory::getType() const
	{
		return Texture::s_resourceType;
	}

	Resource* TextureFactory::createResource() const
	{
		return TIKI_NEW Texture();
	}

	void TextureFactory::destroyResource( Resource* pResource ) const
	{
		TIKI_ASSERT( pResource );

		Texture* pTexture = (Texture*)pResource;
		pTexture->dispose();

		TIKI_DEL pTexture;
	}

	bool TextureFactory::initializeResource( Resource* pResource, const void* pInitData ) const
	{
		TIKI_ASSERT( pResource != nullptr );
		TIKI_ASSERT( pInitData != nullptr );
		TIKI_ASSERT( m_pGraphicsSystem != nullptr );
		
		Texture* pTexture = (Texture*)pResource;

		const void* pData = addPtr( pInitData, sizeof( uint32 ) );
		const TextureDescription* pDescription = (const TextureDescription*)pData;

		pData = addPtr( pData, sizeof( TextureDescription ) );
		return pTexture->initialize( *m_pGraphicsSystem, *pDescription, pData );
	}
}