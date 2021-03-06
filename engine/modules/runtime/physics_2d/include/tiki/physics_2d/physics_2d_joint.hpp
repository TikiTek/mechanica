#pragma once

class b2Joint;

namespace tiki
{
	class Physics2dWorld;

	class Physics2dJoint
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dJoint );

	public:

					Physics2dJoint();
					~Physics2dJoint();

		bool		createAsRope( Physics2dWorld& world, const Physics2dCollisionObject& body1, const Vector2& localAnchor1, const Physics2dCollisionObject& body2, const Vector2& localAnchor2, float maxLength );
		bool		createAsRevolute( Physics2dWorld& world, const Physics2dCollisionObject& body1, const Vector2& localAnchor1, const Physics2dCollisionObject& body2, const Vector2& localAnchor2, bool enableMotor, float maxMotorTorque );
		void		dispose( Physics2dWorld& world );

		void		setRevoluteMotorSteed( float motorSpeed );

	private:

		b2Joint*	m_pJoint;
	};
}
