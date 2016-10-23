#pragma once
#ifndef TIKI_PHYSICS2DCOLLIDER_HPP_INCLUDED
#define TIKI_PHYSICS2DCOLLIDER_HPP_INCLUDED

#include "tiki/physics2d/physics2dcollisionobject.hpp"

#include "tiki/base/types.hpp"

namespace tiki
{
	class Physics2dWorld;
	class Physics2dShape;
	struct Vector2;
	
	class Physics2dCollider : public Physics2dCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( Physics2dCollider );

	public:

						Physics2dCollider();
		virtual			~Physics2dCollider();

		bool			create( Physics2dWorld& world, const Physics2dShape& shape, const Vector2& position, uint32 materialId = 0u );
		void			dispose( Physics2dWorld& world );

		virtual b2Body*	getNativeObject() const TIKI_OVERRIDE_FINAL;

	private:

		b2Body*			m_pBody;
		b2Fixture*		m_pFixture;
	};
}

#endif // TIKI_PHYSICS2DCOLLIDER_HPP_INCLUDED
