#pragma once
#ifndef __TIKI_PHYSICSSPHEREBODY_HPP_INCLUDED__
#define __TIKI_PHYSICSSPHEREBODY_HPP_INCLUDED__

#include "tiki/physics/physicsbody.hpp"

#include "BulletCollision/CollisionShapes/btSphereShape.h"

namespace tiki
{
	class PhysicsSphereBody : public PhysicsBody
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsSphereBody );

	public:

						PhysicsSphereBody();
						~PhysicsSphereBody();

		void			create( const Vector3& position, float radius, float mass );
		void			dispose();

	private:

		btSphereShape	m_shape;

	};
}

#endif // __TIKI_PHYSICSSPHEREBODY_HPP_INCLUDED__
