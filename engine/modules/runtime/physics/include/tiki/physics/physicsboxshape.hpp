#pragma once
#ifndef __TIKI_PHYSICSBOXSHAPE_HPP_INCLUDED__
#define __TIKI_PHYSICSBOXSHAPE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/physics/physicsshape.hpp"

#include "BulletCollision/CollisionShapes/btBoxShape.h"

namespace tiki
{
	struct Vector3;

	class PhysicsBoxShape : public PhysicsShape
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsBoxShape );

	public:

						PhysicsBoxShape();
						~PhysicsBoxShape();

		void			create( const Vector3& size );
		void			dispose();

		virtual void*	getNativeShape() const;

	private:

		btBoxShape		m_shape;

	};
}

#endif // __TIKI_PHYSICSBOXSHAPE_HPP_INCLUDED__
