#pragma once
#ifndef __TIKI_PHYSICSBOXBODY_HPP_INCLUDED__
#define __TIKI_PHYSICSBOXBODY_HPP_INCLUDED__

#include "tiki/physics/physicsbody.hpp"

#include "BulletCollision/CollisionShapes/btBoxShape.h"

namespace tiki
{
	struct Vector3;

	class PhysicsBoxBody : public PhysicsBody
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsBoxBody );

	public:

					PhysicsBoxBody();
					~PhysicsBoxBody();

		void		create( const Vector3& position, const Vector3& size, float mass );
		void		dispose();

	private:

		btBoxShape	m_shape;

	};
}

#endif // __TIKI_PHYSICSBOXBODY_HPP_INCLUDED__
