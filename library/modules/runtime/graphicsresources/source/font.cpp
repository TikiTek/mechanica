
#include "tiki/graphicsresources/font.hpp"

#include "tiki/resource/resourcefile.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	struct FontInitializationData
	{
		TextureDescription	textureDesc;
		ResRef< void >		textureData;

		uint				charCount;
		ResRef< FontChar >	charArray;
	};

	struct FontFactoryContext : public FactoryContextGenericBase< Font >
	{
		FontFactoryContext( GraphicsSystem& _graphicsSystem )
			: graphicsSystem( _graphicsSystem )
		{
		}

		GraphicsSystem& graphicsSystem;
	};

	void Font::registerResourceType( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem )
	{
		static FontFactoryContext context( graphicsSystem );
		resourceManager.registerResourceType( s_resourceType, context );
	}

	void Font::unregisterResourceType( ResourceManager& resourceManager )
	{
		resourceManager.unregisterResourceType( s_resourceType );
	}

	Font::Font()
	{		
	}

	Font::~Font()
	{
	}

	bool Font::createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext )
	{
		const FontFactoryContext* pFactory		= static_cast< const FontFactoryContext* >( &factoryContext );
		const FontInitializationData* pInitData	= static_cast< const FontInitializationData* >( initData.pData );
				
		if ( m_textureData.create( pFactory->graphicsSystem, pInitData->textureDesc, pInitData->textureData.getData() ) )
		{
			m_chars.create( pInitData->charArray.getData(), pInitData->charCount );
			return true;
		}

		return false;
	}

	void Font::disposeInternal( const FactoryContext& factoryContext )
	{
		const FontFactoryContext* pFactory = static_cast< const FontFactoryContext* >( &factoryContext );

		m_chars.dispose();
		m_textureData.dispose( pFactory->graphicsSystem );
	}

	void Font::fillVertices( FontChar* pChars, size_t capacity, cstring text, size_t textLength ) const
	{
		TIKI_ASSERT( capacity >= textLength );

		for (size_t i = 0u; i < capacity; ++i)
		{
			TIKI_ASSERT( text[ i ] != '\0' );
			pChars[ i ]	= m_chars[ text[ i ] ];
		}
	}
}
