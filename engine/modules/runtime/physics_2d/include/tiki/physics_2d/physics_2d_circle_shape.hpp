#pragma once

#include "tiki/physics_2d/physics_2d_shape.hpp"

#include "tiki/base/types.hpp"

#include <box2d/b2_circle_shape.h>

namespace tiki
{
	class Physics2dCircleShape : public Physics2dShape
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dCircleShape );

	public:

							Physics2dCircleShape();
							~Physics2dCircleShape();

		void				create( float radius );
		void				dispose();

		virtual b2Shape*	getNativeShape() const TIKI_OVERRIDE_FINAL;

	private:

		b2CircleShape		m_shape;
	};
}
