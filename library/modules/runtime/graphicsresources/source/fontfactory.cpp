
#include "tiki/graphics/fontfactory.hpp"

#include "tiki/graphics/font.hpp"

namespace tiki
{
	FontFactory::FontFactory()
	{
		m_pGraphicsSystem = nullptr;
	}

	FontFactory::~FontFactory()
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
	}

	void FontFactory::create( GraphicsSystem& graphicsSystem )
	{
		m_pGraphicsSystem = &graphicsSystem;
	}

	void FontFactory::dispose()
	{
		m_pGraphicsSystem = nullptr;
	}

	fourcc FontFactory::getType() const
	{
		return Font::s_resourceType;
	}

	Resource* FontFactory::createResource() const
	{
		return TIKI_NEW Font();
	}

	void FontFactory::destroyResource( Resource* pResource ) const
	{
		TIKI_ASSERT( pResource );

		Font* pFont = (Font*)pResource;
		pFont->dispose();
		
		TIKI_DEL pFont;
	}

	bool FontFactory::initializeResource( Resource* pResource, const void* pInitData ) const
	{
		TIKI_ASSERT( pResource != nullptr );
		TIKI_ASSERT( pInitData != nullptr );
		TIKI_ASSERT( m_pGraphicsSystem != nullptr );

		Font* pFont = static_cast< Font* >( pResource );
		return pFont->initialize( *m_pGraphicsSystem, pInitData );
	}
}