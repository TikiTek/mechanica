#include "tiki/physics3d/physics3dsphereshape.hpp"

namespace tiki
{
	Physics3dSphereShape::Physics3dSphereShape()
		: m_shape( 1.0f )
	{
	}

	Physics3dSphereShape::~Physics3dSphereShape()
	{
	}

	void Physics3dSphereShape::create( float radius )
	{
		m_shape = btSphereShape( radius * 0.5f );
	}

	void Physics3dSphereShape::dispose()
	{
	}

	void* Physics3dSphereShape::getNativeShape() const
	{
		return (void*)&m_shape;
	}
}