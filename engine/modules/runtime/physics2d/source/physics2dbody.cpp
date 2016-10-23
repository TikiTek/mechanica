#include "tiki/physics2d/physics2dbody.hpp"

#include "tiki/math/vector.hpp"
#include "tiki/physics2d/physics2dshape.hpp"
#include "tiki/physics2d/physics2dworld.hpp"

#include "physics2dinternal.hpp"

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Dynamics/b2World.h>

namespace tiki
{
	Physics2dBody::Physics2dBody()
	{
		m_pBody		= nullptr;
		m_pFixture	= nullptr;
	}

	Physics2dBody::~Physics2dBody()
	{
		TIKI_ASSERT( m_pBody	== nullptr );
		TIKI_ASSERT( m_pFixture	== nullptr );
	}

	bool Physics2dBody::create( Physics2dWorld& world, const Physics2dShape& shape, const Vector2& position, float density, float friction, bool fixedRotation /* = false */ )
	{
		TIKI_ASSERT( m_pBody == nullptr );

		b2BodyDef bodyDef;
		bodyDef.type			= b2_dynamicBody;
		bodyDef.position		= toPhysicsVector( position );
		bodyDef.fixedRotation	= fixedRotation;
		bodyDef.userData		= this;

		m_pBody = world.getNativeWorld().CreateBody( &bodyDef );
		if( m_pBody == nullptr )
		{
			return false;
		}

		b2FixtureDef fixtureDef;
		fixtureDef.shape	= (b2Shape*)shape.getNativeShape();
		fixtureDef.density	= density;
		fixtureDef.friction	= friction;
		fixtureDef.userData	= this;

		m_pFixture = m_pBody->CreateFixture( &fixtureDef );
		if( m_pFixture == nullptr )
		{
			dispose( world );
			return false;
		}
		
		return true;
	}

	void Physics2dBody::dispose( Physics2dWorld& world )
	{
		if( m_pFixture != nullptr )
		{
			m_pBody->DestroyFixture( m_pFixture );
			m_pFixture = nullptr;
		}

		if( m_pBody != nullptr )
		{
			world.getNativeWorld().DestroyBody( m_pBody );
			m_pBody = nullptr;
		}
	}

	void Physics2dBody::applyForce( const Vector2& force, const Vector2& worldPoint )
	{
		m_pBody->ApplyForce( toPhysicsVector( force ), toPhysicsVector( worldPoint ), true );
	}

	void Physics2dBody::applyLocalForce( const Vector2& force, const Vector2& localPoint /*= Vector2::zero */ )
	{
		Vector2 worldPoint;
		vector::add( worldPoint, getPosition(), localPoint );

		applyForce( force, worldPoint );
	}

	void Physics2dBody::applyLinearImpluse( const Vector2& impulse, const Vector2& worldPoint )
	{
		m_pBody->ApplyLinearImpulse( toPhysicsVector( impulse ), toPhysicsVector( worldPoint ), true );
	}

	void Physics2dBody::applyLocalLinearImpluse( const Vector2& impulse, const Vector2& localPoint /*= Vector2::zero */ )
	{
		Vector2 worldPoint;
		vector::add( worldPoint, getPosition(), localPoint );

		applyLinearImpluse( impulse, worldPoint );
	}

	Vector2 Physics2dBody::getPosition() const
	{
		return toEngineVector( m_pBody->GetPosition() );
	}

	float Physics2dBody::getRotation() const
	{
		return m_pBody->GetAngle();
	}

	Vector2 Physics2dBody::getLinearVelocity() const
	{
		return toEngineVector( m_pBody->GetLinearVelocity() );
	}

	void Physics2dBody::setLinearVelocity( const Vector2& value ) const
	{
		m_pBody->SetLinearVelocity( toPhysicsVector( value ) );
	}

	float Physics2dBody::getAngularVelocity() const
	{
		return m_pBody->GetAngularVelocity();
	}

	void Physics2dBody::setAngularVelocity( float value ) const
	{
		m_pBody->SetAngularVelocity( value );
	}

	AxisAlignedRectangle Physics2dBody::getShapeBounds() const
	{
		b2AABB aabb;
		m_pFixture->GetShape()->ComputeAABB( &aabb, m_pBody->GetTransform(), 0u );

		return createAxisAlignedRectangleMinMax( toEngineVector( aabb.lowerBound ), toEngineVector( aabb.upperBound ) );
	}

	AxisAlignedRectangle Physics2dBody::getBodyBounds() const
	{
		b2AABB aabb;
		m_pFixture->GetShape()->ComputeAABB( &aabb, m_pBody->GetTransform(), 0u );

		return createAxisAlignedRectangleMinMax( toEngineVector( aabb.lowerBound ), toEngineVector( aabb.upperBound ) );
	}

	b2Body* Physics2dBody::getNativeObject() const
	{
		return m_pBody;
	}
}