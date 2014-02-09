
#include "tiki/graphics/material.hpp"

#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	struct MaterialFactoryContext : public FactoryContextGenericBase< Material >
	{
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
		m_pData = *(const RenderEffectData**)initData.pData;
		return m_pData != nullptr;
	}

	void Material::disposeInternal( const FactoryContext& factoryContext )
	{
	}
}


