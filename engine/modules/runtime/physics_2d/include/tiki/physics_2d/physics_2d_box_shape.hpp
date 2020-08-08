#pragma once

#include "tiki/base/types.hpp"
#include "tiki/physics_2d/physics_2d_shape.hpp"

#include <box2d/b2_polygon_shape.h>

namespace tiki
{
	struct Vector2;

	class Physics2dBoxShape : public Physics2dShape
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dBoxShape );

	public:

							Physics2dBoxShape();
							~Physics2dBoxShape();

		void				create( const Vector2& size );
		void				dispose();

		virtual b2Shape*	getNativeShape() const TIKI_OVERRIDE_FINAL;

	private:

		b2PolygonShape		m_shape;
	};
}
