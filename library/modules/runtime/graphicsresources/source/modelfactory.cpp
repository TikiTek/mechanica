
#include "tiki/graphics/modelfactory.hpp"

#include "tiki/graphics/model.hpp"

namespace tiki
{
	ModelFactory::ModelFactory()
	{
		m_pGraphicsSystem = nullptr;
	}

	ModelFactory::~ModelFactory()
	{
		TIKI_ASSERT( m_pGraphicsSystem == nullptr );
	}

	void ModelFactory::create( GraphicsSystem* pGraphicsSystem )
	{
		TIKI_ASSERT( pGraphicsSystem );
		m_pGraphicsSystem = pGraphicsSystem;
	}

	void ModelFactory::dispose()
	{
		m_pGraphicsSystem = nullptr;
	}

	Resource* ModelFactory::createResource() const
	{
		return TIKI_NEW Model();
	}

	void ModelFactory::destroyResource( Resource* pResource ) const
	{
		TIKI_ASSERT( pResource );

		Model* pModel = (Model*)pResource;
		pModel->dispose();

		TIKI_DEL pModel;
	}

	bool ModelFactory::initializeResource( Resource* pResource, const void* pInitData ) const
	{
		TIKI_ASSERT( pResource );

		Model* pModel = (Model*)pResource;
		
		return pModel->initialize( m_pGraphicsSystem, pInitData );
	}
}