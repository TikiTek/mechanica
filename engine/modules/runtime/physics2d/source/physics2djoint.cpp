#include "tiki/physics2d/physics2djoint.hpp"

#include <Box2D/Dynamics/Joints/b2RevoluteJoint.h>
#include <Box2D/Dynamics/Joints/b2RopeJoint.h>

namespace tiki
{
	Physics2dJoint::Physics2dJoint()
	{
		m_pJoint = nullptr;
	}

	Physics2dJoint::~Physics2dJoint()
	{
		TIKI_ASSERT( m_pJoint == nullptr );
	}

	bool Physics2dJoint::createAsRope( Physics2dWorld& world, const Physics2dBody& body1, const Vector2& localAnchor1, const Physics2dBody& body2, const Vector2& localAnchor2, float maxLength )
	{
		b2RopeJointDef jointDef;
		jointDef.bodyA				= body1.getNativeObject();
		jointDef.localAnchorA		= toPhysicsVector( localAnchor1 );
		jointDef.bodyB				= body2.getNativeObject();
		jointDef.localAnchorB		= toPhysicsVector( localAnchor2 );
		jointDef.maxLength			= maxLength;
		jointDef.collideConnected	= true;
		jointDef.userData			= this;

		m_pJoint = world.getNativeWorld().CreateJoint( &jointDef );
		return m_pJoint != nullptr;
	}

	bool Physics2dJoint::createAsRevolute( Physics2dWorld& world, const Physics2dBody& body1, const Vector2& localAnchor1, const Physics2dBody& body2, const Vector2& localAnchor2, bool enableMotor, float maxMotorTorque )
	{
		b2RevoluteJointDef jointDef;
		jointDef.bodyA				= body1.getNativeObject();
		jointDef.localAnchorA		= toPhysicsVector( localAnchor1 );
		jointDef.bodyB				= body2.getNativeObject();
		jointDef.localAnchorB		= toPhysicsVector( localAnchor2 );
		jointDef.enableMotor		= enableMotor;
		jointDef.maxMotorTorque		= maxMotorTorque;
		jointDef.collideConnected	= false;
		jointDef.userData			= this;

		m_pJoint = world.getNativeWorld().CreateJoint( &jointDef );
		return m_pJoint != nullptr;
	}

	void Physics2dJoint::dispose( Physics2dWorld& world )
	{
		if( m_pJoint != nullptr )
		{
			world.getNativeWorld().DestroyJoint( m_pJoint );
			m_pJoint = nullptr;
		}
	}

	void Physics2dJoint::setRevoluteMotorSteed( float motorSpeed )
	{
		TIKI_ASSERT( m_pJoint != nullptr );
		TIKI_ASSERT( m_pJoint->GetType() == e_revoluteJoint );

		b2RevoluteJoint* pJoint = (b2RevoluteJoint*)m_pJoint;
		pJoint->SetMotorSpeed( motorSpeed );
	}
}
