#pragma once
#ifndef TIKI_PHYSICS3DBOXSHAPE_HPP_INCLUDED
#define TIKI_PHYSICS3DBOXSHAPE_HPP_INCLUDED

#include "tiki/base/types.hpp"
#include "tiki/physics3d/physics3dshape.hpp"

#include "BulletCollision/CollisionShapes/btBoxShape.h"

namespace tiki
{
	struct Vector3;

	class Physics3dBoxShape : public Physics3dShape
	{
		TIKI_NONCOPYABLE_CLASS( Physics3dBoxShape );

	public:

						Physics3dBoxShape();
						~Physics3dBoxShape();

		void			create( const Vector3& size );
		void			dispose();

		virtual void*	getNativeShape() const;

	private:

		btBoxShape		m_shape;
	};
}

#endif // TIKI_PHYSICS3DBOXSHAPE_HPP_INCLUDED
