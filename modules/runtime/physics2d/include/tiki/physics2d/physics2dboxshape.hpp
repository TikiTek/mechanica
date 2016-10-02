#pragma once
#ifndef __TIKI_PHYSICSBOXSHAPE_HPP_INCLUDED__
#define __TIKI_PHYSICSBOXSHAPE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/physics2d/physics2dshape.hpp"

#include <Box2D/Collision/Shapes/b2PolygonShape.h>

namespace tiki
{
	struct Vector2;

	class Physics2dBoxShape : public Physics2dShape
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dBoxShape );

	public:

						Physics2dBoxShape();
						~Physics2dBoxShape();

		void			create( const Vector2& size );
		void			dispose();

		virtual void*	getNativeShape() const TIKI_OVERRIDE TIKI_FINAL;

	private:

		b2PolygonShape	m_shape;
	};
}

#endif // __TIKI_PHYSICSBOXSHAPE_HPP_INCLUDED__
