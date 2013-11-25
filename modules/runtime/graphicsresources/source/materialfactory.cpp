
#include "tiki/graphicsresources/materialfactory.hpp"

#include "tiki/graphicsresources/material.hpp"

namespace tiki
{
	MaterialFactory::MaterialFactory()
	{

	}

	MaterialFactory::~MaterialFactory()
	{

	}

	void MaterialFactory::create()
	{

	}

	void MaterialFactory::dispose()
	{

	}

	Resource* MaterialFactory::createResource() const
	{
		return TIKI_NEW Material();
	}

	void MaterialFactory::destroyResource( Resource* pResource ) const
	{
		TIKI_ASSERT( pResource != nullptr );

		Material* pMaterial = (Material*)pResource;
		pMaterial->dispose();
		TIKI_DEL pMaterial;
	}

	bool MaterialFactory::initializeResource( Resource* pResource, const void* pInitData, uint initDataSize ) const
	{
		TIKI_ASSERT( pResource != nullptr );
		return static_cast< Material* >( pResource )->initialize( pInitData );
	}

}