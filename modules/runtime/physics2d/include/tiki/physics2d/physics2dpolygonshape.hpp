#pragma once
#ifndef TIKI_PHYSICS2DPOLYGONSHAPE_HPP_INCLUDED
#define TIKI_PHYSICS2DPOLYGONSHAPE_HPP_INCLUDED

#include "tiki/physics2d/physics2dshape.hpp"

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

		void				create( const Vector2* pVertices, size_t vertexCount );
		void				dispose();

		virtual b2Shape*	getNativeShape() const TIKI_OVERRIDE_FINAL;

	private:

		b2PolygonShape		m_shape;
	};
}

#endif // TIKI_PHYSICS2DPOLYGONSHAPE_HPP_INCLUDED
