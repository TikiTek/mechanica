
#include "tiki/graphics/model.hpp"

#include "tiki/base/memory.hpp"
#include "tiki/resource/resourcefile.hpp"
#include "tiki/resource/resourcemanager.hpp"

namespace tiki
{
	struct ModelGeometryInitData;
	struct ModelHierarchyInitData;

	struct ModelFactoryContext : public FactoryContextGenericBase< Model >
	{
		ModelFactoryContext( GraphicsSystem& _graphicsSystem )
			: graphicsSystem( _graphicsSystem )
		{
		}

		GraphicsSystem& graphicsSystem;
	};

	void Model::registerResourceType( ResourceManager& resourceManager, GraphicsSystem& graphicsSystem )
	{
		static ModelFactoryContext context( graphicsSystem );
		resourceManager.registerResourceType( s_resourceType, context );
	}

	void Model::unregisterResourceType( ResourceManager& resourceManager )
	{
		resourceManager.unregisterResourceType( s_resourceType );
	}

	struct ModelInitData
	{
		ResRef< Material >					material;
		ResRef< ModelHierarchyInitData >	hierarchy;

		uint32								geometrieCount;
		ResRef< ModelGeometryInitData >		geometries[ 1u ]; // count is geometrieCount
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
		const ModelFactoryContext& factory = *static_cast< const ModelFactoryContext* >( &factoryContext );
		const ModelInitData& modelInitData = *static_cast< const ModelInitData* >( initData.pData );

		m_pMaterial = modelInitData.material.getData();
		//TIKI_ASSERT( m_pMaterial != nullptr );

		if ( modelInitData.hierarchy.getData() != nullptr )
		{
			m_pHierarchy = TIKI_NEW( ModelHierarchy );
			TIKI_VERIFY( m_pHierarchy->initialize( *modelInitData.hierarchy.getData() ) );
		}

		m_geometries.create( modelInitData.geometrieCount );
		for (size_t i = 0u; i < m_geometries.getCount(); ++i)
		{
			const ModelGeometryInitData* pGeometryInitData = modelInitData.geometries[ i ].getData();
			m_geometries[ i ].initialize( factory.graphicsSystem, *pGeometryInitData, m_pMaterial );
		}

		return true;
	}

	void Model::disposeInternal( const FactoryContext& factoryContext )
	{
		const ModelFactoryContext& factory = *static_cast< const ModelFactoryContext* >( &factoryContext );

		if ( m_pHierarchy )
		{
			m_pHierarchy->dispose();
			TIKI_DELETE( m_pHierarchy );
			m_pHierarchy = nullptr;
		}

		for (size_t i = 0u; i < m_geometries.getCount(); ++i)
		{
			m_geometries[ i ].dispose( factory.graphicsSystem );
		}
		m_geometries.dispose();

		m_pMaterial = nullptr;
	}
}