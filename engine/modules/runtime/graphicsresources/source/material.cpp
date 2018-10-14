#include "tiki/graphics/material.hpp"

#include "tiki/resource/resource_file.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	struct MaterialFactoryContext : public FactoryContextGenericBase< Material >
	{
	};

	struct MaterialInitData
	{
		ResRef< RenderEffectData >	renderEffectData;
	};

	void Material::registerResourceType( ResourceManager& resourceManager )
	{
		static MaterialFactoryContext context;
		resourceManager.registerResourceType( s_resourceType, context );
	}

	void Material::unregisterResourceType( ResourceManager& resourceManager )
	{
		resourceManager.unregisterResourceType( s_resourceType );
	}

	Material::Material()
	{
	}

	Material::~Material()
	{
	}

	bool Material::createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext )
	{
		const MaterialInitData* pInitData = (MaterialInitData*)initData.pData;

		m_pData = pInitData->renderEffectData.getData();
		return m_pData != nullptr;
	}

	void Material::disposeInternal( const FactoryContext& factoryContext )
	{
	}
}


