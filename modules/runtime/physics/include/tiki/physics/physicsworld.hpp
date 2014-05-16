#pragma once
#ifndef __TIKI_PHYSICSWORLD_HPP_INCLUDED__
#define __TIKI_PHYSICSWORLD_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
#	include "tiki/physics/physicsdebugdraw.hpp"
#endif

class btDiscreteDynamicsWorld;

namespace tiki
{
	class PhysicsCollider;
	struct Vector3;

	class PhysicsWorld
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsWorld );

	public:

					PhysicsWorld();
					~PhysicsWorld();

		void		create( const Vector3& gravity );
		void		dispose();

		void		addCollider( PhysicsCollider& collider );
		void		removeCollider( PhysicsCollider& collider );

		void		update( float timeStep );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		void		renderDebug( GraphicsContext& graphicsContext, ImmediateRenderer& renderer, const RenderTarget& renderTarget, const Camera& camera );
#endif

	private:

		btDefaultCollisionConfiguration		m_physicConfig;
		btCollisionDispatcher				m_physicDispatcher;
		btAxisSweep3						m_physicOverlappingPair;

		btDiscreteDynamicsWorld*			m_pPhysicWorld;

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		PhysicsDebugDraw					m_debugDraw;
#endif

	};
}

#endif // __TIKI_PHYSICSWORLD_HPP_INCLUDED__
