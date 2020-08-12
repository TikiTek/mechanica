
#include "tiki/physics/physicssphereshape.hpp"

namespace tiki
{
	PhysicsSphereShape::PhysicsSphereShape()
		: m_shape( 1.0f )
	{
	}

	PhysicsSphereShape::~PhysicsSphereShape()
	{
	}

	void PhysicsSphereShape::create( float radius )
	{
		m_shape = btSphereShape( radius * 0.5f );
	}

	void PhysicsSphereShape::dispose()
	{
	}

	void* PhysicsSphereShape::getNativeShape() const
	{
		return (void*)&m_shape;
	}
}