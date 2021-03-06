#pragma once

#include "tiki/base/types.hpp"

#if TIKI_DISABLED( TIKI_BUILD_MASTER )
#	include "tiki/physics_2d/physics_2d_debug_draw.hpp"
#endif

#include <box2d/b2_world.h>

namespace tiki
{
	class Physics2dCollisionObject;
	struct Vector2;

	typedef bool( *PhysicsRayCastCallback )( const Physics2dCollisionObject* pObject, const Vector2& point, void* pUserData );

	class Physics2dWorld : private b2RayCastCallback
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dWorld );

	public:

					Physics2dWorld();
					~Physics2dWorld();

		void		create( const Vector2& gravity, float simulationToDrawFactor );
		void		dispose();

		b2World&	getNativeWorld() { return m_world; }

		bool		rayCast( const Vector2& startPoint, Vector2& endPoint, PhysicsRayCastCallback pCallback, void* pUserData );

		//bool		checkIntersection( const PhysicsShape& physicsShape, const Vector3& position ) const;
		//bool		checkIntersection( const PhysicsCollisionObject& physicsObject1, const PhysicsCollisionObject& physicsObject2 ) const;

		void		update( double timeStep );

#if TIKI_ENABLED( TIKI_DEBUG_RENDERER )
		void		renderDebug();
#endif

	private:

		b2World					m_world;

#if TIKI_ENABLED( TIKI_DEBUG_RENDERER )
		Physics2dDebugDraw		m_debugDraw;
#endif

		PhysicsRayCastCallback	m_pRayCastCallback;
		void*					m_pRayCastUserData;
		bool					m_rayCastResult;

		virtual float32			ReportFixture( b2Fixture* pFixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction ) TIKI_OVERRIDE_FINAL;
	};
}
