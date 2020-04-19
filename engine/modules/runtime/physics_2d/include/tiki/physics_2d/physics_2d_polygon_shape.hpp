#pragma once

#include "tiki/physics_2d/physics_2d_shape.hpp"

#include "tiki/base/types.hpp"

#include <Box2D/Collision/Shapes/b2PolygonShape.h>

namespace tiki
{
	class Physics2dPolygonShape : public Physics2dShape
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dPolygonShape );

	public:

							Physics2dPolygonShape();
							~Physics2dPolygonShape();

		void				create( const Vector2* pVertices, uintreg vertexCount );
		void				dispose();

		virtual b2Shape*	getNativeShape() const TIKI_OVERRIDE_FINAL;

	private:

		b2PolygonShape		m_shape;
	};
}
