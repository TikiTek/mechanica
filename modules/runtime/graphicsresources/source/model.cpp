
#include "tiki/graphicsresources/model.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/framework/framework.hpp"
#include "tiki/resource/resourcemanager.hpp"
#include "tiki/graphicsresources/material.hpp"

namespace tiki
{
	struct ModelFactoryContext : public FactoryContext
	{
		GraphicsSystem* pGraphicsSystem;
	};
	TIKI_DEFAULT_RESOURCE_CODE( Model )

	struct ModelBinaryData
	{
		uint32		geometrieCount;
		uint32		hierarchySize;
	};

	Model::Model()
	{
		m_pHierarchy = nullptr;
		m_pMaterial = nullptr;
	}

	Model::~Model()
	{
	}

	bool Model::createInternal( const ResourceInitData& initData, const FactoryContext& factoryContext )
	{
		const void* pData = initData.pData;
		const ModelFactoryContext& factory = *static_cast< const ModelFactoryContext* >( &factoryContext );

		//const uint materialLength = *(uint*)pData;
		//pData = addPtr( pData, sizeof(uint) );

		//const char* materialName = (char*)pData;

		//string material( materialName, materialLength );
		//pData = addPtr( pData, materialLength );

		//m_pMaterial = framework::getResourceManager().loadResource< Material >( material );
		//TIKI_ASSERT( m_pMaterial );

		//const ModelBinaryData* pInfo = (const ModelBinaryData*)pData;
		//pData = addPtr( pData, sizeof( ModelBinaryData ) );

		//if ( pInfo->hierarchySize > 0u )
		//{
		//	m_pHierarchy	= (ModelHierarchy*)memory::newAlign< ModelHierarchy >();
		//	pData			= m_pHierarchy->initialize( pData );
		//}

		//m_geometries.create( pInfo->geometrieCount );
		//for (size_t i = 0u; i < m_geometries.getCount(); ++i)
		//{
		//	pData = m_geometries[ i ].initialize( *factory.pGraphicsSystem, pData, m_pMaterial );
		//}

		return true;
	}

	void Model::disposeInternal( const FactoryContext& factoryContext )
	{
		//if ( m_pHierarchy )
		//{
		//	m_pHierarchy->dispose();
		//	memory::deleteAlign( m_pHierarchy );

		//	m_pHierarchy = nullptr;
		//}

		//for (size_t i = 0u; i < m_geometries.getCount(); ++i)
		//{
		//	m_geometries[ i ].dispose();
		//}
		//m_geometries.dispose();

		//framework::getResourceManager().unloadResource( m_pMaterial );
	}
}