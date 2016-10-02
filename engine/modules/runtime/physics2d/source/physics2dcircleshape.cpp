#include "tiki/physics2d/physics2dcircleshape.hpp"

namespace tiki
{
	Physics2dCircleShape::Physics2dCircleShape()
	{
	}

	Physics2dCircleShape::~Physics2dCircleShape()
	{
	}

	void Physics2dCircleShape::create( float radius )
	{
		m_shape.m_radius = radius;
	}

	void Physics2dCircleShape::dispose()
	{
	}

	void* Physics2dCircleShape::getNativeShape() const
	{
		return (void*)&m_shape;
	}
}