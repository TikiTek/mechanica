#include "tiki/physics2d/physics2dboxshape.hpp"

#include "tiki/math/vector.hpp"

#include "physics2dinternal.hpp"

namespace tiki
{
	Physics2dBoxShape::Physics2dBoxShape()
	{
	}

	Physics2dBoxShape::~Physics2dBoxShape()
	{
	}
	
	void Physics2dBoxShape::create( const Vector2& size )
	{
		m_shape.SetAsBox( size.x, size.y );
	}

	void Physics2dBoxShape::dispose()
	{
	}

	b2Shape* Physics2dBoxShape::getNativeShape() const
	{
		return const_cast< b2Shape* >( static_cast< const b2Shape* >( &m_shape ) );
	}
}