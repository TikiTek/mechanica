
#include "tiki/physics/physicsboxshape.hpp"

#include "tiki/math/vector.hpp"

#include "physicsinternal.hpp"

namespace tiki
{
	PhysicsBoxShape::PhysicsBoxShape()
		: m_shape( btVector3( 1.0f, 1.0f, 1.0f ) )
	{
	}

	PhysicsBoxShape::~PhysicsBoxShape()
	{
	}
	
	void PhysicsBoxShape::create( const Vector3& size )
	{
		m_shape = btBoxShape( toBulletVector( size ) * 0.5f );
	}

	void PhysicsBoxShape::dispose()
	{
	}

	void* PhysicsBoxShape::getNativeShape() const
	{
		return (void*)&m_shape;
	}
}