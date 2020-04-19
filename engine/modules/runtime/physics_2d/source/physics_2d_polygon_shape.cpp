#include "tiki/physics_2d/physics_2d_polygon_shape.hpp"

namespace tiki
{
	Physics2dPolygonShape::Physics2dPolygonShape()
	{
	}

	Physics2dPolygonShape::~Physics2dPolygonShape()
	{
	}

	void Physics2dPolygonShape::create( const Vector2* pVertices, uintreg vertexCount )
	{
		m_shape.Set( (const b2Vec2*)pVertices, (int)vertexCount );
	}

	void Physics2dPolygonShape::dispose()
	{
	}

	b2Shape* Physics2dPolygonShape::getNativeShape() const
	{
		return const_cast<b2Shape*>(static_cast<const b2Shape*>(&m_shape));
	}
}