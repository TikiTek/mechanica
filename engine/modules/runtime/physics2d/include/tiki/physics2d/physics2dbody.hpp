#pragma once
#ifndef TIKI_PHYSICS2DBODY_HPP_INCLUDED
#define TIKI_PHYSICS2DBODY_HPP_INCLUDED

#include "tiki/physics2d/physics2dcollisionobject.hpp"

#include "tiki/base/types.hpp"
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
		virtual				~Physics2dBody();

		bool				create( Physics2dWorld& world, const Physics2dShape& shape, const Vector2& position, float density, float friction );
		void				dispose( Physics2dWorld& world );

		void				applyForce( const Vector2& force, const Vector2& point = Vector2::zero );

		Vector2				getPosition() const;
		float				getRotation() const;

		virtual b2Body*		getNativeObject() const TIKI_OVERRIDE_FINAL;

	private:

		b2Body*				m_pBody;
		b2Fixture*			m_pFixture;
	};
}

#endif // TIKI_PHYSICS2DBODY_HPP_INCLUDED
