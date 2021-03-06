#include "tiki/physics_2d/physics_2d_box_shape.hpp"

#include "tiki/math/vector.hpp"

#include "physics_2d_internal.hpp"

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
		m_shape.SetAsBox( size.x * 0.5f, size.y * 0.5f );
	}

	void Physics2dBoxShape::dispose()
	{
	}

	b2Shape* Physics2dBoxShape::getNativeShape() const
	{
		return const_cast< b2Shape* >( static_cast< const b2Shape* >( &m_shape ) );
	}
}