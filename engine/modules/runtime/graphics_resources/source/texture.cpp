#include "tiki/graphics/texture.hpp"

#include "tiki/resource/resource_file.hpp"
#include "tiki/resource/resource_manager.hpp"

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
			: FactoryContextGenericBase( FlagMask8< ResourceDefinitionFeature >( ResourceDefinitionFeature_GraphicsApi ) )
			, graphicsSystem( _graphicsSystem )
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
		const TextureFactoryContext& factory		= *static_cast< const TextureFactoryContext* >( &factoryContext );
		const TextureInitializationData* pInitData	= static_cast< const TextureInitializationData* >( initData.pData );

		return m_textureData.create( factory.graphicsSystem, pInitData->description, pInitData->data.getData(), getFileName() );
	}

	void Texture::disposeInternal( const FactoryContext& factoryContext )
	{
		const TextureFactoryContext& factory = *static_cast< const TextureFactoryContext* >( &factoryContext );

		m_textureData.dispose( factory.graphicsSystem );
	}

}
