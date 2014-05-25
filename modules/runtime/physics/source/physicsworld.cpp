
#include "tiki/physics/physicsworld.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/debugprop.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/physics/physicsbody.hpp"
#include "tiki/physics/physicscharactercontroller.hpp"
#include "tiki/physics/physicscollider.hpp"

#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"

namespace tiki
{	
	TIKI_DEBUGPROP_BOOL( s_drawDebug, "DrawDebug", true );

	class PhysicsCollisionCallback : public btCollisionWorld::ContactResultCallback
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsCollisionCallback );

	public:

		PhysicsCollisionCallback()
		{
			m_collides = false;
		}

		bool getCollides() const
		{
			return m_collides;	
		}

		virtual	btScalar addSingleResult( btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1 )
		{
			TIKI_TRACE( "test\n" );

			m_collides = true;

			return 0.0f;
		}

	private:

		bool	m_collides;

	};

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

		m_physicOverlappingPair.getOverlappingPairCache()->setInternalGhostPairCallback( &m_ghostPairCallback );

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

	void PhysicsWorld::addBody( PhysicsBody& body )
	{
		TIKI_ASSERT( m_pPhysicWorld != nullptr );

		m_pPhysicWorld->addRigidBody( &body.m_ridgidBody );		
	}

	void PhysicsWorld::removeBody( PhysicsBody& body )
	{
		TIKI_ASSERT( m_pPhysicWorld != nullptr );

		m_pPhysicWorld->removeRigidBody( &body.m_ridgidBody );
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


	void PhysicsWorld::addCharacterController( PhysicsCharacterController& controller )
	{
		TIKI_ASSERT( m_pPhysicWorld != nullptr );

		m_pPhysicWorld->addCollisionObject( &controller.m_ghostObject, btBroadphaseProxy::CharacterFilter, ( btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter ) );
		m_pPhysicWorld->addAction( &controller.m_controller );
	}

	void PhysicsWorld::removeCharacterController( PhysicsCharacterController& controller )
	{
		TIKI_ASSERT( m_pPhysicWorld != nullptr );

		m_pPhysicWorld->removeAction( &controller.m_controller );
		m_pPhysicWorld->removeCollisionObject( &controller.m_ghostObject );
	}

	bool PhysicsWorld::checkIntersection( const PhysicsShape& physicsShape, const Vector3& position ) const
	{
		btCollisionShape* pShape = static_cast< btCollisionShape* >( physicsShape.getNativeShape() );

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin( toBulletVector( position ) );

		btCollisionObject object;
		object.setCollisionShape( pShape );
		object.setWorldTransform( transform );

		PhysicsCollisionCallback callback;

		m_pPhysicWorld->contactTest( &object, callback );

		return callback.getCollides();
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