#include "tiki/physics3d/physics3dworld.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/debugprop.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/math/vector.hpp"
#include "tiki/physics3d/physics3dbody.hpp"
#include "tiki/physics3d/physics3dcharactercontroller.hpp"
#include "tiki/physics3d/physics3dcollider.hpp"
#include "tiki/physics3d/physics3dshape.hpp"

#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"

#include "physics3dinternal.hpp"

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
			m_collides = true;
			return 0.0f;
		}

	private:

		bool	m_collides;

	};

	Physics3dWorld::Physics3dWorld()
		: m_physicConfig(), m_physicDispatcher( &m_physicConfig ),
		m_physicOverlappingPair( btVector3( -1000.0f, -1000.0f, -1000.0f ), btVector3( 1000.0f, 1000.0f, 1000.0f ) )
	{
		m_pPhysicWorld = nullptr;
	}

	Physics3dWorld::~Physics3dWorld()
	{
		TIKI_ASSERT( m_pPhysicWorld == nullptr );
	}

	void Physics3dWorld::create( const Vector3& gravity )
	{
		m_pPhysicWorld = TIKI_MEMORY_NEW_OBJECT( btDiscreteDynamicsWorld )(
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

	void Physics3dWorld::dispose()
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
			TIKI_MEMORY_DELETE_OBJECT( m_pPhysicWorld );
			m_pPhysicWorld = nullptr;
		}
	}

	void Physics3dWorld::addBody( Physics3dBody& body )
	{
		TIKI_ASSERT( m_pPhysicWorld != nullptr );

		m_pPhysicWorld->addRigidBody( &body.m_ridgidBody );		
	}

	void Physics3dWorld::removeBody( Physics3dBody& body )
	{
		TIKI_ASSERT( m_pPhysicWorld != nullptr );

		m_pPhysicWorld->removeRigidBody( &body.m_ridgidBody );
	}

	void Physics3dWorld::addCollider( Physics3dCollider& collider )
	{
		TIKI_ASSERT( m_pPhysicWorld != nullptr );

		m_pPhysicWorld->addCollisionObject( &collider.m_collitionObject );
	}

	void Physics3dWorld::removeCollider( Physics3dCollider& collider )
	{
		TIKI_ASSERT( m_pPhysicWorld != nullptr );

		m_pPhysicWorld->removeCollisionObject( &collider.m_collitionObject );
	}
	
	void Physics3dWorld::addCharacterController( Physics3dCharacterController& controller )
	{
		TIKI_ASSERT( m_pPhysicWorld != nullptr );

		m_pPhysicWorld->addCollisionObject( &controller.m_ghostObject, btBroadphaseProxy::CharacterFilter, ( btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter ) );
		m_pPhysicWorld->addAction( &controller.m_controller );
	}

	void Physics3dWorld::removeCharacterController( Physics3dCharacterController& controller )
	{
		TIKI_ASSERT( m_pPhysicWorld != nullptr );

		m_pPhysicWorld->removeAction( &controller.m_controller );
		m_pPhysicWorld->removeCollisionObject( &controller.m_ghostObject );
	}

	bool Physics3dWorld::checkIntersection( const Physics3dShape& physicsShape, const Vector3& position ) const
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

	bool Physics3dWorld::checkIntersection( const Physics3dCollisionObject& physicsObject1, const Physics3dCollisionObject& physicsObject2 ) const
	{
		btCollisionObject* pCollisionObject1 = (btCollisionObject*)physicsObject1.getNativeObject();
		btCollisionObject* pCollisionObject2 = (btCollisionObject*)physicsObject2.getNativeObject();

		PhysicsCollisionCallback callback;
		m_pPhysicWorld->contactPairTest( pCollisionObject1, pCollisionObject2, callback );

		return callback.getCollides();
	}

	void Physics3dWorld::update( float timeStep )
	{
		TIKI_ASSERT( m_pPhysicWorld != nullptr );

		m_pPhysicWorld->stepSimulation( timeStep );

		m_pPhysicWorld->updateAabbs();
		m_pPhysicWorld->computeOverlappingPairs();
	}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	void Physics3dWorld::renderDebug()
	{
		if ( s_drawDebug )
		{
			m_pPhysicWorld->debugDrawWorld();
		}
	}
#endif
}