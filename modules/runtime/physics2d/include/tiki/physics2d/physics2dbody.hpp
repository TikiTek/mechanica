#pragma once
#ifndef TIKI_PHYSICS2DBODY_HPP_INCLUDED
#define TIKI_PHYSICS2DBODY_HPP_INCLUDED

#include "tiki/physics2d/physics2dcollisionobject.hpp"

#include "tiki/base/types.hpp"
#include "tiki/math/axisalignedrectangle.hpp"
#include "tiki/math/vector.hpp"

class b2Fixture;

namespace tiki
{
	class Physics2dWorld;
	class Physics2dShape;
	
	class Physics2dBody : public Physics2dCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dBody );
		friend class Physics2dWorld;

	public:

								Physics2dBody();
		virtual					~Physics2dBody();

		bool					create( Physics2dWorld& world, const Physics2dShape& shape, const Vector2& position, float density, float friction, bool fixedRotation = false );
		void					dispose( Physics2dWorld& world );

		void					applyForce( const Vector2& force, const Vector2& worldPoint );
		void					applyLocalForce( const Vector2& force, const Vector2& localPoint = Vector2::zero );

		void					applyLinearImpluse( const Vector2& impulse, const Vector2& worldPoint );
		void					applyLocalLinearImpluse( const Vector2& impulse, const Vector2& localPoint = Vector2::zero );

		Vector2					getPosition() const;
		float					getRotation() const;

		Vector2					getLinearVelocity() const;
		void					setLinearVelocity( const Vector2& value ) const;

		float					getAngularVelocity() const;
		void					setAngularVelocity( float value ) const;

		AxisAlignedRectangle	getShapeBounds() const;
		AxisAlignedRectangle	getBodyBounds() const;

		virtual b2Body*			getNativeObject() const TIKI_OVERRIDE_FINAL;

	private:

		b2Body*				m_pBody;
		b2Fixture*			m_pFixture;
	};
}

#endif // TIKI_PHYSICS2DBODY_HPP_INCLUDED
