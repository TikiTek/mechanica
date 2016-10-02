#pragma once
#ifndef TIKI_PHYSICS2DBODY_HPP_INCLUDED
#define TIKI_PHYSICS2DBODY_HPP_INCLUDED

#include "tiki/physics2d/physics2dcollisionobject.hpp"

#include "tiki/base/types.hpp"

class b2Body;

namespace tiki
{
	class Physics2dShape;
	struct Vector2;
	
	class Physics2dBody : public Physics2dCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dBody );
		friend class Physics2dWorld;

	public:

							Physics2dBody();
		virtual				~Physics2dBody();

		void				create( const Physics2dShape& shape, const Vector2& position, float mass, float sensity, float friction );
		void				dispose();

		void				applyForce( const Vector2& direction );

		Vector2				getPosition() const;
		float				getRotation() const;

		virtual void*		getNativeObject() const TIKI_OVERRIDE_FINAL;

	private:

		b2Body*				m_pBody;
	};
}

#endif // TIKI_PHYSICS2DBODY_HPP_INCLUDED
