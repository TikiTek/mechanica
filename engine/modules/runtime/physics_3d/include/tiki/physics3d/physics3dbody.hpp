#pragma once
#ifndef TIKI_PHYSICS3DBODY_HPP_INCLUDED
#define TIKI_PHYSICS3DBODY_HPP_INCLUDED

#include "tiki/physics3d/physics3dcollisionobject.hpp"

#include "tiki/base/types.hpp"

#include "BulletDynamics/Dynamics/btRigidBody.h"

namespace tiki
{
	class Physics3dShape;
	struct Quaternion;
	struct Vector3;
	
	class Physics3dBody : public Physics3dCollisionObject
	{
		TIKI_NONCOPYABLE_CLASS( Physics3dBody );
		friend class Physics3dWorld;

	public:

							Physics3dBody();
		virtual				~Physics3dBody();

		void				create( const Physics3dShape& shape, const Vector3& position, float mass, bool freeRotation );
		void				dispose();

		void				applyForce( const Vector3& direction );

		void				getPosition( Vector3& position ) const;
		void				getRotation( Quaternion& rotation ) const;

		virtual void*		getNativeObject() const TIKI_OVERRIDE TIKI_FINAL;
		
	private:

		btRigidBody			m_ridgidBody;
	};
}

#endif // TIKI_PHYSICS3DBODY_HPP_INCLUDED
