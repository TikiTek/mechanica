
#include "tiki/graphics/font.hpp"

#include "tiki/math/vector.hpp"
#include "tiki/resource/resourcefile.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	struct FontInitializationData
	{
		TextureDescription	textureDesc;
		ResRef< void >		textureData;

		uint32				charCount;
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
				
		if ( m_textureData.create( pFactory->graphicsSystem, pInitData->textureDesc, pInitData->textureData.getData(), getFileName() ) )
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
	
	const FontChar& Font::getChar( char c ) const
	{
		return m_chars[ c ];
	}

	void Font::calcuateTextSize( Vector2& textSize, const char* pText, uint textLength ) const
	{
		textSize = Vector2::zero;

		for (uint i = 0u; i < textLength; ++i)
		{
			const char charIndex		= pText[ i ];
			const FontChar& charStruct	= m_chars[ charIndex ];
			TIKI_ASSERT( charIndex != '\0' );
			
			textSize.x += charStruct.width;
			textSize.y = TIKI_MAX( textSize.y, charStruct.height );
		}
	}

	void Font::fillVertices( FontChar* pChars, uint capacity, const char* pText, uint textLength ) const
	{
		TIKI_ASSERT( capacity >= textLength );
		for (uint i = 0u; i < textLength; ++i)
		{
			TIKI_ASSERT( pText[ i ] != '\0' );
			pChars[ i ]	= m_chars[ pText[ i ] ];
		}
	}
}
