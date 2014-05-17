#pragma once
#ifndef __TIKI_PHYSICSBODY_HPP_INCLUDED__
#define __TIKI_PHYSICSBODY_HPP_INCLUDED__

#include "tiki/base/types.hpp"

#include "BulletDynamics/Dynamics/btRigidBody.h"

namespace tiki
{
	struct PhysicsBodyParameters;
	
	class PhysicsBody
	{
		TIKI_NONCOPYABLE_CLASS( PhysicsBody );
		friend class PhysicsWorld;

	public:

							PhysicsBody();
							~PhysicsBody();

		void				create( const PhysicsBodyParameters& parameters );
		void				dispose();

	private:

		btRigidBody			m_ridgidBody;

	};
}

#endif // __TIKI_PHYSICSBODY_HPP_INCLUDED__
