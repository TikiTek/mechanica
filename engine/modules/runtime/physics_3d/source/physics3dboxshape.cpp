#include "tiki/physics3d/physics3dboxshape.hpp"

#include "tiki/math/vector.hpp"

#include "physics3dinternal.hpp"

namespace tiki
{
	Physics3dBoxShape::Physics3dBoxShape()
		: m_shape( btVector3( 1.0f, 1.0f, 1.0f ) )
	{
	}

	Physics3dBoxShape::~Physics3dBoxShape()
	{
	}
	
	void Physics3dBoxShape::create( const Vector3& size )
	{
		m_shape = btBoxShape( toBulletVector( size ) * 0.5f );
	}

	void Physics3dBoxShape::dispose()
	{
	}

	void* Physics3dBoxShape::getNativeShape() const
	{
		return (void*)&m_shape;
	}
}