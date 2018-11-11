#pragma once
#ifndef TIKI_PHYSICS3DWORLD_HPP_INCLUDED
#define TIKI_PHYSICS3DWORLD_HPP_INCLUDED

#include "tiki/base/types.hpp"

#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
#	include "tiki/physics3d/physics3ddebugdraw.hpp"
#endif

class btDiscreteDynamicsWorld;

namespace tiki
{
	class Physics3dBody;
	class Physics3dCharacterController;
	class Physics3dCollider;
	class Physics3dCollisionObject;
	class Physics3dShape;
	struct Vector3;

	class Physics3dWorld
	{
		TIKI_NONCOPYABLE_CLASS( Physics3dWorld );

	public:

					Physics3dWorld();
					~Physics3dWorld();

		void		create( const Vector3& gravity );
		void		dispose();

		void		addBody( Physics3dBody& body );
		void		removeBody( Physics3dBody& body );
		void		addCollider( Physics3dCollider& collider );
		void		removeCollider( Physics3dCollider& collider );
		void		addCharacterController( Physics3dCharacterController& controller );
		void		removeCharacterController( Physics3dCharacterController& controller );

		bool		checkIntersection( const Physics3dShape& physicsShape, const Vector3& position ) const;
		bool		checkIntersection( const Physics3dCollisionObject& physicsObject1, const Physics3dCollisionObject& physicsObject2 ) const;

		void		update( float timeStep );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		void		renderDebug();
#endif

	private:

		btDefaultCollisionConfiguration		m_physicConfig;
		btCollisionDispatcher				m_physicDispatcher;
		btAxisSweep3						m_physicOverlappingPair;
		btGhostPairCallback					m_ghostPairCallback;

		btDiscreteDynamicsWorld*			m_pPhysicWorld;

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		Physics3dDebugDraw					m_debugDraw;
#endif
	};
}

#endif // TIKI_PHYSICS3DWORLD_HPP_INCLUDED
