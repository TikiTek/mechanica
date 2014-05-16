
#include "tiki/physics/physicsworld.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/debugprop.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/physics/physicscollider.hpp"

#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"

namespace tiki
{	
	TIKI_DEBUGPROP_BOOL( s_drawDebug, "DrawDebug", true );

	PhysicsWorld::PhysicsWorld()
		: m_physicConfig(), m_physicDispatcher( &m_physicConfig ),
		m_physicOverlappingPair( btVector3( -1000.0f, -1000.0f, -1000.0f ), btVector3( 1000.0f, 1000.0f, 1000.0f ) )
	{
		m_pPhysicWorld = nullptr;
	}

	PhysicsWorld::~PhysicsWorld()
	{
		TIKI_ASSERT( m_pPhysicWorld == nullptr );
	}

	void PhysicsWorld::create( const Vector3& gravity )
	{
		m_pPhysicWorld = new btDiscreteDynamicsWorld(
			&m_physicDispatcher,
			&m_physicOverlappingPair,
			nullptr,
			&m_physicConfig
		);
		m_pPhysicWorld->setGravity( btVector3( gravity.x, gravity.y, gravity.z ) );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_debugDraw.create();
		m_pPhysicWorld->setDebugDrawer( &m_debugDraw );
#endif
	}

	void PhysicsWorld::dispose()
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		if ( m_pPhysicWorld != nullptr )
		{
			m_pPhysicWorld->setDebugDrawer( nullptr );
		}
		m_debugDraw.dispose();
#endif

		if ( m_pPhysicWorld != nullptr )
		{
			TIKI_DEL m_pPhysicWorld;
			m_pPhysicWorld = nullptr;
		}
	}

	void PhysicsWorld::addCollider( PhysicsCollider& collider )
	{
		TIKI_ASSERT( m_pPhysicWorld != nullptr );

		m_pPhysicWorld->addCollisionObject( &collider.m_collitionObject );
	}

	void PhysicsWorld::removeCollider( PhysicsCollider& collider )
	{
		TIKI_ASSERT( m_pPhysicWorld != nullptr );

		m_pPhysicWorld->removeCollisionObject( &collider.m_collitionObject );
	}

	void PhysicsWorld::update( float timeStep )
	{
		TIKI_ASSERT( m_pPhysicWorld != nullptr );

		m_pPhysicWorld->stepSimulation( timeStep );

		m_pPhysicWorld->updateAabbs();
		m_pPhysicWorld->computeOverlappingPairs();
	}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	void PhysicsWorld::renderDebug( GraphicsContext& graphicsContext, ImmediateRenderer& renderer, const RenderTarget& renderTarget, const Camera& camera )
	{
		if ( s_drawDebug )
		{
			m_debugDraw.beginRendering( graphicsContext, renderer, renderTarget, camera );

			m_pPhysicWorld->debugDrawWorld();

			m_debugDraw.endRendering();
		}
	}
#endif
}