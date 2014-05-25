#pragma once
#ifndef __TIKI_PHYSICSSPHERESHAPE_HPP_INCLUDED__
#define __TIKI_PHYSICSSPHERESHAPE_HPP_INCLUDED__

#include "tiki/base/types.hpp"
#include "tiki/physics/physicsshape.hpp"

#include "BulletCollision/CollisionShapes/btSphereShape.h"

namespace tiki
{
	class PhysicsSphereShape : public PhysicsShape
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsSphereShape );

	public:

						PhysicsSphereShape();
						~PhysicsSphereShape();

		void			create( float radius );
		void			dispose();

		virtual void*	getNativeShape() const;

	private:

		btSphereShape	m_shape;

	};
}

#endif // __TIKI_PHYSICSSPHERESHAPE_HPP_INCLUDED__
