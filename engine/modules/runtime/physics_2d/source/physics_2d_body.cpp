#include "tiki/physics_2d/physics_2d_body.hpp"

#include "tiki/math/vector.hpp"
#include "tiki/physics_2d/physics_2d_shape.hpp"
#include "tiki/physics_2d/physics_2d_world.hpp"

#include "physics_2d_internal.hpp"

#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_world.h>

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

	bool Physics2dBody::create( Physics2dWorld& world, const Physics2dShape& shape, const Vector2& position, float density, float friction, bool fixedRotation /* = false */, uint32 materialId /*= 0u*/ )
	{
		TIKI_ASSERT( m_pBody == nullptr );

		m_materialId = materialId;

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

	float Physics2dBody::getDensity() const
	{
		return m_pFixture->GetDensity();
	}

	void Physics2dBody::setDensity( float value )
	{
		m_pFixture->SetDensity( value );
	}

	float Physics2dBody::getFriction() const
	{
		return m_pFixture->GetFriction();
	}

	void Physics2dBody::setFriction( float value )
	{
		m_pFixture->SetFriction( value );
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

	uint Physics2dBody::getVertices( Slice<Vector2>& targetVertices ) const
	{
		const b2Shape* pShape = m_pFixture->GetShape();
		if( pShape->GetType() != b2Shape::e_polygon )
		{
			return 0u;
		}
		const b2PolygonShape* pPolygonShape = (const b2PolygonShape*)pShape;
		const int minCount = TIKI_MIN( (int)targetVertices.getCapacity(), pPolygonShape->m_count );

		targetVertices.clear();
		for( int i = 0u; i < minCount; ++i )
		{
			targetVertices.push( toEngineVector( pPolygonShape->m_vertices[ i ] ) );
		}

		return (uint)pPolygonShape->m_count;
	}

	AxisAlignedRectangle Physics2dBody::getShapeBounds() const
	{
		b2AABB aabb;
		m_pFixture->GetShape()->ComputeAABB( &aabb, m_pBody->GetTransform(), 0u );

		return AxisAlignedRectangle::createMinMax( toEngineVector( aabb.lowerBound ), toEngineVector( aabb.upperBound ) );
	}

	AxisAlignedRectangle Physics2dBody::getBodyBounds() const
	{
		b2AABB aabb;
		m_pFixture->GetShape()->ComputeAABB( &aabb, m_pBody->GetTransform(), 0u );

		return AxisAlignedRectangle::createMinMax( toEngineVector( aabb.lowerBound ), toEngineVector( aabb.upperBound ) );
	}

	b2Body* Physics2dBody::getNativeObject() const
	{
		return m_pBody;
	}
}