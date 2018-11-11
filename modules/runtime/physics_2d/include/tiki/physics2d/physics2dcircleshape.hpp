#pragma once
#ifndef TIKI_PHYSICS2DCIRCLESHAPE_HPP_INCLUDED
#define TIKI_PHYSICS2DCIRCLESHAPE_HPP_INCLUDED

#include "tiki/physics2d/physics2dshape.hpp"

#include "tiki/base/types.hpp"

#include <Box2D/Collision/Shapes/b2CircleShape.h>

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

#endif // TIKI_PHYSICS2DCIRCLESHAPE_HPP_INCLUDED
