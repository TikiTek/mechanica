#pragma once
#ifndef TIKI_PHYSICS3DCOLLIDER_HPP_INCLUDED
#define TIKI_PHYSICS3DCOLLIDER_HPP_INCLUDED

#include "tiki/physics3d/physics3dcollisionobject.hpp"

#include "tiki/base/types.hpp"

#include "BulletCollision/CollisionDispatch/btCollisionObject.h"

namespace tiki
{
	class Physics3dShape;
	struct Vector3;
	
	class Physics3dCollider : public Physics3dCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( Physics3dCollider );
		friend class Physics3dWorld;

	public:

							Physics3dCollider();
		virtual				~Physics3dCollider();

		void				create( const Physics3dShape& shape, const Vector3& position );
		void				dispose();

		virtual void*		getNativeObject() const TIKI_OVERRIDE TIKI_FINAL;

	private:

		btCollisionObject	m_collitionObject;
	};
}

#endif // TIKI_PHYSICS3DCOLLIDER_HPP_INCLUDED
