
#include "tiki/graphicsresources/texture.hpp"

#include "tiki/resource/resourcefile.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	struct TextureInitializationData
	{
		TextureDescription	description;
		ResRef< void >		data;
	};

	struct TextureFactoryContext : public FactoryContextGenericBase< Texture >
	{
		TextureFactoryContext( GraphicsSystem& _graphicsSystem )
			: graphicsSystem( _graphicsSystem )
		{
		}

		GraphicsSystem& graphicsSystem;
	};

	void Texture::registerResourceType( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem )
	{
		static TextureFactoryContext context( graphicsSystem );
		resourceManager.registerResourceType( s_resourceType, context );
	}

	void Texture::unregisterResourceType( ResourceManager& resourceManager )
	{
		resourceManager.unregisterResourceType( s_resourceType );
	}

	Texture::Texture()
	{
	}

	Texture::~Texture()
	{
	}

	bool Texture::createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext )
	{
		const TextureFactoryContext* pFactory		= static_cast< const TextureFactoryContext* >( &factoryContext );
		const TextureInitializationData* pInitData	= static_cast< const TextureInitializationData* >( initData.pData );

		return m_textureData.create( pFactory->graphicsSystem, pInitData->description, pInitData->data.getData() );
	}

	void Texture::disposeInternal( const FactoryContext& factoryContext )
	{
		const TextureFactoryContext* pFactory = static_cast< const TextureFactoryContext* >( &factoryContext );
		m_textureData.dispose( pFactory->graphicsSystem );
	}

}
