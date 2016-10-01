#pragma once
#ifndef TIKI_PHYSICS3DCAPSULESHAPE_HPP_INCLUDED
#define TIKI_PHYSICS3DCAPSULESHAPE_HPP_INCLUDED

#include "tiki/physics3d/physics3dshape.hpp"

#include "tiki/base/types.hpp"

#include "BulletCollision/CollisionShapes/btCapsuleShape.h"

namespace tiki
{
	struct Vector3;

	class Physics3dCapsuleShape : public Physics3dShape
	{
		TIKI_NONCOPYABLE_CLASS( Physics3dCapsuleShape );

	public:

						Physics3dCapsuleShape();
						~Physics3dCapsuleShape();

		void			create( float height, float radius );
		void			dispose();

		virtual void*	getNativeShape() const;

	private:

		btCapsuleShape	m_shape;

	};
}

#endif // TIKI_PHYSICS3DCAPSULESHAPE_HPP_INCLUDED
