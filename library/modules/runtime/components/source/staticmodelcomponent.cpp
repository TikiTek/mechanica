
#include "tiki/components/staticmodelcomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components/staticmodelcomponent_initdata.hpp"
#include "tiki/components/transformcomponent.hpp"
#include "tiki/renderer/gamerenderer.hpp"

namespace tiki
{
	struct TransformComponentState;

	struct StaticModelComponentState : public ComponentState
	{
		const TransformComponentState*	pTransform;
		const Model*					pModel;
	};

	StaticModelComponent::StaticModelComponent()
	{
	}

	StaticModelComponent::~StaticModelComponent()
	{
	}

	bool StaticModelComponent::create( TransformComponent& transformComponent )
	{
		m_transformTypeId = transformComponent.getTypeId();
		if ( m_transformTypeId == InvalidComponentTypeId )
		{
			return false;
		}

		return true;
	}

	void StaticModelComponent::dispose()
	{

	}

	void StaticModelComponent::render( GameRenderer& gameRenderer ) const
	{
		ConstIterator componentStates = getConstIterator();

		const State* pState = nullptr;
		while ( pState = componentStates.getNext() )
		{
			gameRenderer.queueModel( pState->pModel, &pState->pTransform->worldTransform, nullptr );
		}
	}

	crc32 StaticModelComponent::getTypeCrc() const
	{
		return crcString( "StaticModelComponent" );
	}

	uint32 StaticModelComponent::getStateSize() const
	{
		return sizeof( StaticModelComponentState );
	}

	const char* StaticModelComponent::getTypeName() const
	{
		return "StaticModelComponent";
	}

	bool StaticModelComponent::internalInitializeState( ComponentEntityIterator& componentIterator, StaticModelComponentState* pState, const StaticModelComponentInitData* pInitData )
	{
		pState->pTransform	= static_cast< TransformComponentState* >( componentIterator.getFirstOfType( m_transformTypeId ) );
		pState->pModel		= pInitData->model.getData();

		return true;
	}

	void StaticModelComponent::internalDisposeState( StaticModelComponentState* pState )
	{
		pState->pModel = nullptr;
	}

	//void StaticModelComponent::initializeSystem()
	//{
	//	s_sampler.create();
	//}

	//void StaticModelComponent::disposeSystem()
	//{
	//	s_sampler.dispose();
	//}

	//StaticModelComponent::StaticModelComponent()
	//{
	//	m_pModel		= nullptr;
	//	m_pTransform	= nullptr;
	//}

	//void StaticModelComponent::initialize( const Transform* pTransform, const Model* pModel )
	//{
	//	TIKI_ASSERT( pTransform );
	//	TIKI_ASSERT( pModel );

	//	m_pTransform	= pTransform;
	//	m_pModel		= pModel;
	//	m_color			= Color::white;
	//	m_visibile		= true;
	//}

	//void StaticModelComponent::dispose()
	//{
	//	m_pTransform	= nullptr;
	//	m_pModel		= nullptr;

	//	StaticModelComponent::dispose( this );
	//}

	//void StaticModelComponent::render( GpuContext* pContext )
	//{
	//	const size_t componentCount			= s_pool.getCount();

	//	for (size_t i = 0u; i < componentCount; ++i)
	//	{
	//		const StaticModelComponent* pCurrentComp = s_pool[ i ];

	//		if( !pCurrentComp->m_visibile )
	//			continue;

	//		pContext->setMaterial( pCurrentComp->m_pModel->getMaterial() );

	//		const size_t geoCount = pCurrentComp->m_pModel->getGeometryCount();
	//		for (size_t i = 0u; i < geoCount; ++i)
	//		{
	//			const ModelGeometry& geometry = pCurrentComp->m_pModel->getGeometryByIndex( i );

	//			pContext->setInputLayout( geometry.getVertexFormat() );

	//			WorldBuffer* world = MeshRenderer::s_worldBuffer.map( 1 );
	//			world->m_world = pCurrentComp->m_pTransform->getWorld();
	//			world->m_color = pCurrentComp->m_color;
	//			MeshRenderer::s_worldBuffer.unmap( );

	//			pContext->setConstantBuffer( MeshRenderer::s_worldBuffer, 1 );
	//			pContext->setVertexBuffer( geometry.getVertexBuffer() );
	//			pContext->setIndexBuffer( geometry.getIndexBuffer() );

	//			pContext->setSampler( s_sampler );

	//			pContext->setPrimitiveTopology( PrimitiveTopology_TriangleList );
	//			pContext->drawIndexed( geometry.getIndexCount() );
	//		}
	//	}
	//}

	//StaticModelComponent::~StaticModelComponent()
	//{

	//}

}