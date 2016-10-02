#pragma once
#ifndef TIKI_PHYSICS2DCOLLIDER_HPP_INCLUDED
#define TIKI_PHYSICS2DCOLLIDER_HPP_INCLUDED

#include "tiki/physics2d/physics2dcollisionobject.hpp"

#include "tiki/base/types.hpp"

class b2Body;

namespace tiki
{
	class Physics2dShape;
	struct Vector2;
	
	class Physics2dCollider : public Physics2dCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dCollider );
		friend class PhysicsWorld;

	public:

						Physics2dCollider();
		virtual			~Physics2dCollider();

		void			create( const Physics2dShape& shape, const Vector2& position );
		void			dispose();

		virtual void*	getNativeObject() const TIKI_OVERRIDE_FINAL;

	private:

		b2Body*			m_pBody;
	};
}

#endif // TIKI_PHYSICS2DCOLLIDER_HPP_INCLUDED
