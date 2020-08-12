#pragma once
#ifndef __TIKI_PHYSICSCAPSULESHAPE_HPP_INCLUDED__
#define __TIKI_PHYSICSCAPSULESHAPE_HPP_INCLUDED__

#include "tiki/physics/physicsshape.hpp"

#include "tiki/base/types.hpp"

#include "BulletCollision/CollisionShapes/btCapsuleShape.h"

namespace tiki
{
	struct Vector3;

	class PhysicsCapsuleShape : public PhysicsShape
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsCapsuleShape );

	public:

						PhysicsCapsuleShape();
						~PhysicsCapsuleShape();

		void			create( float height, float radius );
		void			dispose();

		virtual void*	getNativeShape() const;

	private:

		btCapsuleShape	m_shape;

	};
}

#endif // __TIKI_PHYSICSCAPSULESHAPE_HPP_INCLUDED__
