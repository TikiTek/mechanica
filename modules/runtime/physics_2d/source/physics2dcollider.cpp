#include "tiki/physics2d/physics2dcollider.hpp"

#include "tiki/math/vector.hpp"
#include "tiki/physics2d/physics2dshape.hpp"

#include "physics2dinternal.hpp"

namespace tiki
{
	Physics2dCollider::Physics2dCollider()
	{
		m_pBody		= nullptr;
		m_pFixture	= nullptr;
	}

	Physics2dCollider::~Physics2dCollider()
	{
		TIKI_ASSERT( m_pBody == nullptr );
		TIKI_ASSERT( m_pFixture == nullptr );
	}

	bool Physics2dCollider::create( Physics2dWorld& world, const Physics2dShape& shape, const Vector2& position, uint32 materialId /*= 0u*/ )
	{
		TIKI_ASSERT( m_pBody == nullptr );

		m_materialId = materialId;

		b2BodyDef bodyDef;
		bodyDef.type			= b2_staticBody;
		bodyDef.position		= toPhysicsVector( position );
		bodyDef.fixedRotation	= true;
		bodyDef.userData		= this;

		m_pBody = world.getNativeWorld().CreateBody( &bodyDef );
		if( m_pBody == nullptr )
		{
			return false;
		}

		b2Shape* pShape = shape.getNativeShape();
		if( pShape != nullptr )
		{
			b2FixtureDef fixtureDef;
			fixtureDef.shape	= pShape;
			fixtureDef.density	= 1.0f;
			fixtureDef.friction	= 1.0f;
			fixtureDef.userData	= this;

			m_pFixture = m_pBody->CreateFixture( &fixtureDef );
			if( m_pFixture == nullptr )
			{
				dispose( world );
				return false;
			}
		}

		return true;
	}

	void Physics2dCollider::dispose( Physics2dWorld& world )
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

	b2Body* Physics2dCollider::getNativeObject() const
	{	
		return m_pBody;
	}
}