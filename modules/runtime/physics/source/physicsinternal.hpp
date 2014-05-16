#pragma once
#ifndef __TIKI_PHYSICSINTERNAL_HPP_INCLUDED__
#define __TIKI_PHYSICSINTERNAL_HPP_INCLUDED__

#include "LinearMath/btTransform.h"

namespace tiki
{
	struct PhysicColliderParameters
	{
		btCollisionShape*	pCollisionShape;
		btTransform			transform;
	};	
}

#endif // __TIKI_PHYSICSINTERNAL_HPP_INCLUDED__
