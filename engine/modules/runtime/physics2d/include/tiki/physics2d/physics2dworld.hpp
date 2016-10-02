#pragma once
#ifndef TIKI_PHYSICS2DWORLD_HPP_INCLUDED
#define TIKI_PHYSICS2DWORLD_HPP_INCLUDED

#include "tiki/base/types.hpp"


#if TIKI_DISABLED( TIKI_BUILD_MASTER )
#	include "tiki/physics3d/physics3ddebugdraw.hpp"
#endif

#include <Box2D/Dynamics/b2World.h>

namespace tiki
{
	struct Vector2;

	class Physics2dWorld
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dWorld );

	public:

					Physics2dWorld();
					~Physics2dWorld();

		void		create( const Vector2& gravity );
		void		dispose();

		//void		addBody( PhysicsBody& body );
		//void		removeBody( PhysicsBody& body );
		//void		addCollider( PhysicsCollider& collider );
		//void		removeCollider( PhysicsCollider& collider );
		//void		addCharacterController( PhysicsCharacterController& controller );
		//void		removeCharacterController( PhysicsCharacterController& controller );

		//bool		checkIntersection( const PhysicsShape& physicsShape, const Vector3& position ) const;
		//bool		checkIntersection( const PhysicsCollisionObject& physicsObject1, const PhysicsCollisionObject& physicsObject2 ) const;

		void		update( float timeStep );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		void		renderDebug();
#endif

	private:

		b2World					m_world;

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		Physics2dDebugDraw		m_debugDraw;
#endif

	};
}

#endif // TIKI_PHYSICS2DWORLD_HPP_INCLUDED