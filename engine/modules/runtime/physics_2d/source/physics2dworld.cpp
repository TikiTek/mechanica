#include "tiki/physics2d/physics2dworld.hpp"

#include "tiki/base/assert.hpp"
#include "tiki/base/debugprop.hpp"
#include "tiki/base/memory.hpp"
#include "tiki/math/vector.hpp"

#include "physics2dinternal.hpp"

namespace tiki
{	
	TIKI_DEBUGPROP_BOOL( s_drawDebug, "Physics2dDrawDebug", true );
	TIKI_DEBUGPROP_INT( s_stepVelocityIterations, "StepVelocityIterations", 6, 1, 100 );
	TIKI_DEBUGPROP_INT( s_stepPositionIterations, "StepPositionIterations", 2, 1, 100 );

	Physics2dWorld::Physics2dWorld()
		: m_world( b2Vec2() )
	{
	}

	Physics2dWorld::~Physics2dWorld()
	{
	}

	void Physics2dWorld::create( const Vector2& gravity, float simulationToDrawFactor )
	{
		m_world.SetGravity( toPhysicsVector( gravity ) );

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_debugDraw.create( simulationToDrawFactor );
		m_world.SetDebugDraw( &m_debugDraw );
#endif
	}

	void Physics2dWorld::dispose()
	{
#if TIKI_DISABLED( TIKI_BUILD_MASTER )
		m_world.SetDebugDraw( nullptr );
		m_debugDraw.dispose();
#endif
	}

	//bool Physics2dWorld::checkIntersection( const PhysicsShape& physicsShape, const Vector3& position ) const
	//{
	//	btCollisionShape* pShape = static_cast< btCollisionShape* >( physicsShape.getNativeShape() );

	//	btTransform transform;
	//	transform.setIdentity();
	//	transform.setOrigin( toBulletVector( position ) );

	//	btCollisionObject object;
	//	object.setCollisionShape( pShape );
	//	object.setWorldTransform( transform );

	//	PhysicsCollisionCallback callback;

	//	m_pPhysicWorld->contactTest( &object, callback );
	//	
	//	return callback.getCollides();
	//}

	//bool Physics2dWorld::checkIntersection( const PhysicsCollisionObject& physicsObject1, const PhysicsCollisionObject& physicsObject2 ) const
	//{
	//	btCollisionObject* pCollisionObject1 = (btCollisionObject*)physicsObject1.getNativeObject();
	//	btCollisionObject* pCollisionObject2 = (btCollisionObject*)physicsObject2.getNativeObject();

	//	PhysicsCollisionCallback callback;
	//	m_pPhysicWorld->contactPairTest( pCollisionObject1, pCollisionObject2, callback );

	bool Physics2dWorld::rayCast( const Vector2& startPoint, Vector2& endPoint, PhysicsRayCastCallback pCallback, void* pUserData )
	{		
		m_pRayCastCallback	= pCallback;
		m_pRayCastUserData	= pUserData;
		m_rayCastResult		= false;

		m_world.RayCast( this, toPhysicsVector( startPoint ), toPhysicsVector( endPoint ) );

		return m_rayCastResult;
	}

	//	return callback.getCollides();
	//}

	void Physics2dWorld::update( float timeStep )
	{
		m_world.Step( timeStep, s_stepVelocityIterations, s_stepPositionIterations );
	}

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
	void Physics2dWorld::renderDebug()
	{
		if ( s_drawDebug )
		{
			m_world.DrawDebugData();
		}
	}
#endif

	float32 Physics2dWorld::ReportFixture( b2Fixture* pFixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction )
	{
		m_rayCastResult = true;

		Physics2dCollisionObject* pObject = (Physics2dCollisionObject*)pFixture->GetBody()->GetUserData();

		const bool continue2 = m_pRayCastCallback( pObject, toEngineVector( point ), m_pRayCastUserData );
		return continue2 ? -1.0f : 0.0f;
	}
}