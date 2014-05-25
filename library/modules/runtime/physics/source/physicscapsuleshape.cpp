
#include "tiki/physics/physicscapsuleshape.hpp"

namespace tiki
{
	PhysicsCapsuleShape::PhysicsCapsuleShape()
		: m_shape( 1.0f, 1.0f )
	{
	}

	PhysicsCapsuleShape::~PhysicsCapsuleShape()
	{
	}

	void PhysicsCapsuleShape::create( float height, float radius )
	{
		m_shape = btCapsuleShape( height * 0.5f, radius * 0.5f );
	}

	void PhysicsCapsuleShape::dispose()
	{
	}

	void* PhysicsCapsuleShape::getNativeShape() const
	{
		return (void*)&m_shape;
	}
}