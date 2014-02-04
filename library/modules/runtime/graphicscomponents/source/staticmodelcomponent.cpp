
#include "tiki/components/staticmodelcomponent.hpp"

#include "tiki/components/componentstate.hpp"

//#include "tiki/gamecore/transform.hpp"
//#include "tiki/graphics/gpucontext.hpp"
//#include "tiki/graphics/model.hpp"
//#include "tiki/graphicsbase/primitivetopologies.hpp"
//
//#include "tiki/gamecore/meshrenderer.hpp"
//
//#include "tiki/game/gamedata.hpp"

namespace tiki
{
	struct TransformComponentState;

	struct StaticModelComponentState : public ComponentState< StaticModelComponentState >
	{
		const TransformComponentState*	pTransform;
		const Model*					pModel;
	};

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