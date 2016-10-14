#include "tiki/components3d/skinnedmodelcomponent.hpp"

#include "tiki/base/crc32.hpp"
#include "tiki/components/componentstate.hpp"
#include "tiki/components3d/transform3dcomponent.hpp"
#include "tiki/graphics/model.hpp"
#include "tiki/math/matrix.hpp"

#include "components.hpp"

namespace tiki
{
	struct SkinnedModelComponentState : public ComponentState
	{
		const Model*	pModel;

		Matrix43		modelPose[ 256u ];
		uint			jointCount;
	};

	SkinnedModelComponent::SkinnedModelComponent()
	{
		m_transformTypeId = InvalidComponentTypeId;
	}

	SkinnedModelComponent::~SkinnedModelComponent()
	{
		TIKI_ASSERT( m_transformTypeId == InvalidComponentTypeId );
	}

	bool SkinnedModelComponent::create( Transform3dComponent& transformComponent )
	{
		m_transformTypeId = transformComponent.getTypeId();
		if ( m_transformTypeId == InvalidComponentTypeId )
		{
			return false;
		}

		return true;
	}

	void SkinnedModelComponent::dispose()
	{
		m_transformTypeId = InvalidComponentTypeId;
	}

	void SkinnedModelComponent::render( RenderScene& scene ) const
	{
	}

	crc32 SkinnedModelComponent::getTypeCrc() const
	{
		return crcString( "SkinnedModelComponent" );
	}

	uint32 SkinnedModelComponent::getStateSize() const
	{
		return sizeof( SkinnedModelComponentState );
	}

	const char* SkinnedModelComponent::getTypeName() const
	{
		return "SkinnedModelComponent";
	}

	bool SkinnedModelComponent::internalInitializeState( ComponentEntityIterator& componentIterator, SkinnedModelComponentState* pComponentState, const SkinnedModelComponentInitData* pComponentInitData )
	{		
		return false;
	}

	void SkinnedModelComponent::internalDisposeState( SkinnedModelComponentState* pComponentState )
	{		
	}

	//void SkinnedModelComponent::initializeSystem()
	//{
	//}

	//void SkinnedModelComponent::disposeSystem()
	//{
	//	s_skinningBuffer.dispose();
	//}

	//void SkinnedModelComponent::initialize( const Transform* pTransform, const Model* pModel, const Material* pMaterial /*= nullptr*/ )
	//{
	//	TIKI_ASSERT( pTransform );
	//	TIKI_ASSERT( pModel );
	//	TIKI_ASSERT( pModel->getHierarchy() );

	//	m_pTransform	= pTransform;
	//	m_pModel		= pModel;

	//	m_ownMaterial	= pMaterial != nullptr ? true : false;
	//	m_color			= Color::white;

	//	if( m_ownMaterial )
	//	{
	//		m_pMaterial = pMaterial;
	//		m_pMaterial->addReference();
	//	}
	//	else
	//	{
	//		m_pMaterial = m_pModel->getMaterial();
	//	}

	//	TIKI_ASSERT( m_pModel->getHierarchy()->getJointCount() < 256u );
	//	m_poseMatrices.create( m_pModel->getHierarchy()->getJointCount() );

	//	m_renderOffset = Vector3::zero;
	//}

	//void SkinnedModelComponent::dispose()
	//{
	//	m_poseMatrices.dispose();

	//	if( m_ownMaterial )
	//		framework::getResourceManager().unloadResource( m_pMaterial );

	//	SkinnedModelComponent::dispose( this );
	//}

	//const Matrix& SkinnedModelComponent::getJointMatrix( uint index ) const
	//{
	//	TIKI_ASSERT( index >= 0 && index < m_poseMatrices.getCount() );
	//	return m_poseMatrices[ index ];
	//}

	//void SkinnedModelComponent::getJointPoseMatrix( uint index, Matrix& mat ) const
	//{
	//	TIKI_ASSERT( index >= 0 && index < m_poseMatrices.getCount() );
	//	mat = m_pModel->getHierarchy()->getSkinToBoneMatrix( index );
	//	mat.invert();
	//	mat.mul( m_poseMatrices[ index ] );
	//}

	//void SkinnedModelComponent::setMaterial( const Material* pMaterial )
	//{
	//	TIKI_ASSERT( pMaterial );

	//	if( m_ownMaterial )
	//		framework::getResourceManager().unloadResource( m_pMaterial );

	//	m_ownMaterial	= true;
	//	m_pMaterial		= pMaterial;
	//	m_pMaterial->addReference();	
	//}

	//void SkinnedModelComponent::setColor( const Color& color )
	//{
	//	m_color = color;
	//}

	//Color SkinnedModelComponent::getColor( void )
	//{
	//	return m_color;
	//}

	//void SkinnedModelComponent::render( GpuContext* pContext )
	//{
	//	const size_t componentCount = s_pool.getCount();

	//	for (size_t i = 0u; i < componentCount; ++i)
	//	{
	//		SkinnedModelComponent* pCurrentComp = s_pool[ i ];

	//		const Matrix& world = pCurrentComp->m_pTransform->getWorld();
	//		const ModelHierarchy& hierarchy = *pCurrentComp->m_pModel->getHierarchy();

	//		if ( hierarchy.getJointCount() > 1u && s_drawHierarchy )
	//		{
	//			const Quaternion* pRot	= (const Quaternion*)hierarchy.getDefaultPoseOrientation();
	//			const Vector3* pPos		= (const Vector3*)hierarchy.getDefaultPoseTranslation();
	//			const Vector3* pSca		= (const Vector3*)hierarchy.getDefaultPoseScale();

	//			Vector3 p1;
	//			Vector3 p2;
	//			
	//			for (size_t j = 0u; j < hierarchy.getJointCount(); ++j)
	//			{
	//				Matrix mtx = pCurrentComp->m_poseMatrices[ j ];
	//				Matrix mtx2;
	//				mtx2.invert( hierarchy.getSkinToBoneMatrix( j ) );

	//				pCurrentComp->m_poseMatrices[ j ].mul( mtx2, mtx );
	//			}

	//			for (size_t j = 1u; j < hierarchy.getJointCount(); ++j)
	//			{
	//				const uint16 parent = hierarchy.getParentByIndex( j );
	//				if ( parent == ModelHierarchy::InvalidBoneIndex ) continue;
	//				
	//				p1 = Vector3( 0.0f, 0.0f, 0.0f );
	//				p2 = Vector3( 0.0f, 0.0f, 0.0f );

	//				pCurrentComp->m_poseMatrices[ j ].transform( p1 );
	//				pCurrentComp->m_poseMatrices[ parent ].transform( p2 );

	//				world.transform( p1 );
	//				world.transform( p2 );

	//				pContext->drawLine( p1, p2, Color::blue );
	//			}

	//			//continue;
	//		}

	//		Pose* pPose = s_skinningBuffer.map( 1u );

	//		size_t j = 0u;
	//		for (; j < pCurrentComp->m_poseMatrices.getCount(); ++j)
	//		{
	//			pPose->pose[ j ] = pCurrentComp->m_poseMatrices[ j ];
	//		}

	//		s_skinningBuffer.unmap();
	//		s_skinningBuffer.apply( 2u );

	//		pContext->setMaterial( pCurrentComp->m_pMaterial );

	//		const size_t geoCount = pCurrentComp->m_pModel->getGeometryCount();
	//		for (size_t j = 0u; j < geoCount; ++j)
	//		{
	//			const ModelGeometry& geometry = pCurrentComp->m_pModel->getGeometryByIndex( j );

	//			pContext->setInputLayout( geometry.getVertexFormat() );

	//			WorldBuffer* world2 = MeshRenderer::s_worldBuffer.map( 1 );
	//			world2->m_world.createTranslation( pCurrentComp->m_renderOffset );// = world;
	//			world2->m_world.mul( world );
	//			world2->m_color = pCurrentComp->m_color;
	//			MeshRenderer::s_worldBuffer.unmap( );

	//			pContext->setConstantBuffer( MeshRenderer::s_worldBuffer, 1 );
	//			pContext->setVertexBuffer( geometry.getVertexBuffer() );
	//			pContext->setIndexBuffer( geometry.getIndexBuffer() );

	//			pContext->setPrimitiveTopology( PrimitiveTopology_TriangleList );

	//			pContext->drawIndexed( s_drawHierarchy ? 0u : geometry.getIndexCount() );
	//		}
	//	}
	//}


}