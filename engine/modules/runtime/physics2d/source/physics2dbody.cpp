#include "tiki/physics2d/physics2dbody.hpp"

#include "tiki/math/vector.hpp"
#include "tiki/physics2d/physics2dshape.hpp"

#include "physics2dinternal.hpp"

#include <Box2D/Dynamics/b2Body.h>

namespace tiki
{
	Physics2dBody::Physics2dBody()
	{
	}

	Physics2dBody::~Physics2dBody()
	{
	}

	void Physics2dBody::create( const Physics2dShape& shape, const Vector2& position, float mass, float sensity, float friction )
	{
	}

	void Physics2dBody::dispose()
	{
	}

	void Physics2dBody::applyForce( const Vector2& force )
	{
	}

	Vector2 Physics2dBody::getPosition() const
	{
		return toTikiVector( m_pBody->GetPosition() );
	}

	float Physics2dBody::getRotation() const
	{
		return m_pBody->GetAngle();
	}

	void* Physics2dBody::getNativeObject() const
	{
		return m_pBody;
	}
}