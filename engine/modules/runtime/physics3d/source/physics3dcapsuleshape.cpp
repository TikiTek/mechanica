#include "tiki/physics3d/physics3dcapsuleshape.hpp"

namespace tiki
{
	Physics3dCapsuleShape::Physics3dCapsuleShape()
		: m_shape( 1.0f, 1.0f )
	{
	}

	Physics3dCapsuleShape::~Physics3dCapsuleShape()
	{
	}

	void Physics3dCapsuleShape::create( float height, float radius )
	{
		m_shape = btCapsuleShape( height * 0.5f, radius * 0.5f );
	}

	void Physics3dCapsuleShape::dispose()
	{
	}

	void* Physics3dCapsuleShape::getNativeShape() const
	{
		return (void*)&m_shape;
	}
}