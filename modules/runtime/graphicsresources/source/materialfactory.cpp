
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
		TIKI_ASSERT( pResource );

		Material* pMat = (Material*)pResource;
		pMat->dispose();

		TIKI_DEL pMat;
	}

	bool MaterialFactory::initializeResource( Resource* pResource, const void* pInitData ) const
	{
		TIKI_ASSERT( pResource );

		Material* pMat = (Material*)pResource;
		return pMat->initialize( pInitData );
	}

}