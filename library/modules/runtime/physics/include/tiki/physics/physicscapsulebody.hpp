#pragma once
#ifndef __TIKI_PHYSICSCAPSULEBODY_HPP_INCLUDED__
#define __TIKI_PHYSICSCAPSULEBODY_HPP_INCLUDED__

#include "tiki/physics/physicsbody.hpp"

#include "BulletCollision/CollisionShapes/btCapsuleShape.h"

namespace tiki
{
	struct Vector3;

	class PhysicsCapsuleBody : public PhysicsBody
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsCapsuleBody );

	public:

						PhysicsCapsuleBody();
						~PhysicsCapsuleBody();

		void			create( const Vector3& position, float height, float radius, float mass );
		void			dispose();

	private:

		btCapsuleShape	m_shape;

	};
}

#endif // __TIKI_PHYSICSCAPSULEBODY_HPP_INCLUDED__
