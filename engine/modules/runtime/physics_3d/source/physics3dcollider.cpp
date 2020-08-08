#include "tiki/physics3d/physics3dcollider.hpp"

#include "tiki/math/vector.hpp"
#include "tiki/physics3d/physics3dshape.hpp"

#include "physics3dinternal.hpp"

namespace tiki
{
	Physics3dCollider::Physics3dCollider()
	{
	}

	Physics3dCollider::~Physics3dCollider()
	{
		TIKI_ASSERT( m_collitionObject.getUserPointer() == nullptr );
	}

	void Physics3dCollider::create( const Physics3dShape& shape, const Vector3& position )
	{
		dispose();

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin( toBulletVector( position ) );

		m_collitionObject.setCollisionShape( static_cast< btCollisionShape* >( shape.getNativeShape() ) );
		m_collitionObject.setWorldTransform( transform );
		m_collitionObject.setUserPointer( this );
	}

	void Physics3dCollider::dispose()
	{
		m_collitionObject.setUserPointer( nullptr );
	}

	void* Physics3dCollider::getNativeObject() const
	{
		return (void*)&m_collitionObject;
	}
}